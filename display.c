#include "include/display.h"

#define pinguin_height  2
#define pinguin_width  2



void printEmoji(int x, int y){ //affiche le pinguin
    move(x, y);
    printw("🐧");

    refresh();
}


void showTiles(int x, int y){
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(y, x, "  #"); //tab-1
    mvprintw(y-1, x-1, "#     #"); //tab+3
    mvprintw(y-2, x-1, "#     #"); //tab+3
    mvprintw(y-3, x-1, "#     #"); //tab+3
    mvprintw(y-4, x, "  #");
    attroff(COLOR_PAIR(1));
}



void showIceFloe(int x, int y){
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 18; j++) {
            showTiles(x + j * 8, y + i * 6); // on gere l'espacement des #
            if(j % 2 != 1){
                x-=5; //on baisse la coordonne de x pour aligner a droite le losange
                y+=3;//on augmente la coordonne de y pour aligner a droite le losange
            }
            else{
                x-=5;  // si ont met + ont a 4 ligne mais decoller si ont met - on a 2 lignes et tout coller
                y-=3;
            }
        }
    }v}

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
