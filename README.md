# Boulder-Dash

Programação 2 class assignment for Federal University of Paraná

## Dependencies
This game uses the allegro 5 library. To install it, you can see how on their quickstart guide:
https://github.com/liballeg/allegro_wiki/wiki/Quickstart

or by doing the following, if you are using a Debian-based linux (Debian, Mint, Ubuntu, etc..):
```bash
sudo add-apt-repository ppa:allegro/5.2
sudo apt-get install liballegro-ttf5-dev
```

This project was only tested on linux, so other operating systems may not work out of the box.

## Running
Once you have installed allegro, simply run `make` to build the game and `make clean` to remove object files.

`make purge` will remove everything that is not a source file.

Now you can finally run the game with 
```bash
./boulder
```

## EasterEggs
Can be found if the player follows the steps in the middle of a game:
- pauses the game
- goes to the help screen
- sees the high scores 
- pressing backspace.

But beware to not be crushed by greed!!

You will also find the numbers (not the numpad!) very interesting.
