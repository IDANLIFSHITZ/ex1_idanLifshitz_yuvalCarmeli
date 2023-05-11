# Makefile

# Define the compiler
CC = gcc

QUEUE = /new_home/courses/mtm/public/2223b/ex1
TOOL = tool
CFLAGS = -std=c99 -lm -I$(QUEUE) -I$(TOOL) -Wall -pedantic-errors -Werror -DNDEBUG
TARGET = program
EXEC = HackEnrollment
DEBUG_FLAG = #now empty
OBJS = HackEnrollment.o IsraeliQueue.o main.o

COMPILE_TOOL = $(CC) $(CFLAGS) $(DEBUG_FLAG) -c $(TOOL)/$*.c -o $@

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJS) -o $(EXEC)

# Rule to build object files
main.o: $(TOOL)/main.c $(TOOL)/HackEnrollment.h
	$(COMPILE_TOOL)

HackEnrollment.o: $(TOOL)/HackEnrollment.c $(TOOL)/HackEnrollment.h $(QUEUE)/IsraeliQueue.h
	$(COMPILE_TOOL)

IsraeliQueue.o: IsraeliQueue.c $(QUEUE)/IsraeliQueue.h

# Rule to clean the project
clean:
	rm -f $(OBJS) $(EXEC)