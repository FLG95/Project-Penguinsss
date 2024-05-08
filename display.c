#include "deps/display.h"

#define pinguin_height  2
#define pinguin_width  2

void printEmoji(int x, int y){ //affiche le pinguin
    move(x, y);
    printw("🐧");
    refresh();
}


void showTiles(){
    initscr();
    start_color();
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

void showIceFloe(){
    for(int i = 0; i < 1; i++){
        for(int j  = 0; j < 2; j++){
            showTiles();
        }
    }
}

void destroyWin(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}




void makeWindow(){
    WINDOW* win;
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

    win = createWindow(pinguin_height, pinguin_width, startx, starty);
    showIceFloe();
    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);



    while((touch = getch()) != 27){
        switch(touch){
            case KEY_LEFT:
                if(starty > 0){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty--);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_RIGHT:
                if(starty < COLS - pinguin_width){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty++);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_UP:
                if(startx > 0){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_height, startx--, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case KEY_DOWN:
                if(startx < LINES - pinguin_height){ //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx++, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;
        }
    }
    endwin();
}
