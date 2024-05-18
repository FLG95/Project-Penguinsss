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

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
