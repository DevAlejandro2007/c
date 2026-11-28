CC = gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -I.
TARGET ?= tetris
SOURCES = main.c juego/logica.c juego/puntajes.c interfaz/render.c interfaz/ui.c platform/term.c
HEADERS = juego/tetris.h juego/puntajes.h interfaz/ui.h platform/platform.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) juego/*.o interfaz/*.o platform/*.o
