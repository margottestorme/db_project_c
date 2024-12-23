CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror
CLIBS = -lm
EXE = db_project
OBJ = obj/
SRC = src/

$(OBJ)main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)repl.o: $(SRC)repl.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXE): $(OBJ)main.o $(OBJ)repl.o
	$(CC) $(CFLAGS) -o $(EXE) $^ $(CLIBS)

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(EXE)

