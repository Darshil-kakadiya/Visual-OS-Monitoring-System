# Makefile for Virtual OS Monitor

CC = gcc
CFLAGS = -Wall
TARGET = monitor
SCRIPT = monitor.sh

all: $(TARGET) permissions

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

permissions:
	chmod +x $(SCRIPT)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) output.txt cpu_history.txt

.PHONY: all permissions run clean
