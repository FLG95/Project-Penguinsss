
#include "curses.h"

#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <unistd.h>

const int l = 4;
const int c = 4;

typedef struct {

    char* name;
    int num;


}player;

typedef struct{

    int fish;
    int isTherePlayer;
    int isAlive;

}tile;


player* createPlayers(){
    /*This function is used to create the player array for the game.*/
    int n = 0, b = 0;
    unsigned long length;
    char name[100];

    player* player;

    printf("How many players? between 2 and 6");
    scanf("%d", &n);

    while(n < 2 || n > 6){  /* We check that the number of players entered by the user is between 2 and 6.*/
        printf("between 2 and 6 please");
        scanf("%d", &n);
        if(b == 10) {
            printf("Too much try miss");
            exit(2);
        }
        b++;
    }

    player = malloc( n * sizeof(player));
    if(!player){ /*We check that the allocation has been successful.*/
        exit(1);
    }


    for (int i = 0; i < n; ++i) { /*For each player, we ask the user to enter the player's name
         and the we create a array containing the player's name and number.*/
        printf("Enter the name of the player %d", i+1);
        scanf("%s", name);
        length = strlen(name);

        player[i].name = malloc( length * sizeof(char));
        if(!player[i].name){ /*We check that the allocation has been successful.*/
            exit(1);
        }
        player[i].name = name;
        player[i].num = i+1;
    }

    return player;

}

tile createTiles(){
    /*This function create tiles and fills in each attribute.*/
    tile tile;

    tile.isAlive = 1;

    tile.fish = (rand()%3) + 1; /*Each tile contain between 1 and 3 fish.*/

    tile.isTherePlayer = 0;


}

tile** createBoard(){
    /*BONNE ou MAUVAISE fonction ??????????? */
    tile** board;

    board = malloc( c * sizeof(tile*));
    if(!board){
        exit(2);
    }

    for (int i = 0; i < l; ++i) {
        board[i] = malloc( l * sizeof (tile));
        if(!board[i]){
            exit(2);
        }
        for (int j = 0; j < c; ++j) {
            board[i][j] = createTiles();
        }
    }

    return board;
}

void showTiles(tile tiles){
    /*ON EN A PAS BESOIN DE CA !!!!!!!!!! */
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

void showBoard(tile** board){
    /*Inutile aussi a supprimer !!!!!!!!!!! */

    for (int i = 0; i < l; ++i) {
        for (int j = 0; j < c; ++j) {
            showTiles(board[i][j]);
            //printf("l : %d, c : %d\n", i, j);
        }

    }
    

}


int main() {


    tile** board = NULL;

    srand(time(NULL));


    initscr();
    refresh();

    sleep(1);

    getch();

    endwin();


    //player* players;

    //players = createPlayers();

    board = createBoard();


    showBoard(board);


    //scanf("%d", &a);

    //free(players);
    free(board);
    return 0;
}
