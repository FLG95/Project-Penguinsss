
#include "curses.h"

#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <unistd.h>

const int l = 4; // y
const int c = 4; // x
const int tilesSquare = 4;

typedef struct {

    char* name;
    int num;


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

void showTiles(tile tiles){

    int x= tiles.posX;
    int y = tiles.posY;


    move(tiles.posY, tiles.posX);

    for (int i = 0; i < tilesSquare; ++i) {// colone
        for (int j = 0; j < tilesSquare; ++j) { // ligne
            if(i == 0 || i == tilesSquare-1 ){
                printw("-");
            }
            else if(j == 0 || j == tilesSquare-1 ){
                printw("|");
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

void showBoard(tile** board, WINDOW *window){

    for (int i = 0; i < l; ++i) {
        for (int j = 0; j < c; ++j) {
            showTiles(board[i][j]);
        }
    }

    wrefresh(window);
}

void destroyTiles(tile tiles){

    int x= tiles.posX;
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


void mainCurse() {

    int ch;
    initscr();


    int height, width, start_y, start_x;
    tile** board = NULL;

    srand(time(NULL));

    height = l*tilesSquare;
    width = c*tilesSquare;

    start_y = 0;
    start_x = 0;

    WINDOW *window = newwin(height, width, start_y, start_x);
    wrefresh(window);


    //box(window, 0, 0);




/*
    player* players;

    //players = createPlayers();

*/

    board = createBoard();

    showBoard(board, window);

    destroyTiles(board[1][0]);


    int b = 1;
    while( b ){
        noecho(); // empeche d'écrire ce qu'on tape au clavier
        ch = getch();
        if( ch == 27){
            b = 0;
        }
    }


    //free(players);

    free(board);
    return 0;
}
