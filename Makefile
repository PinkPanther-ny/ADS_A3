CC=gcc
#CFLAGS=-Wall   -O3 -std=gnu99
CFLAGS +=  -g -std=gnu99


SRC=src/utils.o src/hashtable.o src/stack.o src/ai.o  peg_solitaire.o
TARGET=pegsol
LEVEL=2

all: build run clean

build: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

run:
	./$(TARGET) $(LEVEL)

clean:
	@rm -f $(TARGET) *.o src/*.o
