# 2048 Game

Welcome to the 2048 game! This is a simple yet addictive puzzle game implemented in C using the ncurses library.

## Description

2048 is a single-player sliding tile puzzle game. The objective is to slide numbered tiles on a grid to combine them and create a tile with the number 2048. The game is won when a tile with a value of 2048 appears on the board. Players can continue to play to reach higher scores even after achieving the 2048 tile.

## Features

- Classic 2048 gameplay
- Smooth sliding animations using ncurses
- Simple and intuitive keyboard controls
- Score tracking

## Controls

- **Arrow Keys**: Move tiles up, down, left, or right
- **Backspace**: Quit the game

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/2048.git
    ```
2. Navigate to the project directory:
    ```sh
    cd 2048
    ```
3. Compile the game:
    ```sh
    gcc -lncurses -o game game.c      

    ```
4. Run the game:
    ```sh
    ./2048
    ```

## Dependencies

- ncurses library

To install ncurses on Ubuntu, use:
```sh
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Acknowledgements

- Inspired by the original [2048 game](https://gabrielecirulli.github.io/2048/) by Gabriele Cirulli.

Enjoy the game and have fun reaching 2048!
