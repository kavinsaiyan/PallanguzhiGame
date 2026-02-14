CC = gcc
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE
LDFLAGS = -lczmq

TARGET = server

SRC = server.c

OBJ = $(SRC:.c=.o)

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
