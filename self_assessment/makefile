# name of target, compiler and compile flags
TARGET = testvector
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2 # -g # debug info for valgrind

# provide the rules default, all and clean
.PHONY: clean all run default

# default will compile the target
default: $(TARGET)

# all will be same as default
all: default

# all object files in the directory
# take all .c files and substitute the pattern .c with the suffix .o to get all .o files
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
# list of all needed header files (alternatively btree.h)
HEADERS = $(wildcard *.h)

# compile btree_test.c to an object file
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# keep target and objects if make is interrupted
.PRECIOUS: $(TARGET) $(OBJECTS)

# compile the object file
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

clean:
# clean all 
# '-' before rm tells the shell to ignore errors
	-rm -f btree_test.o # *.o
	-rm -f testvector

run: $(TARGET)
	./$(TARGET)
