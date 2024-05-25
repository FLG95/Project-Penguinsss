# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11

# Libraries
LIBS = -lncurses

# Source files
SRC = main.c display.c create.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
TARGET = Project_Pinguinsss

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compiling object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)
