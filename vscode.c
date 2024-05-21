
#include "ncurses.h"
#include "locale.h"
//#include "ncurses\curses.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"

// The code is highly commented. However, some functions, consts and 
// variable name are self explanatory, so they will not be commented.

const int l = 9; // y
const int c = 9; // x
const int tileHeigth = 8;
const int tileWidth = 8;



const int startTilesTabX = 5;       // In the window, X coordinate of the first tile at the top left of the game board

const int startTilesTabY = 5;       // In the window, Y coordinate of the first tile at the top left of the game board




typedef struct {

    int color;                      // Each player have penguins, and we'll use one color per player
    int tileX;                      // x position of the penguin on the board
    int tileY;                      // y position of the penguin on the board
    int num;                        // number of the penguin for a specific player
    int isMoveable;                 // can the penguin move ?

} Penguin;


typedef struct {

    char *name;
    int num;                        // Player number
    int currentPenguins;
    Penguin *penguin;               // tab of penguins
    int score;


} Player;

typedef struct {

    int fish;                       // Number of fishes on the tile
    int isTherePlayer;              // 0 if no, 1 if yes
    int isAlive;
    int posX;                       // x position on the board
    int posY;                       // y position on the board
    int penguinColor;               // Each player have penguins, and we'll use one color per player
    int isRed;                      // While playing, if the iceberg where the tile is located is selected, the tile will show red

} Tile;


Tile createTile(int y, int x) {

    Tile Tile;

    Tile.isAlive = 1;                       // By default, when the board is generated, all tiles are "alive" (ie not melted)

    Tile.fish = (rand() % 3) + 1;           // Fishes are generated randomly between 1 and 3

    Tile.isTherePlayer = 0;

    Tile.posX = x;
    Tile.posY = y;

    Tile.isRed = 0;

    return Tile;
}


Tile **createBoard() {
    Tile **board;
    int x = startTilesTabX; // origine en x de la premère tile
    int y = startTilesTabY; // origine en y de la première tile

    board = malloc(c * sizeof(Tile *));     // tab of tab of Tiles
    if (!board) {                           // if board == NULL || NB : this notation will often be used
        exit(2);
    }

    for (int i = 0; i < l; ++i) {           // y <=> line

        board[i] = malloc(l * sizeof(Tile));    // tab of Tiles <=> the line of the tab
        if (!board[i]) {
            exit(2);
        }
        for (int j = 0; j < c; ++j) {       // x <=> columns
            // Now we can generate Tile inside this tab of tab
            if (i % 2 == 0) {               // "Even" line
                board[i][j] = createTile(y + i * tileHeigth - 1, x + j * tileWidth);
            } else {                        // "Odd" line
                board[i][j] = createTile(y + i * tileHeigth - 1, x + 4 + j * tileWidth);
            }
            // We are doing this because we have to generate a hexagonal board
            // So one line in two have to be shifted slightly to the right
        }
        y -= 4;                             // ?

    }
    return board;
}


int PenguinsPerPlayer(int n) {
    int nbPenguins = 0;
    switch (n) {
        case 2:                                 // 2 players = 4 penguins per player (see game rules)
            nbPenguins = 4;
            break;
        case 3:                                 // 3 players = 3 penguins per player
            nbPenguins = 3;
            break;
        case 4:                                 // 4 players = 2 penguins per player
            nbPenguins = 2;
            break;
        case 5:                                 // 5 players = 1 penguins per player
            nbPenguins = 1;
            break;
        case 6:                                 // 6 players = 1 penguins per player
            nbPenguins = 1;
            break;
    }
    return nbPenguins;
}


Penguin createPenguin(Tile **board, int color, int num) {

    int randX, randY;
    Penguin penguin;

    // Penguins are spawning randomly on the game board  
    randX = rand() % 9;
    randY = rand() % 9;

    // Here, we are testing if there is already penguins at the coords we generated
    while (board[randY][randX].isTherePlayer != 0) {
        randX = rand() % 9;
        randY = rand() % 9;
    }
    // At this point, we found a Tile where there is no penguin that has already been generated
    board[randY][randX].isTherePlayer = 1;

    // penguins coords = coords we generated
    penguin.tileX = randX;
    penguin.tileY = randY;

    penguin.color = color;                  // We give a color to this penguin
    //penguin.num = num;

    board[randY][randX].penguinColor = color;       // At this random Tile without penguin, we assign the color of the penguin
    penguin.isMoveable = 1;                 // By default (at the beginning of the game), a penguin is moveable   


    return penguin;
}


Player *createTabPlayers(Tile **board, int nbPlayer) {
    int nbPenguin = 0;               // nbPenguin = numbers of penguins per player
    int try = 0;                    // Numbers of tries at the question "how many players"
    unsigned long length;
    char name[100];
    Player *tabPlayers;                 // tab of players

    while (nbPlayer < 2 || nbPlayer > 6) {
        scanf("%d", &nbPlayer);
    }
    tabPlayers = malloc(nbPlayer * sizeof(Player));            // Tab of nbPlayer players
    if (!tabPlayers) {
        exit(1);
    }

    nbPenguin = PenguinsPerPlayer(nbPlayer);

    // Going through this tab and ask for the name of each player
    for (int i = 0; i < nbPlayer; ++i) {
        mvprintw(5+i, 2, "Enter the name of the player %d:\n", i + 1);
        refresh();
        scanf("%s", name);
        length = strlen(name);

        // we use dynamic allocation for the player's name
        tabPlayers[i].name = malloc(length * sizeof(char));
        if (!tabPlayers[i].name) {
            exit(1);
        }


        tabPlayers[i].penguin = malloc(PenguinsPerPlayer(nbPlayer) * sizeof (Penguin));
        if (!tabPlayers[i].penguin) {
            tabPlayers[i].penguin = malloc(nbPenguin * sizeof(Penguin));
            exit(1);
        }
        
        // Attribution
        tabPlayers[i].num = i;
        strcpy(tabPlayers[i].name, name);



        for (int j = 0; j < nbPenguin; ++j) {
            tabPlayers[i].penguin[j] = createPenguin(board, i + 1, j);
        }

        tabPlayers[i].currentPenguins = 0;              // By default, we initialize the currentPenguins at 0 to avoid some bug
        tabPlayers[i].score = 0;                        // By default, each player have 0 points
    }

    return tabPlayers;

}

int colorPerPlayer(int currentPlayer) {     // At the beginning of the game, we give a penguin color to every player
    switch (currentPlayer) {
        // mvprintw is part of the ncurses library
        case 0: //Black
            mvprintw(6, 100, "You play the black penguins");        // print at the (y=6;x=100) coords the message 
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
}

void ColorPenguins(Tile tile, Player *player, int nbPlayer, int y, int x) {
    // This function displays each player's penguins with a different background to differentiate them from each other.
    switch ((tile.penguinColor)) {
        // each colour corresponds to the colour of a player's penguins
        // init_pair, attron, mvprintw, refresh an attroff are part of the ncurses library
        case 1:
            init_pair(1, COLOR_WHITE, COLOR_BLACK);           // we initialise the window background with those colors
            attron(COLOR_PAIR(1));                          // we start using those colors
            mvprintw(y + 1, x + 2, "🐧");                   // we print the penguin
            refresh();                                      // we update the window, because otherwise we can't see the difference
            attroff(COLOR_PAIR(1));                         // we stop using those colors
            break;

        case 2:
            init_pair(2, COLOR_WHITE, COLOR_WHITE);
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
    }
}


int showTile(Tile tile, Player *player, int nbPlayer) {     // print a "tile" with the coordinates stored in the "Tile" parameter
    int x = tile.posX;
    int y = tile.posY;
    int color;
    if (tile.isAlive == 0) {            // if tile not alive, then don't go further : no tile draw
        return 0;
    }

    // height =  4 ; width = 7

    if (tile.isRed == 1) {
        color = 10;
    } else {
        color = 11;
    }

    init_pair(10, COLOR_WHITE, COLOR_RED);
    init_pair(11, COLOR_WHITE, COLOR_CYAN);


    attron(COLOR_PAIR(color));

    if (tile.isTherePlayer == 0) {      // if no penguins on tile
        // can be replaced by a switch
        // "\U0001f41f" = fish emoji
        if (tile.fish == 1) {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "   \U0001f41f  "); // \U0001f41f code émoji marche pas sur mon linux
            mvprintw(y + 2, x, "       ");
            mvprintw(y + 3, x + 2, "   ");
        } else if (tile.fish == 2) {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "   \U0001f41f  ");
            mvprintw(y + 2, x, "   \U0001f41f  ");
            mvprintw(y + 3, x + 2, "   ");
        } else {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "  \U0001f41f\U0001f41f ");
            mvprintw(y + 2, x, "  \U0001f41f   ");
            mvprintw(y + 3, x + 2, "   ");
        }
    } else if (tile.isTherePlayer == 1) {

        mvprintw(y, x + 2, "   ");
        mvprintw(y + 1, x, "       ");
        mvprintw(y + 2, x, "       ");
        mvprintw(y + 3, x + 2, "   ");

        attroff(COLOR_PAIR(color));
        ColorPenguins(tile, player, nbPlayer, y, x); // ne réaffiche pas les pingouins quand on jouer
        //mvprintw(y + 1, x + 2, "🐧");

        // ou colorPinguins
    }


    attroff(COLOR_PAIR(color));

    // ici si la tile est dead la fonction va return direct (cf L 218) donc jsp si c'est mieux d'afficher une croix
    // "vide" genre noire grace aux espaces
    /*
    mvprintw(y, x + 2, "   ");
    mvprintw(y + 1, x, "       ");
    mvprintw(y + 2, x, "       ");
    mvprintw(y + 3, x + 2, "   ");
     */

    //mvprintw(y, x, "X"); Debug to show Anchor of the Tile

    return 1; // comme on en a fait une int pour pouvoir return 0, c mieux d'avoir un return pour chaque possibilité.
}


void showIceFloe(Tile **board, Player *player, int nbPlayer) {              // shows the ice floe and score
    for (int i = 0; i < l; i++) { // on boucle sur le nb de ligne
        for (int j = 0; j < c; j++) { //on boucle sur le nb de colone par ligne
            showTile(board[i][j], player, nbPlayer); // on print la tile grace aux donnés stocké dans board
        }
    }

    // at each frame, the score for each player is updated
    mvprintw(5, 150, "Score : ");
    for (int i = 0; i < nbPlayer; ++i) {
        mvprintw(6 + i, 150, "%s : %d", player[i].name, player[i].score);
    }

    //showPlayerScore;

    refresh();
}

// 
void InitCurse() {                  // curses initialization for our window
    // all the functions are part of the curses library
    setlocale(LC_ALL, "");          // check if the shell is in "UTF-8"
    initscr();                      // Initialise the window
    start_color();                  // starting colors
    cbreak();                       // configure the terminal to make user input more fluid in the terminal

    //keypad(stdscr, TRUE);

    noecho();                       // avoid the user input to be displayed at the screen

    curs_set(0);                    // deactivate the mouse cursor
    refresh();

}

void drawExit() { // On garde ? elle sert a quoi ????
    init_pair(10, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(10));
    printw("Press echap to exit\n \n");
    refresh();
    attroff(COLOR_PAIR(10));
    refresh();
}

void HomePage(){                    // This is the home page which is displayed when the game is launched

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


bool canMove(Tile **board, Penguin Penguin) {       // can the penguin move ? 


    // Checker si les cases autour du Pingouin sont morte ou prise par un autre Pingouin si c le cas return false sinon return true

}


int tileDontExist(int y, int x) {       // if don't exist, return 0. Else, return 1

    if (x > c || x < 0 || y > l || y < 0) {
        return 1;
    } else {
        return 0;
    }
}


void Inputs(Tile **board, Player *player, Penguin *virtualPenguin, int touch, int nbPlayer, int *extraMove) {
    /*
    This function handle movements in a player's turn
    All verifications are done before moving a player

    
    Keybinds :
    /!\ in AZERTY /!\ (cocorico)
    "a" = top corner left
    "e" = top corner right
    "q" = left
    "d" = right
    "w" = bottom corner left
    "c" = bottom corner right
    */


    int moreMove = 0;

    switch (touch) {  // rajouter des conditions pour x = 0;
        // in this switch, we use a "virtual penguin" to check if movements wanted by user are possible

        case 'a': // a Checker toute les conditions de déplacement avant

            if(virtualPenguin->tileX == 0){

                // This part of code is recurrent in this function, for comments we will use (COM1) to refer to the following comment :
                // If the tile is either not on the board, dead or taken by another penguin :
                if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) {
                    moreMove += 1;
                    mvprintw(25, 100, "non");
                }
                else {                                                            // else, meaning the penguin can move (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY - 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
                break;
            }


            if (virtualPenguin->tileY % 2 == 0) { // pour éviter un bug sur les lignes impaires     "even" line

                if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX - 1) ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX - 1].isAlive == 0 ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY - 1;
                    virtualPenguin->tileX = virtualPenguin->tileX - 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
            }
            else { // Ligne impaire         "odd" line
                if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX - 1) ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)

                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY - 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }

            }
            break;

        case 'e': // e Checker toute les conditions de déplacement avant

            if (virtualPenguin->tileY % 2 == 0) { // pour éviter un bug sur les lignes impaires

                if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY - 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
            }
            else { // Ligne impaire
                if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX + 1) ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX + 1].isAlive == 0 ||
                    board[virtualPenguin->tileY - 1][virtualPenguin->tileX + 1].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY - 1;
                    virtualPenguin->tileX = virtualPenguin->tileX + 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
            }
            break;


            break;

        case 'q':// q Checker toute les conditions de déplacement avant
            if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX - 1) ||
                board[virtualPenguin->tileY][virtualPenguin->tileX - 1].isAlive == 0 ||
                board[virtualPenguin->tileY][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)

                moreMove += 1;
            }
            else { // (COM2)

                board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;

                //délire condition pour y paire impaire


                virtualPenguin->tileY = virtualPenguin->tileY;
                virtualPenguin->tileX = virtualPenguin->tileX - 1;

                board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;

            }
            break;

        case 'd': // d Checker toute les conditions de déplacement avant
            if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX + 1) ||
                board[virtualPenguin->tileY][virtualPenguin->tileX + 1].isAlive == 0 ||
                board[virtualPenguin->tileY][virtualPenguin->tileX + 1].isTherePlayer == 1) { // (COM1)

                moreMove += 1;
            }
            else { // (COM2)

                board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;

                //délire condition pour y paire impaire

                virtualPenguin->tileY = virtualPenguin->tileY;
                virtualPenguin->tileX = virtualPenguin->tileX + 1;

                board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;

            }
            break;


        case 'w': // w Checker toute les conditions de déplacement avant

            if(virtualPenguin->tileX == 0){

                if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY + 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
                break;
            }


            if (virtualPenguin->tileY % 2 == 0) { // pour éviter un bug sur les lignes impaires

                if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX - 1) ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX - 1].isAlive == 0 ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY + 1;
                    virtualPenguin->tileX = virtualPenguin->tileX - 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
            } else { // Ligne impaire
                if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX) ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY + 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }

            }
            break;

        case 'x': // x Checker toute les conditions de déplacement avant
            if (virtualPenguin->tileY % 2 == 0) { // pour éviter un bug sur les lignes impaires

                if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX) ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)

                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY + 1;
                    virtualPenguin->tileX = virtualPenguin->tileX;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
            }
            else { // Ligne impaire
                if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX + 1) ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX + 1].isAlive == 0 ||
                    board[virtualPenguin->tileY + 1][virtualPenguin->tileX + 1].isTherePlayer == 1) { // (COM1)
                    moreMove += 1;
                }
                else { // (COM2)
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY + 1;
                    virtualPenguin->tileX = virtualPenguin->tileX + 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }

            }
            break;


        case 27:                            // 27 = echap : go back to the menu
            clear();
            exit(7);
            break;

        case KEY_ENTER: // Si ne peut/veut pas bouger
            break;


        case KEY_RESIZE:                            // if the user resize the window, we update the window
            showIceFloe(board, player, nbPlayer);
            break;
    }


    *extraMove = moreMove;

    showIceFloe(board, player, nbPlayer
    );
}


void deplacement(Tile **board, Player *player, Penguin *virtualPenguin, int touch, int nbPlayer, int *extraMove, int movementNb) {

    mvprintw(15, 100, "Now move your penguin if you cant move just try an other movement");

    for (int i = 0; i < movementNb;) {
        if (i == 0) {
            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
        }
        showIceFloe(board, player, nbPlayer);
        mvprintw(16, 100, "movement %d", i + 1);
        do {
            touch = getch();
            Inputs(board, player, virtualPenguin, touch, nbPlayer, extraMove);
            mvprintw(15, 100, "Now move your penguin if you cant move just try an other movement");
            mvprintw(16, 100, "movement %d", i + 1);


        } while (touch != 'a' && touch != 'e' && touch != 'q' && touch != 'd' && touch != 'w' && touch != 'x' &&
                 touch != KEY_ENTER); // While the user is pressing keys other than those expected
        i -= *extraMove;
        i++;                // i++ dans une boucle for ???
    }

    mvprintw(16, 100, "                                                ");
    mvprintw(15, 100, "Press L to confirm your deplacement or press k to remake it           ");
    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
}


void Score(Player *player, Tile *tile) {        // Add fishes collected by the player to his points 
    player->score += tile->fish; //une ligne de code dans une fonction ?? c'est inutile -- un peu oui
}



int simple(Tile** board, Penguin penguin, int y, int x) { // pour avoir un code plus lisible séparer j'ai séparer en plusieurs fonction les conditions
    if (tileDontExist(penguin.tileY + y, penguin.tileX + x) ||
        board[penguin.tileY + y][penguin.tileX + x].isAlive == 0 ||
        board[penguin.tileY + y][penguin.tileX + x].isTherePlayer == 1) {
        return 1;
    } else {
        return 0;
    }
}

int isPenguinMoveable(Tile** board, Penguin penguin){

    if (penguin.tileY % 2 == 0) { // condition sur les lignes paires

        if (simple(board, penguin, -1, 0) && simple(board, penguin, -1, +1) && simple(board, penguin, 0, -1) &&
            simple(board, penguin, 0, +1) && simple(board, penguin, +1, 0) && simple(board, penguin, -1, +1)) { // if penguin can't move
            return 0;
        }
        else{
            return 1;
        };

    } else { // Lignes impaires
        if (simple(board, penguin, -1, -1) && simple(board, penguin, -1, 0) && simple(board, penguin, 0, -1) &&
            simple(board, penguin, 0, +1) && simple(board, penguin, +1, -1) && simple(board, penguin, -1, 0)) { // if penguin can't move
            return 0;
        }
        else{
            return 1;
        };
    }
}


int isAllPlayerBlocked(Tile** board, Player* player, int nbPlayer){         // reminder : if all players can't move, the game end
    int blockedCount = 0;
    for (int i = 0; i < nbPlayer; ++i) {
        for (int j = 0; j < PenguinsPerPlayer(nbPlayer); ++j) {
            if(isPenguinMoveable(board, player[i].penguin[j])){
                player[i].penguin[j].isMoveable = 1;
            }
            else{
                player[i].penguin[j].isMoveable = 0;
                blockedCount++;
            }
        }
    }
    if( blockedCount == nbPlayer * PenguinsPerPlayer(nbPlayer)){            // if the numbers of penguins blocked == the sum of every penguins of each player
        return 1;
    }
    else{
        return 0;
    }
}


void Game(Tile **board) {                 // the main game function
    int touch;
    int turn = 0;
    int currentPlayer = 0;
    int nbPlayer;
    int selectedPenguinNb;
    int movementNb = 0;
    int extraMove = 0;
    int impossibleSelection;
    int nbPenguin;
    int disableL;
    int passK = 0;
    Player *player;

    HomePage();                             // shows the home page

    do {
        touch = getch();
        switch (touch) {
            case 'u':                       // Start a game
                break;
            case 27:                        // échap =  quit the game
                clear();
                exit(1);
                break;

            case KEY_RESIZE:                // If the window is resized by user, reprint the main menu
                HomePage();
                break;
        }
    } while (touch != 'u');               // While the user hasn't started a game

    clear();
    mvprintw(4, 2, "How many players? (between 2 and 6) press enter after you press the number");
    refresh();
    scanf("%d", &nbPlayer);

    player = createTabPlayers(board, nbPlayer);

    nbPenguin = PenguinsPerPlayer(nbPlayer);


    // PRINT LOOP :
    while (!isAllPlayerBlocked(board, player, nbPlayer)) {          // stop conditions : see game rules


        // demander a l'utilisateur de press Enter pour commenceer son tour
        clear();
        refresh();
        showIceFloe(board, player, nbPlayer);

        mvprintw(0, 50, " tour : %d", turn);
        mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only

        colorPerPlayer(currentPlayer);


        for (int i = 0; i < nbPenguin; ++i) {
            if(player[currentPlayer].penguin[i].isMoveable){
                mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                         player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
            }
            else{
                mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
            }
        }

        mvprintw(11, 100, "Choose Your Penguin");

        do {
            touch = getch();
            switch (touch) {
                case 'r': //a

                    if(player[currentPlayer].penguin[0].isMoveable == 1){
                        selectedPenguinNb = 0;
                        impossibleSelection = 0;
                    }
                    else{
                        impossibleSelection = 1;
                    }

                    break;
                case 't': //z
                    if(player[currentPlayer].penguin[1].isMoveable == 1){
                        selectedPenguinNb = 1;
                        impossibleSelection = 0;
                    }
                    else{
                        impossibleSelection = 1;
                    }
                case 'y': //e
                    if(player[currentPlayer].penguin[2].isMoveable == 1){
                        selectedPenguinNb = 2;
                        impossibleSelection = 0;
                    }
                    else{
                        impossibleSelection = 1;
                    }
                case 'u': //r
                    if(player[currentPlayer].penguin[3].isMoveable == 1){
                        selectedPenguinNb = 3;
                        impossibleSelection = 0;
                    }
                    else{
                        impossibleSelection = 1;
                    }

                case KEY_RESIZE:
                    showIceFloe(board, player, nbPlayer);
                    mvprintw(0, 50, " tour : %d", turn);
                    mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only

                    colorPerPlayer(currentPlayer);


                    for (int i = 0; i < nbPenguin; ++i) {
                        if(player[currentPlayer].penguin[i].isMoveable){
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                                     player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                        }
                        else{
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);

                        }
                    }

                    mvprintw(11, 100, "Choose Your Penguin");
                    break;

                case 27: // échap =  quitter le jeu
                    clear();
                    exit(1);
                    break;
            }
        } while (touch != 'r' && touch != 't' && touch != 'y' && touch != 'u' || selectedPenguinNb > nbPenguin || impossibleSelection);


        mvprintw(12, 100, "You chose the %d penguin", selectedPenguinNb + 1);


        Penguin virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];
        board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 1;
        showIceFloe(board, player, nbPlayer);

        mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6");
        /*
        Keybinds :
        /!\ in AZERTY /!\ (cocorico)
        "f" = distance = 1
        "g" = distance = 2
        "h" = distance = 3
        "j" = distance = 4
        "k" = distance = 5
        "l" = distance = 6
        */

        do {
            touch = getch();
            switch (touch) {
                case 'f':
                    movementNb = 1;
                    break;

                case 'g':
                    movementNb = 2;
                    break;

                case 'h':
                    movementNb = 3;
                    break;

                case 'j':
                    movementNb = 4;
                    break;

                case 'k':
                    movementNb = 5;
                    break;

                case 'l':
                    movementNb = 6;
                    break;

                case KEY_RESIZE:
                    showIceFloe(board, player, nbPlayer);
                    mvprintw(0, 50, " tour : %d", turn);
                    mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only
                    colorPerPlayer(currentPlayer);

                    for (int i = 0; i < nbPenguin; ++i) {
                        if(player[currentPlayer].penguin[i].isMoveable){
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                                     player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                        }
                        else{
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);

                        }
                    }

                    mvprintw(11, 100, "Choose Your Penguin");
                    mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6");

                    break;

                case 27: // échap =  quitter le jeu
                    clear();
                    exit(1);
                    break;
            }

        } while (touch != 'f' && touch != 'g' && touch != 'h' && touch != 'j' && touch != 'k' &&
                 touch != 'l'); // if the user tries to input another key than what is requested


        mvprintw(14, 100, "You want to do %d movement", movementNb);
        mvprintw(15, 100, "Press K");
        disableL = 1;

        touch = 'k';
        passK = 1;

        do {
            if(passK != 1){
                touch = getch();
            }
            switch (touch) {
                case 'l':

                    if(disableL == 1){
                        break;
                    }

                    board[player[currentPlayer].penguin[selectedPenguinNb].tileY][player[currentPlayer].penguin[selectedPenguinNb].tileX].isTherePlayer = 0;
                    board[player[currentPlayer].penguin[selectedPenguinNb].tileY][player[currentPlayer].penguin[selectedPenguinNb].tileX].isAlive = 0;

                    player[currentPlayer].penguin[selectedPenguinNb] = virtualPenguin;
                    Score(&player[currentPlayer], &board[virtualPenguin.tileY][virtualPenguin.tileX]);  // Add fishes collected by the player to his points 
                    mvprintw(16, 100, "Score : %d", player[currentPlayer].score);


                    board[virtualPenguin.tileY][virtualPenguin.tileX].isTherePlayer = 1;
                    board[virtualPenguin.tileY][virtualPenguin.tileX].penguinColor = currentPlayer + 1; // +1 parce que currentPlayer commence a 0 alors que le system de couleur commence a 1

                    break;

                case 'k':

                    board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 0;
                    virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];

                    showIceFloe(board, player, nbPlayer);
                    deplacement(board, player, &virtualPenguin, touch, nbPlayer, &extraMove, movementNb);
                    disableL = 0;
                    break;

                case KEY_RESIZE:

                    showIceFloe(board, player, nbPlayer);
                    mvprintw(0, 50, " tour : %d", turn);
                    mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only
                    colorPerPlayer(currentPlayer);

                    for (int i = 0; i < nbPenguin; ++i) {
                        if(player[currentPlayer].penguin[i].isMoveable){
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                                     player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                        }
                        else{
                            mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);

                        }
                    }

                    mvprintw(11, 100, "Choose Your Penguin");
                    mvprintw(14, 100, "You want to do %d movement", movementNb);
                    mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6");
                    mvprintw(15, 100, "Press K");
                    mvprintw(16, 100, "                                                ");
                    mvprintw(15, 100, "Press L to confirm your deplacement or press k to remake it           ");
                    break;

                case 27: // échap =  quitter le jeu
                    clear();
                    exit(1);
                    break;
            }
            passK = 0;

        } while (touch != 'l' || disableL == 1);


        clear();
        player[currentPlayer].currentPenguins +=
                (player[currentPlayer].currentPenguins + 1) % nbPenguin; // on boucle les pinguins de ce joueur
        currentPlayer = (currentPlayer + 1) % nbPlayer; // on boucle sur les joeueurs
        turn++;
    }

    clear();
    refresh();
    // boucle sur tout les joeur on gadre le meiluer socre de chaque 1v1 et on affiche le gagnant
    mvprintw(15, 100, " You win");
    // Afficher le gagnant le score de chaque joueur et proposé de rematch quitteez revenir au menue ect...
}


int main() {
    int nbPlayer;

    Tile **board = NULL;
    Player *players = NULL;

    srand(time(NULL));                      // necessary for the rand() function to work proprely


    InitCurse();                            // initialising curses

    board = createBoard();

    Game(board); // a mettre dans une boucle pour pouvoir rejouer blablabla

    //showScore(players);

    endwin();                               // close the window

    // freeing all the memory used
    free(players);
    free(board);

    return 0;

}