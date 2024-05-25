# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11

# Libraries
LIBS = -lncursesw

# Source files
SRC = main.c display.c create.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
TARGET = NFS_Pinguins

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)


# Clean target
clean:
	rm -f $(OBJ) $(TARGET)
