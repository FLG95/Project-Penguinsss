#ifndef PROJECT_PINGUINSSS_CREATE_H

#define PROJECT_PINGUINSSS_CREATE_H

#include "ncurses.h"
#include "locale.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define  l 9
#define  c 9
#define  tileHeight 4
#define tileWidth 8
#define startTilesTabX 5      // In the window, X coordinate of the first tile at the top left of the game board
#define startTilesTabY 5      // In the window, Y coordinate of the first tile at the top left of the game board


/*
const int l = 9; // y
const int c = 9; // x
const int tileHeight = 4;
const int tileWidth = 8;
const int startTilesTabX = 5;       // In the window, X coordinate of the first tile at the top left of the game board
const int startTilesTabY = 5;       // In the window, Y coordinate of the first tile at the top left of the game board
*/

typedef struct {
    int color;                      // Each player have penguins, and we'll use one color per player
    int tileX;                      // x position of the penguin on the board
    int tileY;                      // y position of the penguin on the board
    int isMoveable;                 // can the penguin move ?
} Penguin;


typedef struct {
    char *name;
    int num;                        // Player number
    int currentPenguins;            // Penguin played by player
    Penguin *penguin;               // Tab of penguins
    int score;
    int canPlay;
} Player;

typedef struct {
    int fish;                       // Number of fishes on the tile
    int isTherePlayer;              // 0 if no, 1 if yes
    int isAlive;
    int posX;                       // x position on the board
    int posY;                       // y position on the board
    int penguinColor;               // Each player have penguins, and we'll use one color per player
    int isRed;                      // While playing, if the iceberg where the tile is located is selected, the tile will show red
} Tile;


Tile createTile(int y, int x);
Tile **createBoard();
int PenguinsPerPlayer(int n);
Penguin createPenguin(Tile **board, int color);
Player *createTabPlayers(Tile **board, int nbPlayer);






#endif //PROJECT_PINGUINSSS_CREATE_H
