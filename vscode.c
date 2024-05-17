//#include "ncurses.h"
#include "locale.h"
#include "ncurses\curses.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"


const int l = 9; // y
const int c = 9; // x
const int tileHeigth = 8;
const int tileWidthlessone = 8;
const int pinguin_width = 2;
const int pinguin_height = 1;


const int startTilesTabX = 5; // coordoné X de la première Tile en haut a gauche du tableau
const int startTilesTabY = 5;// coordoné Y de la première Tile en haut a gauche du tableau



typedef struct {

    int color;
    int tilesY;
    int tilesX;

}Pinguin;


typedef struct {

    char* name;
    int num;
    Pinguin* pinguin;

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



Tile creatTile(int x, int y, int isTherePlayer){

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

            // fonction pour poser alatoirement les joueurs

            if(i % 2 == 0) {// ligne paire
                board[i][j] = creatTile(x + j * tileWidthlessone, y + i * tileHeigth-1, 1);
            }
            else{ // ligne impaire
                board[i][j] = creatTile(x+4 + j * tileWidthlessone, y + i * tileHeigth-1, 0);
            }
        }
        y-= 4;

    }
    return board;
}



int playerHandle(int n){
    int nbPinguuins = 0;
    switch(n){
        case 2: // 2 joueurs = 4 pinguins par joueur
            nbPinguuins = 4;
            break;
        case 3: // 3 joueurs = 3 pinguins par joueur
            nbPinguuins = 3;
            break;
        case 4: // 4 joueurs = 2 pinguins par joueur
            nbPinguuins = 2;
            break;
        case 5: // 5 joueurs = 2 pinguins par joueur
            nbPinguuins = 2;
            break;
        case 6: // 6 joueurs = 1 pinguins par joueur
            nbPinguuins = 2;
            break;
    }
    return nbPinguuins;
}


int colorHandle(int color){

    switch(color){

        case 1:

            break;
    }
}


Pinguin createPinguin(Tile** board, int color){
    int randX, randY;

    Pinguin pinguin;

    randX = rand() % 9;
    randY = rand() % 9;

    while(board[randX][randY].isTherePlayer != 0){
        randX = rand() % 9;
        randY = rand() % 9;
    }
    board[randX][randY].isTherePlayer = 1;
    pinguin.tilesX = randX;
    pinguin.tilesY = randY;
    pinguin.color = color;

    return pinguin;
}

Player* createPlayers(Tile** board){
    int n = 0, b = 0, randX, randYn, nbPinguins = 0;
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

    nbPinguins = playerHandle(n);

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


        player[i].pinguin = malloc(nbPinguins * sizeof (Pinguin));
        if(!player[i].pinguin){
            exit(1);
        }

        for (int j = 0; j < nbPinguins; ++j) {
            player[i].pinguin[j] = createPinguin(board, i);
        }
    }

    return player;

}



void printEmoji(int x, int y){ //affiche le pinguin
    move(x, y);
    printw("🐧");
    refresh();
}



void showTile(Tile tile){ // print une tile au cooordonés stockées dans la tile envoyé
    int x = tile.posX-1;
    int y = tile.posY;


    // height =  4 ;width = 7

    init_pair(1, COLOR_CYAN, COLOR_CYAN);

    attron(COLOR_PAIR(1));

    mvprintw(y, x+2, "   ");
    mvprintw(y+1, x, "       ");
    mvprintw(y+2, x, "       ");
    mvprintw(y+3, x+2, "   ");



    if(tile.isTherePlayer == 1){
        mvprintw(y+1, x+2, "🐧");
    }

    attroff(COLOR_PAIR(1));

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
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    cbreak(); // ??
    keypad(stdscr, TRUE);

    noecho();

    init_pair(10, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(10));
    printw("Press echap to exit\n \n");
    refresh();
    attroff(COLOR_PAIR(10));
    curs_set(0); // desactive le curseur
    refresh();

}


void HomePage(WINDOW *win){
    if(!win){
        exit(1);
    }
    wclear(win);

    int title_length = strlen("CY FISH");
    int title_width = (getmaxx(win) - title_length) / 2;
    wattr_set(win, A_BOLD, COLOR_MAGENTA, NULL); //je met le texte dans la fenetre en gras et en violet
    mvwprintw(win, 1, title_width, "CY FISH"); //affiche a la ligne 1 et colonne title length (pour le centrer)

    wattr_set(win, A_NORMAL, COLOR_WHITE, NULL);
    mvwprintw(win, 3, 2, "START THE GAME");
    mvprintw(win, 4, 2, "EXIT");

    wmove(win, 2, 0);

    int ch = wgetch(win); //on recupere la saisi de l'utilisateur
    
    switch (ch){

        case '1':
            // on lance la fonction jeu
        break;
        case '2':
            // on sort du jeu
            endwin(); 
            exit(1);
        default:
            mvwprintw(win, 6, 2, "Invalid selection, please select any key to continue.");
            wgetch(win);
        break;
    }

}


void ColorPinguins(Player * tab, int size){
    /*This function displays each player's pinguins with a different background to differentiate them from each other.*/
    for(int i = 0; i < size; i++){
        switch ((tab[i].num))
        {
        case '1':
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(10));
        break;
        
        case '2':
            init_pair(1, COLOR_WHITE, COLOR_BLUE);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(1));
        break;

        case '3':
            init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(1));
        break;

        case '4':
            init_pair(1, COLOR_WHITE, COLOR_RED);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(1));
        break;

        case '5':
            init_pair(1, COLOR_WHITE, COLOR_YELLOW);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(1));
        break;

        case '6':
            init_pair(1, COLOR_WHITE, COLOR_GREEN);
            attron(COLOR_PAIR(1));
            printw("\U0001f427");
            refresh();
            attroff(COLOR_PAIR(1));
        break;
        }
    }
}


/*
void Inputs(Player player) {
    int touch;
    int startx, starty;

    WINDOW *win;
    PinguinCoords p_coords;

    startx = (LINES - pinguin_height) / 2;
    starty = (COLS - pinguin_width) / 2;

    win = createWindow(pinguin_height, pinguin_width, startx, starty);


    while((touch = getch()) != 27) {
        switch (touch) {
            case 113:// q
                if (starty > 0) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty -= 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 100: // d
                if (starty < COLS - pinguin_width) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx, starty += 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 120: // x
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx += 8, starty += 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 97: // a
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx -= 8, starty -= 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 101: // e
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx -= 8, starty += 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

            case 119: // w
                if (startx < LINES - pinguin_height) { //verifie que le pinguin sort pas de l'écran
                    destroyWin(win);
                    win = createWindow(pinguin_height, pinguin_width, startx += 8, starty -= 8);
                    printEmoji(startx + pinguin_height / 2, starty + pinguin_width / 2);
                    p_coords.x = startx; //on met a jour les coordonnées du pinguin
                    p_coords.y = starty;
                }
                break;

        }
    }
    endwin();
}*/



void Inputs(Player player) {
    int touch;
    int x, y;

    PinguinCoords p_coords;

    x = player.pinguin->tilesX;
    y = player.pinguin->tilesY;

    while((touch = getch()) != 27) {
        switch (touch) {
            case 113:// q
                    mvprintw(y, x, "  ");
                    printEmoji(x + 8, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = x;

                break;

            case 100: // d
                    mvprintw(y, x, "  ");
                    printEmoji(x + pinguin_height / 2, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = y;

                break;

            case 120: // x
                    mvprintw(y, x, "  ");
                    printEmoji(x + pinguin_height / 2, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = y;

                break;

            case 97: // a
                    mvprintw(y, x, "  ");
                    printEmoji(x + pinguin_height / 2, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = y;

                break;

            case 101: // e
                    mvprintw(y, x, "  ");
                    printEmoji(x + pinguin_height / 2, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = y;

                break;

            case 119: // w
                    mvprintw(y, x, "  ");
                    printEmoji(x + pinguin_height / 2, y + pinguin_width / 2);
                    p_coords.x = x; //on met a jour les coordonnées du pinguin
                    p_coords.y = y;

                break;

        }
    }
    endwin();
}


int main(){
    int touch;
    int b;
    int shouldClose = 0;


    Tile** board = NULL;
    Player* players = NULL;

    srand(time(NULL));

    /*
    printf("Put your terminal window in fullscreen please. Then enter a key\n");
    scanf("%d", &b);
*/

    board = createBoard();

    players = createPlayers(board);


    //checkfish = checkFish(board, players);

    //showScore(players);



    InitCurse();


    showIceFloe(board);


    while((touch = getch()) != 27){

        //Inputs(players[0]);// a changer en fonction des différent joueurs
    }

    endwin();

    free(players);
    free(board);
    return 0;

}