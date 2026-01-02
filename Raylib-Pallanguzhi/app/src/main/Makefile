# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -I/usr/local/include/freetype2 -I/usr/local/include/harfbuzz
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -L/usr/local/lib -lfreetype -lharfbuzz

# Output file
TARGET = Game

# Source files
SRC = cpp/Game.c cpp/Board.c cpp/SlotSelector.c cpp/Queue.c cpp/Array.c cpp/MainMenu.c cpp/EndScreen.c cpp/SaveData.c cpp/Button.c cpp/Translation.c cpp/LanguageSelection.c cpp/TamilText.c cpp/TextRender.c

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
