
#ifndef PROJECT_PINGUINSSS_DISPLAY_H
#define PROJECT_PINGUINSSS_DISPLAY_H

#include "ncurses.h"
#include "create.h"
#include "create.h"



typedef struct {
    int x;
    int y;
} PinguinCoords;

void printEmoji(int x, int y);
void showTiles();
void showBoard(Tile** board, WINDOW *window, int l, int c);
void showIceFloe();
void destroyTiles(Tile tiles);

void makeWindow();
void Inputs();

void showTile00(Tile tile, int tilesSquareX, int tilesSquareY);
void showTile0X(Tile tile, int tilesSquare);
void showTileY0(Tile tile, int tilesSquare);
void showTileYX(Tile tile, int tilesSquare);



#endif //PROJECT_PINGUINSSS_DISPLAY_H
