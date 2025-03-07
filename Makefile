CC = gcc

CFLAGS   = -std=c11 -O2 -g -Wall -Wextra -pthread
IFLAGS   = -Ilib/sokol
IFLAGS  += -Ilib/cglm/include

LDFLAGS  = -lX11 -lXi -lXcursor -lGL -ldl -lpthread -lm

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean dirs libs run game

all: dirs game

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)
	./$(BIN)/game

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<

clean:
	rm -rf compile_commands.json $(BIN) $(OBJ)
