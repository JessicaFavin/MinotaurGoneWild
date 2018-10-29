CC=gcc
CFLAGS=-Wall
SDLFLAGS=-lSDL2
MATHFLAGS=-lm
NCURSESFLAGS=-lncurses
EXEC=launchTheBeast

all: $(EXEC)

run:
	./$(EXEC)

launchTheBeast: main.o display.o game.o maze.o
	$(CC) -o $@ $^ $(CFLAGS) $(MATHFLAGS) $(NCURSESFLAGS) $(SDLFLAGS)

maze.o: maze.h game.h
display.o: maze.h
game.o: display.h maze.h
main.o: display.h maze.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
