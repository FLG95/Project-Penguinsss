

#include "deps/display.h"
#include "time.h"

const int tilesSquare = 4;
const int l = 9; // y
const int c = 9; // x
const int pinguin_width = 2;
const int pinguin_height = 2;
const int height ;
const int width ;
const int startX = 0;
const int startY = 0;

int main(){

    Tile** board = NULL;
    Player* players;
    WINDOW* window;


    srand(time(NULL));

    window = createWindow(pinguin_height, pinguin_width, startX, startY);


    makeWindow();

    //players = createPlayers();

    board = createBoard(l, c, tilesSquare);

    showBoard(board, window, l, c);


    //checkfish = checkFish(board, players);

    //showBoard(board, window);
    //showScore(players);




    //free(players);
    //free(board);
    return 0;

}