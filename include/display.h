
#ifndef PROJECT_PINGUINSSS_DISPLAY_H
#define PROJECT_PINGUINSSS_DISPLAY_H

#include "ncurses.h"
#include "create.h"



typedef struct {
    int x;
    int y;
} PinguinCoords;

void printEmoji(int x, int y);
void showTiles(int x, int y);
void showBoard(Tile** board, WINDOW *window, int l, int c);
void showIceFloe(int x, int y);
void destroyTiles(Tile tiles);

void makeWindow();
void Inputs();




#endif //PROJECT_PINGUINSSS_DISPLAY_H
