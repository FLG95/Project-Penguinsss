#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct {

    char* name;
    int num;


}player;


player* createPlayers(){
    int n = 0, b = 0;
    unsigned long l;
    char name[100];

    player* player;

    printf("How many players? between 2 and 4");
    scanf("%d", &n);

    while(n < 2 || n > 4){
        printf("between 2 and 4 pleese");
        scanf("%d", &n);
        if(b == 10) {
            printf("Trop de tentative raté");
            exit(2);
        }
        b++;
    }

    player = malloc( n * sizeof(player));
    if(!player){
        exit(1);
    }


    for (int i = 0; i < n; ++i) {
        printf("Enter the name of the player %d", i);
        scanf("%s", name);
        l = strlen(name);

        player[i].name = malloc( l * sizeof(char));
        if(!player[i].name){
            exit(1);
        }
        player[i].name = name;
        player[i].num = i+1;
    }

    return player;

}


int main() {
    printf("Hello, World!\n");

    player* players;

    players = createPlayers();











    free(players);
    return 0;
}
