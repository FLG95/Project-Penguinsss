

#ifndef PROJECT_PINGUINSSS_CREATE_H

#define PROJECT_PINGUINSSS_CREATE_H

#include "ncurses.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"




/*
const int tilesSquare = 4;
const int l = 9; // y
const int c = 9; // x
const int pinguin_width = 2;
const int pinguin_height = 2;
const int height ;
const int width ;
const int startX = 0;
const int startY = 0;
*/


typedef struct {

    char* name;
    int num;
    int tileX;
    int tilesY;

}Player;

typedef struct{

    int fish;
    int isTherePlayer;
    int isAlive;
    int posX;
    int posY;

}Tile;


Player* createPlayers();
Tile createTiles();
Tile** createBoard(int l, int c, int tilesSquare);
WINDOW *createWindow(int height, int width, int startx, int starty);



#endif //PROJECT_PINGUINSSS_CREATE_H
