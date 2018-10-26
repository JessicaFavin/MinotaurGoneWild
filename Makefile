CC=gcc
CFLAGS=-Wall
SDLFLAGS=-lSDL2
MATHFLAGS=-lm
NCURSESFLAGS=-lncurses
EXEC=launchTheBeast

all: $(EXEC)

run:
	./$(EXEC)

launchTheBeast: main.o display.o
	$(CC) -o $@ $^ $(CFLAGS) $(MATHFLAGS) $(NCURSESFLAGS)

main.o: display.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
