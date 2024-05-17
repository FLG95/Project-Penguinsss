#include "ncurses.h"
#include "locale.h"
//#include "ncurses\curses.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"


const int l = 9; // y
const int c = 9; // x
const int tileHeigth = 8;
const int tileWidth = 8;
const int penguin_width = 2;
const int penguin_height = 1;


const int startTilesTabX = 5; // coordoné X de la première Tile en haut a gauche du tableau
const int startTilesTabY = 5;// coordoné Y de la première Tile en haut a gauche du tableau




typedef struct {

    int color;
    int tileX;
    int tileY;
    int num;


} Penguin;


typedef struct {

    char *name;
    int num;
    int currentPenguins;
    Penguin *penguin;


} Player;

typedef struct {

    int fish;
    int isTherePlayer;
    int isAlive;
    int posX;
    int posY;
    int penguinColor;

} Tile;


Tile creatTile(int y, int x) {

    Tile Tile;

    Tile.isAlive = 1;

    Tile.fish = (rand() % 3) + 1;

    Tile.isTherePlayer = 0;

    Tile.posX = x;
    Tile.posY = y;

    return Tile;
}


Tile **createBoard() {
    Tile **board;
    int x = startTilesTabX; // origine en x de la premère tile
    int y = startTilesTabY; // origine en y de la première tile

    board = malloc(c * sizeof(Tile *));
    if (!board) {
        exit(2);
    }

    for (int i = 0; i < l; ++i) { // y ligne

        board[i] = malloc(l * sizeof(Tile));
        if (!board[i]) {
            exit(2);
        }
        for (int j = 0; j < c; ++j) { // x colone

            if (i % 2 == 0) {// ligne paire
                board[i][j] = creatTile(y + i * tileHeigth - 1, x + j * tileWidth);
            } else { // ligne impaire
                board[i][j] = creatTile(y + i * tileHeigth - 1, x + 4 + j * tileWidth);
            }
        }
        y -= 4;

    }
    return board;
}


int playerHandle(int n) {
    int nbPenguins = 0;
    switch (n) {
        case 2: // 2 joueurs = 4 pinguins par joueur
            nbPenguins = 4;
            break;
        case 3: // 3 joueurs = 3 pinguins par joueur
            nbPenguins = 3;
            break;
        case 4: // 4 joueurs = 2 pinguins par joueur
            nbPenguins = 2;
            break;
        case 5: // 5 joueurs = 2 pinguins par joueur
            nbPenguins = 1;
            break;
        case 6: // 6 joueurs = 1 pinguins par joueur
            nbPenguins = 1;
            break;
    }
    return nbPenguins;
}



Penguin createPenguin(Tile **board, int color, int num) {

    int randX, randY;
    Penguin penguin;

    randX = rand() % 9;
    randY = rand() % 9;

    while (board[randY][randX].isTherePlayer != 0) {
        randX = rand() % 9;
        randY = rand() % 9;
    }
    board[randY][randX].isTherePlayer = 1;

    penguin.tileX = randX;
    penguin.tileY = randY;

    penguin.color = color;
    //penguin.num = num;

    board[randY][randX].penguinColor = color;



    return penguin;
}


Player *createPlayers(Tile **board, int nbPlayer) {
    int b = 0, nbPenguin = 0; // nb de pinguin par joueur
    unsigned long length;
    char name[100];
    Player *player;

    while (nbPlayer < 2 || nbPlayer > 6) {
        printf("between 2 and 6 please\n");
        scanf("%d", &nbPlayer);
        if (b == 10) {
            printf("Too much try miss");
            exit(2);
        }
        b++;
    }
    player = malloc(nbPlayer * sizeof(*player));
    if (!player) {
        exit(1);
    }

    nbPenguin = playerHandle(nbPlayer);

    for (int i = 0; i < nbPlayer; ++i) {
        printf("Enter the name of the player %d:\n", i + 1);
        scanf("%s", name);
        length = strlen(name);

        player[i].name = malloc(length * sizeof(char));
        if (!player[i].name) {
            exit(1);
        }

        player[i].name = name;
        player[i].num = i;


        player[i].penguin = malloc(nbPenguin * sizeof(Penguin));
        if (!player[i].penguin) {
            exit(1);
        }

        for (int j = 0; j < nbPenguin; ++j) {
            player[i].penguin[j] = createPenguin(board, i+1, j);
        }

        player[i].currentPenguins = 0;
    }

    return player;

}

char* colorHandle(int n){
    char* color;
    switch(n){
        case 1: //Black
            color = "black";
            break;
        case 2: // Blue
            color = "blue";
            break;
        case 3: // Magenta
            color = "magenta";
            break;
        case 4: // Red
            color = "red";
            break;
        case 5: // Yellow
            color = "yellow";
            break;
        case 6: // Green
            color = "green";
            break;
    }
    return color;
}


void ColorPenguins(Tile tile, Player* player, int nbPlayer, int y, int x){
    //This function displays each player's penguins with a different background to differentiate them from each other.
    //for(int i = 0; i < nbPlayer; i++){
        switch (( tile.penguinColor ))
        {
            case 1:
                init_pair(1, COLOR_WHITE, COLOR_BLACK);
                attron(COLOR_PAIR(1));
                mvprintw(y + 1, x + 2, "🐧");
                refresh();
                attroff(COLOR_PAIR(1));
                break;

            case 2:
                init_pair(2, COLOR_WHITE, COLOR_BLUE);
                attron(COLOR_PAIR(2));
                mvprintw(y + 1, x + 2, "🐧");
                refresh();
                attroff(COLOR_PAIR(2));
                break;

            case 3:
                init_pair(3, COLOR_WHITE, COLOR_MAGENTA);
                attron(COLOR_PAIR(3));
                mvprintw(y + 1, x + 2, "🐧");
                attroff(COLOR_PAIR(3));
                break;

            case 4:
                init_pair(4, COLOR_WHITE, COLOR_RED);
                attron(COLOR_PAIR(4));
                mvprintw(y + 1, x + 2, "🐧");
                attroff(COLOR_PAIR(4));
                break;

            case 5:
                init_pair(5, COLOR_WHITE, COLOR_YELLOW);
                attron(COLOR_PAIR(5));
                mvprintw(y + 1, x + 2, "🐧");
                attroff(COLOR_PAIR(5));
                break;

            case 6:
                init_pair(6, COLOR_WHITE, COLOR_GREEN);
                attron(COLOR_PAIR(6));
                mvprintw(y + 1, x + 2, "🐧");
                attroff(COLOR_PAIR(6));
                break;
        //}
    }
}


int showTile(Tile tile, Player* player, int nbPlayer) { // print une tile au cooordonés stockées dans la tile envoyé
    int x = tile.posX;
    int y = tile.posY;

    if (tile.isAlive == 0) { // if tile not alive dont go further = no tile draw

        return 0;
    }


    // height =  4 ;width = 7

    init_pair(10, COLOR_CYAN, COLOR_CYAN);

    attron(COLOR_PAIR(10));

    if(tile.isTherePlayer == 0){               // if no penguins on tile
        if(tile.fish == 1){
            mvprintw(y, x+2,   "   ");
            mvprintw(y+1, x, "   FF  "); // \U0001f41f code émoji marche pas sur mon linux
            mvprintw(y+2, x, "       ");
            mvprintw(y+3, x+2, "   ");
        }
        else if(tile.fish == 2){
            mvprintw(y, x+2,   "   ");
            mvprintw(y+1, x, "   FF  ");
            mvprintw(y+2, x, "   FF  ");
            mvprintw(y+3, x+2, "   ");
        }
        else{
            mvprintw(y, x+2,   "   ");
            mvprintw(y+1, x, "  FFFF ");
            mvprintw(y+2, x, "  FF   ");
            mvprintw(y+3, x+2, "   ");
        }
    }
    else if(tile.isTherePlayer == 1){

        mvprintw(y, x+2,   "   ");
        mvprintw(y+1, x, "       ");
        mvprintw(y+2, x, "       ");
        mvprintw(y+3, x+2, "   ");


        ColorPenguins(tile, player, nbPlayer, y, x);
        // ou colorPinguins
    }

    // ici si la tile est dead la fonction va return direct (cf L 218) donc jsp si c'est mieux d'afficher une croix
    // "vide" genre noire grace aux espaces

    /*
    mvprintw(y, x + 2, "   ");
    mvprintw(y + 1, x, "       ");
    mvprintw(y + 2, x, "       ");
    mvprintw(y + 3, x + 2, "   ");
     */
    attroff(COLOR_PAIR(10));

    if (tile.isTherePlayer == 1) {
        //move(y+1, x+2);
        ColorPenguins(tile, player, nbPlayer, y, x);

        //mvprintw(y + 1, x + 2, "🐧");
        /*
        if (tile.isTherePlayer == 1) { // mettre un switch pour les différente tile : soit détruite, soit poisson, soit penguin
            mvprintw(y + 1, x + 2, "🐧");
            refresh();
        }
        */


    }

    //mvprintw(y, x, "X"); Debug to show Anchor of the Tile

}


void showIceFloe(Tile **board, Player* player, int nbPlayer) {

    for (int i = 0; i < l; i++) { // on boucle sur le nb de ligne
        for (int j = 0; j < c; j++) { //on boucle sur le nb de colone par ligne
            showTile(board[i][j], player, nbPlayer); // on print la tile grace au donné stocké dans board
        }
    }
    refresh();
}


void InitCurse() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    cbreak();

    //keypad(stdscr, TRUE);

    noecho();

    curs_set(0); // desactive le curseur
    refresh();

}

void drawExit() { // On garde ?
    init_pair(10, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(10));
    printw("Press echap to exit\n \n");
    refresh();
    attroff(COLOR_PAIR(10));
    refresh();
}

void HomePage() {

    int title_length = strlen("CY FISH");
    int title_width = (getmaxx(stdscr) - title_length) / 2;


    init_pair(11, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(12, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(11));
    mvprintw(1, title_width, "CY FISH"); //affiche a la ligne 1 et colonne title length (pour le centrer)

    attroff(COLOR_PAIR((11)));
    attron(COLOR_PAIR(12));

    mvprintw(3, 2, "START THE GAME");
    mvprintw(4, 2, "EXIT");

    attroff(COLOR_PAIR(12));
}


bool canMove(Tile **board, Penguin Penguin) { // savoir si le pingouin peut bouger


    // Checker si les cases autour du Pingouin sont morte ou prise par un autre Pingouin si c le cas return false sinon return true

}


int tileDontExist(int y, int x) {

    if (x > c || x < 0 || y > l || y < 0) {
        return 1;
    } else {
        return 0;
    }
}

int Inputs(Tile **board, Penguin penguin, Player* player, int touch, int nbPlayer) {

    switch (touch) {

        case 97: // a Checker toute les conditions de déplacement avant

            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY - 1, penguin.tileX - 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY - 1][penguin.tileX - 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY - 1][penguin.tileX - 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY - 1][penguin.tileX - 1].isTherePlayer = 1;
                return 0;
            }
            break;

        case 101: // e Checker toute les conditions de déplacement avant
            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY - 1, penguin.tileX + 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY - 1][penguin.tileX + 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY - 1][penguin.tileX + 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY + 1][penguin.tileX + 1].isTherePlayer = 1;
                return 0;
            }
            break;

        case 113:// q Checker toute les conditions de déplacement avant
            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY, penguin.tileX - 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY][penguin.tileX - 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY][penguin.tileX - 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY][penguin.tileX - 1].isTherePlayer = 1;
                return 0;
            }
            break;

        case 100: // d Checker toute les conditions de déplacement avant
            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY, penguin.tileX - 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY][penguin.tileX + 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY][penguin.tileX + 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY][penguin.tileX + 1].isTherePlayer = 1;
                return 0;
            }
            break;


        case 119: // w Checker toute les conditions de déplacement avant
            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY, penguin.tileX - 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY + 1][penguin.tileX - 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY + 1][penguin.tileX - 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY + 1][penguin.tileX - 1].isTherePlayer = 1;
                return 0;
            }
            break;

        case 120: // x Checker toute les conditions de déplacement avant
            // si la case n'existe pas on return 1 pour continuer le tour du joueur
            if (tileDontExist(penguin.tileY + 1, penguin.tileX + 1)) { // la case n'est pas dans le tableau board
                return 1;
            } else if (board[penguin.tileY + 1][penguin.tileX + 1].isAlive == 0) { // la case n'est pas vivante
                return 1;
            } else if (board[penguin.tileY + 1][penguin.tileX + 1].isTherePlayer ==
                       1) { // la case est prise par un pingouin
                return 1;
            } else { // le pingouin peut bouger -> il bouge
                board[penguin.tileY][penguin.tileX].isTherePlayer = 0;
                showIceFloe(board, player, nbPlayer);
                board[penguin.tileY][penguin.tileX].isAlive = 0;

                board[penguin.tileY + 1][penguin.tileX + 1].isTherePlayer = 1;
                return 0;
            }
            break;

        case 27: // échap = revenir au menu et redémarrer tout le jeu
            clear();
            exit(7);
            break;

        case KEY_ENTER: // Si ne peut/veut pas bouger

            break;


        case KEY_RESIZE:

            break;
    }
}






Penguin selectPenguin(Penguin *penguin, int nbPenguin, int penguinNum) {

    Penguin selectedPenguin;

    for (int i = 0; i < nbPenguin; ++i) {
        if (penguin[i].num == penguinNum) {
            selectedPenguin = penguin[i];
        }

    }
    return selectedPenguin;
}


void Game(Player *player, Tile **board, int nbPlayer) {
    int touch;
    int turn = 0;
    int currentPlayer = 0;
    int nbPenguin = playerHandle(nbPlayer);
    int endTurn = 0;
    int selectedPenguinNb;
    Penguin selectedPenguin;

    HomePage();
    touch = getch();

    do {
        switch (touch) {
            case 'u': // lancer le jeu
                break;
            case 27: // échap =  quitter le jeu
                clear();
                exit(1);
                break;

            case KEY_RESIZE:
                HomePage();
                break;
        }
        touch = getch();
    } while ((touch != 'u' && touch != 27));


    // utiliser des modulo pour cycler sur les joueurs puis sur les penguins du joueur
    while (TRUE) { //condition d'arrêt

        // demander a l'utilisateur de press Enter pour commenceer son tour

        clear();
        showIceFloe(board, player, nbPlayer);

        mvprintw(0, 50, " tour : %d", turn);
        mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only

        switch(currentPlayer){
            case 0: //Black
                mvprintw(6, 100, "You play the black penguins");
                break;
            case 1: // Blue
                mvprintw(6, 100, "You play the blue penguins");
                break;
            case 2: // Magenta
                mvprintw(6, 100, "You play the magenta penguins");
                break;
            case 3: // Red
                mvprintw(6, 100, "You play the red penguins");
                break;
            case 4: // Yellow
                mvprintw(6, 100, "You play the yellow penguins");
                break;
            case 5: // Green
                mvprintw(6, 100, "You play the green penguins");
                break;
        }

        for (int i = 0; i < nbPenguin; ++i) {
            mvprintw(7+i, 100, "penguins: %d  in y: %d, x: %d", i+1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
        }

        mvprintw(11, 100, "Choose Your Penguin");

        do{
            touch = getch();
            switch(touch){
                case 'a':
                    selectedPenguinNb = 0;
                    break;
                case 'z':
                    selectedPenguinNb = 1;
                    break;
                case 'e':
                    selectedPenguinNb = 2;
                    break;
                case 'r':
                    selectedPenguinNb = 3;
                    break;

                case KEY_RESIZE:
                    showIceFloe(board, player, nbPlayer);
                    break;
            }

        }while(touch != 'a' && touch != 'z' && touch != 'e' && touch != 'r');


        selectedPenguin = selectPenguin(player[currentPlayer].penguin, nbPenguin, selectedPenguinNb);
        mvprintw(11, 100, "You chose the %d penguin", selectedPenguinNb+1);
        mvprintw(12, 100, "Enter the number of movement you want to do. Between 1 and 6");
        do{

        }while(true);// condition pour boucler sur les déplacements


        do {// On attend que le joueur appuie sur une bonne touch
            if (touch == KEY_RESIZE) {
                showIceFloe(board, player, nbPlayer);
            }
            endTurn = Inputs(board, selectedPenguin, player, nbPlayer, touch);
            touch = getch();

        } while (touch != 'a' && touch != 'e' && touch != 'q' && touch != 'd' && touch != 'w' && touch != 'x' ||
                 endTurn == 0);
        clear();





        // fonction print penguin en fonction des couleurs du joueur


        player[currentPlayer].currentPenguins += (player[currentPlayer].currentPenguins + 1) % nbPenguin; // on boucle les pinguins de ce joueur
        currentPlayer = (currentPlayer + 1) % nbPlayer; // on boucle sur les joeueurs
        turn++;
    }
}


int main() {
    int nbPlayer;

    Tile **board = NULL;
    Player *players = NULL;

    srand(time(NULL));


    board = createBoard();

    printf("How many players? (between 2 and 6)\n");
    scanf("%d", &nbPlayer);

    players = createPlayers(board, nbPlayer);


    InitCurse();

    Game(players, board, nbPlayer);


    //checkfish = checkFish(board, players);
    //showScore(players);

    endwin();

    free(players);
    free(board);
    return 0;

}