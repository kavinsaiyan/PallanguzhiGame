# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Output file
TARGET = Game

# Source files
SRC = src/Game.c src/Board.c src/SlotSelector.c src/Queue.c src/Array.c src/MainMenu.c src/EndScreen.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
