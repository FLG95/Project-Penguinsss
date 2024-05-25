
#ifndef PROJECT_PINGUINSSS_DISPLAY_H
#define PROJECT_PINGUINSSS_DISPLAY_H

#include "create.h"




void colorPerPlayer(int currentPlayer);     // At the beginning of the game, we give a penguin color to every player
void ColorPenguins(Tile tile, Player *player, int nbPlayer, int y, int x);
int showTile(Tile tile, Player *player, int nbPlayer);     // Print a "tile" with the coordinates stored in the "Tile" parameter
void showIceFloe(Tile **board, Player *player, int nbPlayer);              // Shows the ice floe and score
void InitCurse();                  // curses initialization for our window
void HomePage();                    // This is the home page which is displayed when the game is launched


#endif //PROJECT_PINGUINSSS_DISPLAY_H
