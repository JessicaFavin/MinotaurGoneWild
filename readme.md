# Minotaur gone wild

<!-- *********************************************************************** -->

## Installation and execution

### Librairies
To compile this porgram properly you'll need the following libraries :
* ttf-ancient-fonts
* SDL2 (libsdl2-dev)
* ncurses (libncurses5-dev)
* SOX (sox et libsox-fmt-mp3)

### Launch
To launch the game first compile the source file with the command ```make``` then execute ```./launchTheBeast [maze_file]```

If no file is given you'll be prompted for the number of lines and columns
and a random maze will be generated.
There are 3 available mode :
* Berserk mode - uses depth search to guide the minotaur to its food
* Smart mode - the minotaur can sense its food thanks to its powerful sense of smell
* Be the minotaur - you can play the minotaur

                    use the arrows or ZQSD to move and L to leave the game
