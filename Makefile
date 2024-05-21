
CC = gcc
CFLAGS = -Wall -Wextra -g

exec = tumasim

all: $(exec)

.PHONY: all

run: all
	./$(exec)

$(exec): $(exec).o

tumasim.o: tumasim.c

clean:
	rm -r tumasim tumasim.o
