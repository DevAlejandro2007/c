CC = gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET ?= tetris
SOURCES = main.c logica.c render.c platform/term.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) tetris.h platform/platform.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o platform/*.o
