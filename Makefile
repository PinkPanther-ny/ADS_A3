CC=gcc
CFLAGS=-Wall -g  -O3 -std=gnu99
#CFLAGS +=  -g -std=gnu99


SRC=src/utils.o src/hashtable.o src/stack.o src/ai.o  peg_solitaire.o
TARGET=pegsol
LEVEL=2
BUDGET = 1200000
RUN_ARGS=$(LEVEL) AI $(BUDGET) play_solution

VALGRIND = valgrind
VALGRIND_FLAG = --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes --tool=memcheck
VALGRIND_ARGS=$(LEVEL) AI $(BUDGET)


all: build run clean

build: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

run:
	./$(TARGET) $(RUN_ARGS)

clean:
	@rm -f $(TARGET) *.o src/*.o


valgrind: clean build
	clear
	$(VALGRIND) $(VALGRIND_FLAG) ./$(TARGET) $(VALGRIND_ARGS)
	make clean
