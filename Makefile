CC=gcc
CFLAGS=-Wall
SDLFLAGS=-lSDL2
MATHFLAGS=-lm
NCURSESFLAGS=-lncurses
EXEC=launchTheBeast

all: $(EXEC)

run:
	./$(EXEC)

launchTheBeast: main.o display.o game.o
	$(CC) -o $@ $^ $(CFLAGS) $(MATHFLAGS) $(NCURSESFLAGS)

game.o: maze.h
main.o: display.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
