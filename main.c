#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

const int ROW = 4;
const int COL= 4;

typedef struct {
    char* name;
    int num;
    int score;
    int penguins;
}Player;

typedef struct{
    int fish;
    int isTherePlayer;
    int isAlive;
}Tile;


Player* createPlayers(){
    int n = 0, b = 0;
    unsigned long length;
    char name[100];

    Player* players;

    do{
        printf("How many players? between 2 and 6");
        scanf("%d", &n);
        b++;
        if(b == 10) {
        printf("Too much try miss");
        exit(2);
    }
    }while(n < 2 || n > 6);

    players = malloc( n * sizeof(Player));
    if(players == NULL){
        exit(1);
    }


    for (int i = 0; i < n; ++i) {
        printf("Enter the name of the player %d", i+1);
        scanf("%s", name);
        length = strlen(name);

        players[i].name = malloc( length * sizeof(char));
        if(players[i].name == NULL){
            exit(1);
        }
        players[i].name = name;
        players[i].num = i+1;
        players[i].score = 0;
        if(n == 2){
            players[i].penguins = 4;
        }
        else if(n == 3){
            players[i].penguins = 3;
        }
        else if(n == 4){
            players[i].penguins = 2;
        }
        else{
            players[i].penguins = 1;
        }
    }

    return players;

}

Tile createTiles(){

    Tile tile;

    Tile.isAlive = 1;

    Tile.fish = (rand()%3) + 1;

    Tile.isTherePlayer = 0;


}

Tile** createBoard(){

    Tile** board;

    board = malloc( COL * sizeof(Tile*));
    if(board == NULL){
        exit(2);
    }

    for (int i = 0; i < ROW; ++i) {
        board[i] = malloc( ROW * sizeof (Tile));
        if(board[i] ==  NULL){
            exit(2);
        }
        for (int j = 0; j < COL; ++j) {
            board[i][j] = createTiles();
        }
    }

    return board;
}
int checkFish(Tile** board, Player* players){
    int cnt = 0;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(board[i][j].fish == 1){
                cnt++;
            }
        }
    }
    if(cnt < strlen(players)){
        return checkFish(createBoard(), players);
    }
    return 1;
}


void showTiles(Tile tiles){

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if(j == 0 || j == 2 || i == 0 || i == 2 ){
                printf("#");
            }
            else{
                printf(" ");
            }
        }
        if(i != 2){
            printf("\n");
        }
    }
}

void showBoard(Tile** board){


    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            showTiles(board[i][j]);
            //printf("ROW : %d, COL : %d\n", i, j);
        }

    }
    

}

void showScore(Player* players){
    length = strlen(players);
    for(int i = 0; i < length; i++){
        printf("Score joueur %s: %d\n", players[i].name, players[i].score);
    }

}

void checkMove(Tile** board, Player player){
    int penguin;
    do{
        printf("Choose your penguins that you want to move:\n");
        scanf("%d", penguin);
        //recuperer les coordonnees du penguin choisi
    }while(penguin < 1 || penguin > player.penguins);
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            //if(board[i][j].isTherePlayer == 1){
                //continue;
            //}
        }
    }
}

int main() {

    Tile** board = NULL;

    srand(time(NULL));


    //Player* players;

    //players = createPlayers();

    board = createBoard();
    //checkfish = checkFish(board, players);


    showBoard(board);
    //showScore(players);









    //free(players);
    free(board);
    return 0;
}
