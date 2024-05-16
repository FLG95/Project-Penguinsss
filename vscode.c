#include "ncurses.h"
//#include "ncurses\curses.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"



const int l = 9; // y
const int c = 9; // x
const int tileHeigth = 8;
const int tileWidthlessone = 8; //largeur de la tile -1 pour le décalage  7-1 = 6
const int pinguin_width = 2;
const int pinguin_height = 2;


const int startTilesTabX = 5; // coordoné X de la première Tile en haut a gauche du tableau
const int startTilesTabY = 5;// coordoné Y de la première Tile en haut a gauche du tableau

typedef struct {

    char* name;
    int num;
    int PX; // c'est quoi??
    int tilesY;
    int tilesX;
    int posX;
    int posY;
    WINDOW* pinguinsWin;
}Player;

typedef struct{

    int fish;
    int isTherePlayer;
    int isAlive;
    int posX;
    int posY;

}Tile;

typedef struct {
    int x;
    int y;
} PinguinCoords;

WINDOW *createWindow(int height, int width, int startX, int startY){
    WINDOW *win = newwin(height, width, startX, startY);
    box(win , 0, 0);
    wrefresh(win);

    return win;
}



Player* createPlayers(){
    int n = 0, b = 0, randX, randY;
    unsigned long length;
    char name[100];
    Player* player;


    printf("How many players? (between 2 and 6)\n");
    scanf("%d", &n);

    while(n < 2 || n > 6){
        printf("between 2 and 6 please\n");
        scanf("%d", &n);
        if(b == 10) {
            printf("Too much try miss");
            exit(2);
        }
        b++;
    }

    player = malloc( n * sizeof(player));
    if(!player){
        exit(1);
    }

    for (int i = 0; i < n; ++i) {
        printf("Enter the name of the player %d:\n", i+1);
        scanf("%s", name);
        length = strlen(name);

        player[i].name = malloc( length * sizeof(char));
        if(!player[i].name){
            exit(1);
        }

        player[i].name = name;
        player[i].num = i+1;

        randX = rand() % 10;
        randY = rand() % 10;

        player[i].posX = randX;
        player[i].posY = randY;
        player[i].pinguinsWin = createWindow(pinguin_height, pinguin_width, player[i].posX, player[i].posX);
    }

    return player;

}


Tile creatTile(int x, int y){

    Tile Tile;

    Tile.isAlive = 1;

    Tile.fish = (rand() % 3) + 1;


    Tile.isTherePlayer = 0;

    Tile.posX = x;
    Tile.posY = y;

    return Tile;
}



Tile** createBoard(){
    Tile** board;
    int x = startTilesTabX; // origine en x de la premère tile
    int y = startTilesTabY; // origine en y de la première tile

    board = malloc( c * sizeof(Tile*));
    if(!board){
        exit(2);
    }

    for (int i = 0; i < l; ++i) { // y ligne

        board[i] = malloc( l * sizeof (Tile));
        if(!board[i]){
            exit(2);
        }
        for (int j = 0; j < c; ++j) { // x colone

            if(i % 2 == 0) {// ligne paire
                board[i][j] = creatTile(x + j * tileWidthlessone, y + i * tileHeigth-1); // On créer notre tile avec les bonne coordoné  *6 car largeur d'une tile-1  *5 car hauteur d'une tile
            }
            else{ // ligne impaire
                board[i][j] = creatTile(x+4 + j * tileWidthlessone, y + i * tileHeigth-1); // on doit décaler notre tile de 3 vers la droite pour être au milleu de celle du dessus
            }
        }
        y-= 4;  // valeur a changer en fonction des tile qu'on affiche (de base 2)

    }
    return board;
}


void printEmoji(int x, int y){ //affiche le pinguin
    move(x, y);
    printw("🐧");
    refresh();
}



void showTile(Tile tile){ // print une tile au cooordonés stockées dans la tile envoyé
    int x = tile.posX-1;
    int y = tile.posY;

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));


    // height =  8 ;width = 7



/*
    mvprintw(y+1, x+2, "   ");
    mvprintw(y+4, x+2, "   ");
*/

    attroff(COLOR_PAIR(1));

    init_pair(2, COLOR_CYAN, COLOR_CYAN);
    attron(COLOR_PAIR(2));

    mvprintw(y, x+2, "   ");
    mvprintw(y+1, x, "       ");
    mvprintw(y+2, x, "       ");
    mvprintw(y+3, x+2, "   ");



    attroff(COLOR_PAIR(2));
}


void showIceFloe(Tile** board){
    int a = 0;
    for (int i = 0; i < l; i++) { // on boucle sur le nb de ligne
        for (int j = 0; j < c; j++) { //on boucle sur le nb de colone par ligne
            showTile(board[i][j]); // on print la tile grace au donné stocké dans board
        }
    }
    refresh();

}


void destroyWin(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}


void InitCurse(){
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
    refresh();

}


void Inputs(int touch, Player player){
    int startx, starty;
    WINDOW *win;
    PinguinCoords p_coords;


    startx = (LINES - pinguin_height) / 2;
    starty = (COLS - pinguin_width) / 2;

    win = createWindow(pinguin_height, pinguin_width, startx, starty);



    while((touch = getch()) != 27) {
        switch (touch) {
            case 113:
                if (starty > 0) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty--);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 100:
                if (starty < COLS - pinguin_width) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty++);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 122:
                if (startx > 0) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_height, startx--, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 120:
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx++, starty);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 97:
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx--, starty--);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 101:
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx--, starty++);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 119:
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx++, starty--);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 99:
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx++, starty++);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;
        }
    }
    endwin();

}

int main(){
    int touch;

    Tile** board = NULL;
    Player* players;

    WINDOW* window; //Sert a rien

    srand(time(NULL));


    //players = createPlayers();
    board = createBoard();


    //checkfish = checkFish(board, players);

    //showBoard(board, window);
    //showScore(players);

    InitCurse();

    showIceFloe(board);


    while((touch = getch()) != 27){

        Inputs(touch, players[0]);// a changer en fonction des différent joueurs

    }
    endwin();


    for (int i = 0; i < l; ++i) {
        printf("\n");
        for (int j = 0; j < c; ++j) {
            printf("%d", board[i][j].isTherePlayer);
        }

    }


    free(players);
    free(board);
    return 0;

}