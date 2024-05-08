
#include <ncurses.h>
#include "ncurses.h"

#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <unistd.h>
#include "wchar.h"


#define PLAYER_PAIR 1;


/*

  VARIANTE :

 GRILLES : 3
 START : 1
 FISH : 1


  (^v^)
 <(   )>
   W W

 /🐟\ /   \ /   \ /  \
|🐟🐟|     |     |    |
 \  / \   /  \   / \  /
   |    |   |   |   |
 /  \ /  \ /  \ /  \ /
|    |    |    |    |
 \  / \  / \  / \  /

aaaaaaaaaaaaa
🐟|︿﹀^
  ︿ ︿  ︿ ︿
 |🐟|🐟|🐟|🐟|
  ﹀ ﹀  ﹀ ﹀


L7 C9  millieu L4, C3
 0123456789
0    #       #       #
1  #   #   #   #   #   #
2#       #       #       #
3#       #       #       #
4#       #       #       #
6  #   #   #   #   #   #   #
7    #       #       #       #
8    #       #       #       #
9    #       #       #       #
   #   #   #   #   #   #   #
 #       #       #       #
 #       #       #       #
 #       #       #       #
   #   #   #   #   #   #
     #       #       #

 */



/*
 / \
| 🐟 |
 \ /



 (^v^)
<(   )>
  W W

  -O<

  🐧 🐟

 */



const int l = 4; // y
const int c = 4; // x

typedef struct {

    char* name;
    int num;
    int tileX;
    int tilesY;

}player;

typedef struct{

    int fish;
    int isTherePlayer;
    int isAlive;
    int posX;
    int posY;

}tile;



player* createPlayers(){
    int n = 0, b = 0;
    unsigned long length;
    char name[100];

    player* player;

    printf("How many players? between 2 and 6");
    scanf("%d", &n);

    while(n < 2 || n > 6){
        printf("between 2 and 6 please");
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
        printf("Enter the name of the player %d", i+1);
        scanf("%s", name);
        length = strlen(name);

        player[i].name = malloc( length * sizeof(char));
        if(!player[i].name){
            exit(1);
        }
        player[i].name = name;
        player[i].num = i+1;
    }

    return player;

}

tile creatTiles(int x, int y){

    tile tile;

    tile.isAlive = 1;

    tile.fish = (rand()%3) + 1;

    tile.isTherePlayer = 0;

    tile.posX = x;
    tile.posY = y;

    return tile;
}

tile** createBoard(){

    tile** board;

    board = malloc( c * sizeof(tile*));
    if(!board){
        exit(2);
    }

    for (int i = 0; i < l; ++i) { // y
        board[i] = malloc( l * sizeof (tile));
        if(!board[i]){
            exit(2);
        }
        for (int j = 0; j < c; ++j) { // x
            board[i][j] = creatTiles(j*tilesSquare, i*tilesSquare);
        }
    }

    return board;
}

void showTilesExa(tile tiles){

    int x = tiles.posX;
    int y = tiles.posY;


    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// colone
        for (int j = 0; j < tilesSquare; ++j) { // ligne


        }
        y++;
        if(i != tilesSquare-1){
            move( y, x);
        }
        refresh();

    }
}


void showTiles(tile tiles, bool first){

    int x;
    int y;

    if(first){// si première ligne on dessine normalement la case
        x = tiles.posX;
        y = tiles.posY;
    }
    else{ // sinon on décale de 1 cran pour superposer la case avec celle d'avant
        x = tiles.posX-1;
        y = tiles.posY-1;
    }

    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// ligne
        for (int j = 0; j < tilesSquare; ++j) { // colone
            if(i == 0 || i == tilesSquare-1 ){
                printw("#");
            }
            else if(j == 0 || j == tilesSquare-1 ){
                printw("#");
            }
            else{
                printw("-");
            }
        }

        y++;
        if(i != tilesSquare-1){
            move( y, x);
        }
        refresh();
    }
}





void showBoard(tile** board, WINDOW *window){

    for (int i = 0; i < l; ++i) {  // ligne
        for (int j = 0; j < c; ++j) {  // colone
            if( (i%2 == 1) && (j == 0)){

            }
            else{
                if(i == 0){
                    showTiles(board[i][j], true);
                }
                else {
                    showTiles(board[i][j], false);
                }
            }

        }
    }

    wrefresh(window);
}

void destroyTiles(tile tiles){

    int x = tiles.posX;
    int y = tiles.posY;


    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// colone
        for (int j = 0; j < tilesSquare; ++j) { // ligne
            if(i == 0 || i == tilesSquare-1 ){
                printw(" ");
            }
            else if(j == 0 || j == tilesSquare-1 ){
                printw(" ");
            }
            else{
                printw(" ");

            }
        }
        y++;
        if(i != tilesSquare-1){
            move( y, tiles.posX);
        }
        refresh();
    }
}

int closeGame(){
    char ch;


void Inputs(){

}

int main() {

    initscr();


    int height, width, start_y, start_x;
    tile **board = NULL;

    srand(time(NULL));

    height = l * tilesSquare;
    width = c * tilesSquare;

    start_y = 0;
    start_x = 5;

    WINDOW *window = newwin(height, width, start_y, start_x);
    wrefresh(window);

    if (has_colors() == false) {
        endwin();
        printf("Terminal dont support color \n");
        exit(1);
    }

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);



/*
    player* players;

    //players = createPlayers();

*/

    board = createBoard();

    showBoard(board, window);

    //destroyTiles(board[1][1]);


    attron(COLOR_PAIR(1));

    move(0, 0);
    printw("P");

    wrefresh(window);
    refresh();

    attroff(COLOR_PAIR(1));

    /*
    mvprintw(0, 0, "%lc \uFE3F");
    mvprintw(10, 10, "%lc \uFE40");
    refresh();
    */

    while (closeGame() == 0) {
        noecho(); // empeche d'écrire ce qu'on tape au clavier
        Inputs(); // fonction pour gérer les touches









    }


    //free(players);

    free(board);
    return 0;
}