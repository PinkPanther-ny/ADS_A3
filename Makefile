CC=gcc
CFLAGS=-Wall   -O6 -std=gnu99
#CFLAGS +=  -g -std=gnu99


SRC=src/utils.o src/hashtable.o src/stack.o src/ai.o  peg_solitaire.o
TARGET=pegsol
LEVEL=5

all: build run clean

build: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

run:
	./$(TARGET) $(LEVEL) AI 100000 play_solution

clean:
	@rm -f $(TARGET) *.o src/*.o
