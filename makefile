# Makefile

# define the C compiler to use
CC = gcc
# define object files
OBJS_FILES = main.o HackEnrollment.o IsraeliQueue.o
# define the executable file name
EXEC = HackEnrollment
# define the debug flag
DEBUG_FLAG = -g# now empty - will be filled in the debug rule
# define any compile-time flags
CFLAGS = -std=c99 -lm -I$(PATH) -Itool -Wall -pedantic-errors Werror -DNDEBUG

# define the path of the files
PATH = /new_home/courses/mtm/public/2223b/ex1
TOOL = tool

# define basic compile command
COMPILE_TOOL = $(CC) $(DEBUG_FLAG) $(CFLAGS) -c $(TOOL)/$*.c -o $@

# how to build the executable
program: $(OBJS_FILES)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS_FILES) -o $(EXEC)

main.o: $(TOOL)/main.c $(TOOL)/HackEnrollment.h
	$(COMPILE_TOOL)

HackEnrollment.o: $(TOOL)/HackEnrollment.c $(TOOL)/HackEnrollment.h $(PATH)/IsraeliQueue.h
	$(COMPILE_TOOL)

IsraeliQueue.o: IsraeliQueue.c $(PATH)/IsraeliQueue.h

clean:
	rm -f $(OBJS) $(EXEC)