#include "platform.h"

#include <stdio.h>

#if defined(_WIN32)

#include <conio.h>
#include <windows.h>

static HANDLE salida_consola;
static DWORD modo_consola_original;
static int terminal_configurado;

static BOOL WINAPI manejar_ctrl(DWORD tipo) {
    (void)tipo;
    restaurar_terminal();
    return FALSE;
}

int configurar_terminal(void) {
    salida_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    if (salida_consola == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(salida_consola, &modo_consola_original)) {
        return -1;
    }

    if (!SetConsoleMode(salida_consola,
                        modo_consola_original | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return -1;
    }

    terminal_configurado = 1;
    return 0;
}

void restaurar_terminal(void) {
    if (terminal_configurado) {
        printf("\033[?25h");
        printf("\033[?1049l");
        fflush(stdout);
        SetConsoleMode(salida_consola, modo_consola_original);
        terminal_configurado = 0;
    }
}

void instalar_manejadores(void) {
    SetConsoleCtrlHandler(manejar_ctrl, TRUE);
}

int tecla_presionada(void) {
    return _kbhit();
}

int leer_tecla(void) {
    int c = _getch();

    if (c == 0 || c == 224) {
        int codigo = _getch();
        switch (codigo) {
            case 72: return TECLA_ARRIBA;
            case 80: return TECLA_ABAJO;
            case 75: return TECLA_IZQUIERDA;
            case 77: return TECLA_DERECHA;
            default: return 0;
        }
    }

    if (c == '\r' || c == '\n') {
        return TECLA_ENTER;
    }
    if (c == 27) {
        return TECLA_ESCAPE;
    }
    if (c == ' ') {
        return TECLA_ESPACIO;
    }

    return c;
}

void pausa_ms(unsigned int milisegundos) {
    Sleep(milisegundos);
}

#else

#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#define ESPERA_SECUENCIA_MS 50

static struct termios modo_terminal_original;
static int terminal_configurado;

static void manejar_senal(int sig) {
    /*
     * Usamos write() y tcsetattr() en lugar de stdio: son async-signal-safe
     * y evitan un bloqueo si la senal interrumpe un printf() en curso.
     */
    static const char restauracion[] = "\033[?25h\033[?1049l";
    const char *cursor = restauracion;
    size_t restante = sizeof restauracion - 1;

    if (terminal_configurado) {
        while (restante > 0) {
            ssize_t escritos = write(STDOUT_FILENO, cursor, restante);
            if (escritos <= 0) {
                break;
            }
            cursor += escritos;
            restante -= (size_t)escritos;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal_original);
        terminal_configurado = 0;
    }

    _exit(128 + sig);
}

int configurar_terminal(void) {
    struct termios modo_terminal;
    if (tcgetattr(STDIN_FILENO, &modo_terminal_original) == -1) {
        return -1;
    }

    modo_terminal = modo_terminal_original;
    modo_terminal.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal) == -1) {
        return -1;
    }

    terminal_configurado = 1;
    return 0;
}

void restaurar_terminal(void) {
    if (terminal_configurado) {
        printf("\033[?25h");
        printf("\033[?1049l");
        fflush(stdout);
        tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal_original);
        terminal_configurado = 0;
    }
}

void instalar_manejadores(void) {
    struct sigaction accion = {0};

    accion.sa_handler = manejar_senal;
    sigemptyset(&accion.sa_mask);
    sigaction(SIGINT, &accion, NULL);
    sigaction(SIGTERM, &accion, NULL);
}

static int esperar_byte(int milisegundos) {
    fd_set descriptores;
    struct timeval espera;
    int resultado;

    FD_ZERO(&descriptores);
    FD_SET(STDIN_FILENO, &descriptores);

    espera.tv_sec = milisegundos / 1000;
    espera.tv_usec = (milisegundos % 1000) * 1000L;

    do {
        resultado = select(STDIN_FILENO + 1, &descriptores, NULL, NULL, &espera);
    } while (resultado < 0 && errno == EINTR);

    return resultado;
}

static int leer_byte(unsigned char *destino) {
    ssize_t leidos;

    do {
        leidos = read(STDIN_FILENO, destino, 1);
    } while (leidos < 0 && errno == EINTR);

    return leidos == 1;
}

int tecla_presionada(void) {
    return esperar_byte(0);
}

int leer_tecla(void) {
    unsigned char c;

    if (!leer_byte(&c)) {
        return EOF;
    }

    if (c == 27) {
        unsigned char inicio;
        unsigned char final = 0;
        int encontrado = 0;

        /* ESC solo: no hay mas bytes en los siguientes milisegundos. */
        if (esperar_byte(ESPERA_SECUENCIA_MS) <= 0) {
            return TECLA_ESCAPE;
        }
        if (!leer_byte(&inicio)) {
            return TECLA_ESCAPE;
        }

        /* Secuencias CSI (ESC [ ...) y SS3 (ESC O ...). */
        if (inicio != '[' && inicio != 'O') {
            return TECLA_ESCAPE;
        }

        for (int i = 0; i < 8; i++) {
            if (esperar_byte(ESPERA_SECUENCIA_MS) <= 0) {
                break;
            }
            if (!leer_byte(&final)) {
                break;
            }
            if (final >= 0x40 && final <= 0x7E) {
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            return TECLA_ESCAPE;
        }

        switch (final) {
            case 'A': return TECLA_ARRIBA;
            case 'B': return TECLA_ABAJO;
            case 'C': return TECLA_DERECHA;
            case 'D': return TECLA_IZQUIERDA;
            default: return 0;
        }
    }

    if (c == '\r' || c == '\n') {
        return TECLA_ENTER;
    }
    if (c == ' ') {
        return TECLA_ESPACIO;
    }

    return c;
}

void pausa_ms(unsigned int milisegundos) {
    struct timeval espera;

    espera.tv_sec = (long)(milisegundos / 1000);
    espera.tv_usec = (long)(milisegundos % 1000) * 1000L;
    select(0, NULL, NULL, NULL, &espera);
}

#endif

void limpiar_pantalla(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void ocultar_cursor(void) {
    printf("\033[?25l");
    fflush(stdout);
}

void mostrar_cursor(void) {
    printf("\033[?25h");
    fflush(stdout);
}

void entrar_pantalla_alterna(void) {
    printf("\033[?1049h");
    fflush(stdout);
}

void salir_pantalla_alterna(void) {
    printf("\033[?1049l");
    fflush(stdout);
}
