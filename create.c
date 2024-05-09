
#include "include/create.h"





Player* createPlayers(){
    int n = 0, b = 0;
    unsigned long length;
    char name[100];

    Player* player;

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

Tile creatTiles(int x, int y){

    Tile Tile;

    Tile.isAlive = 1;

    Tile.fish = (rand() % 3) + 1;

    Tile.isTherePlayer = 0;

    Tile.posX = x;
    Tile.posY = y;

    return Tile;
}

Tile** createBoard(int l, int c, int tilesSquare){

    Tile** board;

    board = malloc( c * sizeof(Tile*));
    if(!board){
        exit(2);
    }

    for (int i = 0; i < l; ++i) { // y
        board[i] = malloc( l * sizeof (Tile));
        if(!board[i]){
            exit(2);
        }
        for (int j = 0; j < c; ++j) { // x
            board[i][j] = creatTiles(j*tilesSquare, i*tilesSquare);
        }
    }

    return board;
}

WINDOW *createWindow(int height, int width, int startX, int startY){
    WINDOW *win = newwin(height, width, startX, startY);
    box(win , 0, 0);
    wrefresh(win);

    return win;
}