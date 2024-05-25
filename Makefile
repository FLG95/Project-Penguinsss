# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11 -Iinclude

# Libraries
LIBS = -lncursesw

# Source files
SRC = main.c display.c create.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
TARGET = NFS_Pinguins


# Linking the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Default target
all: $(TARGET)


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)
