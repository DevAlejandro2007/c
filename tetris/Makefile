CC = gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET ?= tetris
SOURCES = main.c logica.c render.c ui.c platform/term.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) tetris.h ui.h platform/platform.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o platform/*.o
