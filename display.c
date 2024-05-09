#include "deps/display.h"

#define pinguin_height  2
#define pinguin_width  2



void printEmoji(int x, int y){ //affiche le pinguin
    move(x, y);
    printw("🐧");

    refresh();
}


void showTiles(){
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    for(int i = 0; i < 11; i++){ //affiche la premiere colonne
        if(i == 0){
            printw("   #      #\n"); //tab-1 puis tab+2
        }
        else if(i == 10){
            printw("   #\n"); //tab-1
        }
        else if((i >= 4 && i < 7)){
            printw("   #     #\n"); //tab-1 puis tab+3
        }
        else{
            printw("#     #\n"); //tab+3
        }
    }
    attroff(COLOR_PAIR(1));
}

void showTile00(Tile tiles, int tilesSquareX, int tilesSquareY){

    move(tiles.posY, tiles.posX+3);
    printw("#");
}

void showTile0X(Tile tiles, int tilesSquare){

    int x;
    int y;

    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// ligne
        for (int j = 0; j < tilesSquare; ++j) { // colone


        }

        refresh();
    }
}
void showTileY0(Tile tiles, int tilesSquare){

    int x;
    int y;

    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// ligne
        for (int j = 0; j < tilesSquare; ++j) { // colone


        }

        refresh();
    }
}

void showTileYX(Tile tiles, int tilesSquare){

    int x;
    int y;

    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// ligne
        for (int j = 0; j < tilesSquare; ++j) { // colone


        }

        refresh();
    }
}



void showIceFloe(){
    for(int i = 0; i < 1; i++){
        for(int j  = 0; j < 2; j++){
            showTiles();
        }
    }
}

void showBoard(Tile** board, WINDOW *window, int l, int c) {

    for (int i = 0; i < l; ++i) {  // ligne
        for (int j = 0; j < c; ++j) {  // colone
            if ((i % 2 == 1) && (j == 0)) {

            } else {
                if (i == 0) {
                    showTiles(board[i][j]);
                } else {
                    showTiles(board[i][j]);
                }
            }

        }
    }
}

void destroyWin(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}




void makeWindow(WINDOW * window){
    PinguinCoords p_coords;
    int startx = 0, starty = 0;

    int touch;

    initscr();
    start_color();
    cbreak(); // ??
    keypad(stdscr, TRUE);

    noecho();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("Press echap to exit\n \n");
    refresh();
    attroff(COLOR_PAIR(1));
    curs_set(0); // desactive le curseur

    startx = (LINES - pinguin_height) / 2;
    starty = (COLS - pinguin_width) / 2;
    //printw("Press echap to exit");
    refresh();

    showIceFloe();
    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);



    while((touch = getch()) != 27){
        switch(touch){
            case KEY_LEFT:
                if(starty > 0){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(window);
                    window = createWindow(pinguin_height, pinguin_width, startx, starty--);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_RIGHT:
                if(starty < COLS - pinguin_width){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(window);
                    window = createWindow(pinguin_height, pinguin_width, startx, starty++);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_UP:
                if(startx > 0){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(window);
                    window = createWindow(pinguin_height, pinguin_height, startx--, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_DOWN:
                if(startx < LINES - pinguin_height){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(window);
                    window = createWindow(pinguin_height, pinguin_width, startx++, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;
        }
    }
    endwin();
}
