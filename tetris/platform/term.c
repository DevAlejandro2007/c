#include "../tetris.h"

#if defined(_WIN32)
#include <conio.h>
#include <windows.h>

static HANDLE salida_consola;
static DWORD modo_consola_original;
static int terminal_configurado;

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
        fflush(stdout);
        SetConsoleMode(salida_consola, modo_consola_original);
        terminal_configurado = 0;
    }
}

int tecla_presionada(void) {
    return _kbhit();
}

int leer_tecla(void) {
    return _getch();
}

void pausa_ms(unsigned int milisegundos) {
    Sleep(milisegundos);
}
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static struct termios modo_terminal_original;
static int terminal_configurado;

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
        fflush(stdout);
        tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal_original);
        terminal_configurado = 0;
    }
}

int tecla_presionada(void) {
    fd_set descriptores;
    struct timeval espera = {0, 0};

    FD_ZERO(&descriptores);
    FD_SET(STDIN_FILENO, &descriptores);
    return select(STDIN_FILENO + 1, &descriptores, NULL, NULL, &espera);
}

int leer_tecla(void) {
    unsigned char tecla;
    return read(STDIN_FILENO, &tecla, 1) == 1 ? tecla : EOF;
}

void pausa_ms(unsigned int milisegundos) {
    struct timeval espera;

    espera.tv_sec = (long)(milisegundos / 1000);
    espera.tv_usec = (long)(milisegundos % 1000) * 1000L;
    select(0, NULL, NULL, NULL, &espera);
}
#endif
