# Makefile

# define the compiler
CCompiler = gcc

# define the compiler flags
COMPILERFLAGS = -std=c99 -lm -I/home/mtm/public/2223b/ex1 -Itool -Wall -pedantic-errors Werror -DNDEBUG

# defines the name of the output executable
TARGET = program
EXEC = HackEnrollment
DEBUG_FLAG = # now empty - will be filled in the debug rule
TOOL = tool

# define object files
OBJS = HackEnrollment.o IsraeliQueue.o main.o

PATH = /new_home/courses/mtm/public/2223b/ex1

COMPILE_TOOL = $(CCompiler) $(DEBUG_FLAG) $(OBJS) -c $(TOOL)/$*.c -o $@

# how to build the executable
$(TARGET): $(OBJS)
	$(CCompiler) $(CFLAGS) $(DEBUG_FLAG) $(OBJS) -o $(EXEC)

# how to build object files
main.o: $(TOOL)/main.c $(TOOL)/HackEnrollment.h
	$(COMPILE_TOOL)

HackEnrollment.o: $(TOOL)/HackEnrollment.c $(TOOL)/HackEnrollment.h $(QUEUE)/IsraeliQueue.h
	$(COMPILE_TOOL)

IsraeliQueue.o: $(QUEUE)/IsraeliQueue.c $(QUEUE)/IsraeliQueue.h

# Rule to clean the project
clean:
	rm -f $(OBJS) $(EXEC)