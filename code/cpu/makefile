MEMFLAGS := --leak-check=full --show-leak-kinds=all

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c = .o)
CFLAGS := -std=c99 -g3
BIN := build
PROG := prog.o

DEFAULT_GOAL := $(BIN) 

$(BIN): $(SRC) main.c
	@ gcc $(CFLAGS) $^ -o $@  

memcheck:                                                                       
	@ valgrind $(MEMFLAGS) ./$(BIN) $(PROG)

run:
	@ ./$(BIN) $(PROG)

clean:
	@ rm -f $(BIN) 


