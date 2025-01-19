CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror
LDFLAGS = -lm
OBJ_DIR = obj
SRC_DIR = src
TARGET = test

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/repl.c $(SRC_DIR)/btree.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

