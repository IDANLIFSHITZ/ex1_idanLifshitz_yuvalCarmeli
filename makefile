# Makefile

# Define the compiler
CC = gcc

# Define the include directories
TOOL_DIR = tool
PATH = /new_home/courses/mtm/public/2223b/ex1

CFLAGS = -std=c99 -lm -I$(PATH) -I$(TOOL_DIR) -Wall -pedantic-errors -Werror -DNDEBUG
TARGET = program
EXEC = HackEnrollment
DEBUG_FLAG = #now empty
OBJS_FILES = HackEnrollment.o IsraeliQueue.o main.o

COMPILE_TOOL = $(CC) $(CFLAGS) $(DEBUG_FLAG) -c $(TOOL_DIR)/$*.c -o $@

# Rule to build the executable
$(TARGET): $(OBJS_FILES)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJS_FILES) -o $(EXEC)

# Rule to build object files
main.o: $(TOOL_DIR)/main.c $(TOOL_DIR)/HackEnrollment.h
	$(COMPILE_TOOL)

HackEnrollment.o: $(TOOL_DIR)/HackEnrollment.c $(TOOL_DIR)/HackEnrollment.h $(PATH)/IsraeliQueue.h
	$(COMPILE_TOOL)

IsraeliQueue.o: IsraeliQueue.c $(PATH)/IsraeliQueue.h

clean:
	rm -f $(OBJS_FILES) $(EXEC)