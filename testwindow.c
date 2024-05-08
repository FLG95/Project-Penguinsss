#include <stdio.h>
#include <ncurses.h>
#include <locale.h>



WINDOW *createWindow(int height, int width, int startx, int starty){
    WINDOW *win = newwin(height, width, startx, starty);
    box(win , 0, 0);
    wrefresh(win);
    return win;
}

void destroyWin(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

int makeWindow(){
    WINDOW *win;
    int startx, starty, height, width;
    int touch;
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    cbreak(); // ??
    keypad(stdscr, TRUE);

    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("Press echap to exit");
    refresh();
    attroff(COLOR_PAIR(1));
    
    height = 3;
    width = 10;
    startx = (LINES - height) / 2;
    starty = (COLS - width) / 2;
    //printw("Press echap to exit");
    refresh();

    win = createWindow(height, width, startx, starty);
    
    while((touch = getch()) != 27){
        switch(touch){
            case KEY_LEFT:
            destroyWin(win);
            win = createWindow(height, width, startx, --starty);
            break; 

            case KEY_RIGHT:
            destroyWin(win);
            win = createWindow(height, width, startx, ++starty);
            break; 

            case KEY_UP:
            destroyWin(win);
            win = createWindow(height, width, --startx, starty);
            break; 

            case KEY_DOWN:
            destroyWin(win);
            win = createWindow(height, width, ++startx, starty);
            break; 
        }
    }
    endwin();
    return 0;

}

int main(){
    makeWindow();
    return 0;
}