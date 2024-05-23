
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
    int isMoveable;                 // can the penguin move ?

} Penguin;


typedef struct {

    char *name;
    int num;                        // Player number
    int currentPenguins;            // penguin played by player
    Penguin *penguin;               // tab of penguins
    int score;
    int canPlay;


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

    Tile.fish = (rand() % 3) + 1;           // Fishes are generated randomly between 1 and 3 (see game rules)

    Tile.isTherePlayer = 0;

    Tile.posX = x;
    Tile.posY = y;

    Tile.isRed = 0;

    return Tile;
}


Tile **createBoard() {
    Tile **board;
    int x = startTilesTabX;                 // x origin of the first tile in the board
    int y = startTilesTabY;                 // y origin of the first tile in the board

    board = malloc(c * sizeof(Tile *));     // tab of tab of Tiles
    if (!board) {                           // If board == NULL || NB : this notation will often be used
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
        y -= 4;                             // Reduce distance between tiles

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


Penguin createPenguin(Tile **board, int color) {
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

    // At this point, we found a Tile where there is no penguin that has already been generated, so we tell the board that there is now a player on this tile
    board[randY][randX].isTherePlayer = 1;

    // Penguins coords = coords we generated
    penguin.tileX = randX;
    penguin.tileY = randY;

    penguin.color = color;                          // We give a color to this penguin
    board[randY][randX].penguinColor = color;       // At this random Tile without penguin, we assign the color of the penguin
    penguin.isMoveable = 1;                         // By default, at the beginning of the game, a penguin is moveable


    return penguin;
}


Player *createTabPlayers(Tile **board, int nbPlayer) {
    int nbPenguin = 0;               // nbPenguin = numbers of penguins per player
    int try = 0;                    // Numbers of tries at the question "how many players"
    unsigned long length;           // len of player name
    char name[100];
    Player *tabPlayers;                 // tab of players

    while (nbPlayer < 2 || nbPlayer > 6) {
        scanw("%d", &nbPlayer);                                 // scanw is part of the ncurses library : <=> scanf but in the game window
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
        scanw("%s", name);
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

        // Attribution of the player's number and name
        tabPlayers[i].num = i;
        strcpy(tabPlayers[i].name, name);

        // Creating all penguins for each player
        for (int j = 0; j < nbPenguin; ++j) {
            tabPlayers[i].penguin[j] = createPenguin(board, i + 1);
        }
        tabPlayers[i].currentPenguins = 0;              // By default, we initialize at 0 to avoid some bug
        tabPlayers[i].score = 0;                        // By default, each player have 0 points
        tabPlayers[i].canPlay = 1;                      // By default, each player can play
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
    }
}


int showTile(Tile tile, Player *player, int nbPlayer) {     // print a "tile" with the coordinates stored in the "Tile" parameter
int x = tile.posX;
int y = tile.posY;
int color;
if (tile.isAlive == 0) { 
    init_pair(27, COLOR_WHITE, COLOR_CYAN);
    attron(COLOR_PAIR(27));
    mvprintw(y, x + 2, "   ");
    mvprintw(y + 1, x, "       ");
    mvprintw(y + 2, x, "       ");
    mvprintw(y + 3, x + 2, "   ");

    attroff(COLOR_PAIR(27));           // if tile not alive, then don't go further : no tile draw
    return 0;
}

    // height =  4 ; width = 7

    if (tile.isRed == 1) {
        color = 10;
    } else {
        color = 11;
    }

init_pair(10, COLOR_WHITE, COLOR_RED);
init_pair(11, COLOR_WHITE, COLOR_WHITE);


    attron(COLOR_PAIR(color));

    if (tile.isTherePlayer == 0) {      // if no penguins on tile
        // can be replaced by a switch
        // "\U0001f41f" = fish emoji
        if (tile.fish == 1) {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "   \U0001f41f  "); // \U0001f41f code émoji marche pas sur mon linux
            mvprintw(y + 2, x, "       ");
            mvprintw(y + 3, x + 2, "   ");
        }
        else if (tile.fish == 2) {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "   \U0001f41f  ");
            mvprintw(y + 2, x, "   \U0001f41f  ");
            mvprintw(y + 3, x + 2, "   ");
        }
        else {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "  \U0001f41f\U0001f41f ");
            mvprintw(y + 2, x, "  \U0001f41f   ");
            mvprintw(y + 3, x + 2, "   ");
        }
    }
    else if (tile.isTherePlayer == 1) {
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


void InitCurse() {                  // curses initialization for our window
    // all the functions are part of the curses library
    setlocale(LC_ALL, "");          // check if the shell is in "UTF-8"
    initscr();                      // Initialise the window
    start_color();                  // starting colors
    cbreak();                       // configure the terminal to make user input more fluid in the terminal

    //keypad(stdscr, TRUE);

    noecho();                       // avoid the user input to be displayed at the screen

    curs_set(0);                    // deactivate the mouse cursor
    refresh();                      // refresh the window so all we did here is saved
}


void HomePage(){                    // This is the home page which is displayed when the game is launched

    int title_length = strlen("      CCCCCCCCCCCCCYYYYYYY       YYYYYYY     FFFFFFFFFFFFFFFFFFFFFFIIIIIIIIII   SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH");
    int title_width = (getmaxx(stdscr) - title_length) / 2;


    init_pair(11, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(12, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(11));
    mvprintw(1, title_width, "       CCCCCCCCCCCCCYYYYYYY       YYYYYYY     FFFFFFFFFFFFFFFFFFFFFFIIIIIIIIII   SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH"); //affiche a la ligne 1 et colonne title length (pour le centrer)
    mvprintw(2, title_width, "     CCC::::::::::::CY:::::Y       Y:::::Y     F::::::::::::::::::::FI::::::::I SS:::::::::::::::SH:::::::H     H:::::::H");
    mvprintw(3, title_width, "   CC:::::::::::::::CY:::::Y       Y:::::Y     F::::::::::::::::::::FI::::::::IS:::::SSSSSS::::::SH:::::::H     H:::::::H");
    mvprintw(4, title_width, "  C:::::CCCCCCCC::::CY::::::Y     Y::::::Y     FF::::::FFFFFFFFF::::FII::::::IIS:::::S     SSSSSSSHH::::::H     H::::::HH");
    mvprintw(5, title_width, " C:::::C       CCCCCCYYY:::::Y   Y:::::YYY       F:::::F       FFFFFF  I::::I  S:::::S              H:::::H     H:::::H");
    mvprintw(6, title_width, "C:::::C                 Y:::::Y Y:::::Y          F:::::F               I::::I  S:::::S              H:::::H     H:::::H");
    mvprintw(7, title_width, "C:::::C                  Y:::::Y:::::Y           F::::::FFFFFFFFFF     I::::I   S::::SSSS           H::::::HHHHH::::::H");
    mvprintw(8, title_width, "C:::::C                   Y:::::::::Y            F:::::::::::::::F     I::::I    SS::::::SSSSS      H:::::::::::::::::H");
    mvprintw(9, title_width, "C:::::C                    Y:::::::Y             F:::::::::::::::F     I::::I      SSS::::::::SS    H:::::::::::::::::H");
    mvprintw(10, title_width, "C:::::C                     Y:::::Y              F::::::FFFFFFFFFF     I::::I         SSSSSS::::S   H::::::HHHHH::::::H");
    mvprintw(11, title_width, "C:::::C                     Y:::::Y              F:::::F               I::::I              S:::::S  H:::::H     H:::::H");
    mvprintw(12, title_width, " C:::::C       CCCCCC       Y:::::Y              F:::::F               I::::I              S:::::S  H:::::H     H:::::H");
    mvprintw(13, title_width, "  C:::::CCCCCCCC::::C       Y:::::Y            FF:::::::FF           II::::::IISSSSSSS     S:::::SHH::::::H     H::::::HH");
    mvprintw(14, title_width, "   CC:::::::::::::::C    YYYY:::::YYYY         F::::::::FF           I::::::::IS::::::SSSSSS:::::SH:::::::H     H:::::::H");
    mvprintw(15, title_width, "     CCC::::::::::::C    Y:::::::::::Y         F::::::::FF           I::::::::IS:::::::::::::::SS H:::::::H     H:::::::H");
    mvprintw(16, title_width, "        CCCCCCCCCCCCC    YYYYYYYYYYYYY         FFFFFFFFFFF           IIIIIIIIII SSSSSSSSSSSSSSS   HHHHHHHHH     HHHHHHHHH");

    attroff(COLOR_PAIR((11)));
    attron(COLOR_PAIR(12));

    int start_length = strlen(" _ __     ___| |_ __ _ _ __| |_  | |_| |__   ___    __ _  __ _ _ __ ___   ___");
    int start_width = (getmaxx(stdscr) - start_length) / 2;

    mvprintw(20, start_width, "              _             _     _   _");
    mvprintw(21, start_width, "             | |           | |   | | | |");
    mvprintw(22, start_width, " _ __     ___| |_ __ _ _ __| |_  | |_| |__   ___    __ _  __ _ _ __ ___   ___");
    mvprintw(23, start_width, "| '_ \\   / __| __/ _` | '__| __| | __| '_ \\ / _ \\  / _` |/ _` | '_ ` _ \\ / _ \\");
    mvprintw(24, start_width, "| |_) |   \\__\\ || (_| | |  | |_  | |_| | | |  __/ | (_| | (_| | | | | | |  __/");
    mvprintw(25, start_width, "| .__(_) |___/\\__\\__,_|_|   \\__|  \\__|_| |_|\\___|  \\__, |\\__,_|_| |_| |_|\\___|");
    mvprintw(26, start_width, "| |                                                 __/ |");
    mvprintw(27, start_width, "|_|                                                |___/");

    int exit_length = strlen("|  __/ __|/ __/ _` | '_ \\/ _ \\  |  __\\\\/ / | __|");
    int exit_width = (getmaxx(stdscr) - exit_length) / 2;

    mvprintw(30, exit_width, " _____                           _____     _ _");
    mvprintw(31, exit_width, "|  ___|                         |  ___|   (_) |");
    mvprintw(32, exit_width, "| |__ ___  ___ __ _ _ __  ___   | |____  ___| |_");
    mvprintw(33, exit_width, "|  __/ __|/ __/ _` | '_ \\/ _ \\  |  __\\\\/ / | __|");
    mvprintw(34, exit_width, "| |__\\__\\ (_| (_| | |_) |  __/_ | |___>  <| | |_");
    mvprintw(35, exit_width, "\\____/___/\\___\\__,_| .__/\\___(_)\\____/_/\\_\\_|\\__|");
    mvprintw(36, exit_width, "                   | |");
    mvprintw(37, exit_width, "                   |_|");

    attroff(COLOR_PAIR(12));
    refresh();
}



int tileDontExist(int y, int x) {       // if don't exist, return 0. Else, return 1

    if (x > c-1 || x < 0 || y > l-1 || y < 0) {
        return 1;
    } else {
        return 0;
    }
}


void Inputs(Tile **board, Player *player, Penguin *virtualPenguin, int touch, int nbPlayer, int movementNb, int* retry, int currentPlayer, int turn) {
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

    int n = 0;                                          // n is a variable for moving to iceberg to iceberg
    int isBlocked = 0;                                  // Can the penguin move ? (If the next tile is either not on the board, dead or taken by another penguin) 
    int initialPosX = virtualPenguin->tileX;            // Save the original x position of the virtual penguin
    int initialPosY = virtualPenguin->tileY;            // Save the original y position of the virtual penguin

    switch (touch) {  // rajouter des conditions pour x = 0;
        // in this switch, we use a "virtual penguin" to check if movements wanted by user are possible, and move the "true penguin" if possible

        case 'a': // Before moving, we check if movement is possible (COM3)

            // While n < move value inputed by user (n will increment at each loop and we'll check if the user can actually make the move or not)
            while(n < movementNb && !isBlocked){
                if(virtualPenguin->tileX == 0){
                    // This part of code is recurrent in this function, for comments we will use (COM1) to refer to the following comment :
                    // If the tile is either not on the board, dead or taken by another penguin :
                    if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) {
                        isBlocked = 1;
                    }
                    else {                            // else, meaning the penguin can move (COM2)
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                        virtualPenguin->tileY = virtualPenguin->tileY - 1;
                        virtualPenguin->tileX = virtualPenguin->tileX;
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                    }
                }


                if (virtualPenguin->tileY % 2 == 0) {    //"even" line
                    if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX - 1) ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX - 1].isAlive == 0 ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)
                        isBlocked = 1;
                    }
                    else { // (COM2)
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                        virtualPenguin->tileY = virtualPenguin->tileY - 1;
                        virtualPenguin->tileX = virtualPenguin->tileX - 1;
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                    }
                }
                else { //       "odd" line
                    if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX - 1) ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                        isBlocked = 1;
                    }
                    else { // (COM2)
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                        virtualPenguin->tileY = virtualPenguin->tileY - 1;
                        virtualPenguin->tileX = virtualPenguin->tileX;
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                    }

                }
                n++;
            }

            break;

        case 'e': // (COM3)

            while(n < movementNb && !isBlocked){

                if (virtualPenguin->tileY % 2 == 0) {       // handeling even lines

                    if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isAlive == 0 ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                        isBlocked = 1;
                    } else { // (COM2)

                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                        virtualPenguin->tileY = virtualPenguin->tileY - 1;
                        virtualPenguin->tileX = virtualPenguin->tileX;
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                    }
                }
                else {                                 // odd lines 
                    if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX + 1) ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX + 1].isAlive == 0 ||
                        board[virtualPenguin->tileY - 1][virtualPenguin->tileX + 1].isTherePlayer == 1) { // (COM1)
                        isBlocked = 1;
                    }
                    else { // (COM2)
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                        virtualPenguin->tileY = virtualPenguin->tileY - 1;
                        virtualPenguin->tileX = virtualPenguin->tileX + 1;
                        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                    }
                }
                n++;
            }
            break;


        case 'q':   // (COM3)
            while(n < movementNb && !isBlocked){

                if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX - 1) ||
                    board[virtualPenguin->tileY][virtualPenguin->tileX - 1].isAlive == 0 ||
                    board[virtualPenguin->tileY][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)
                    isBlocked = 1;
                }
                else { // (COM2)
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY;
                    virtualPenguin->tileX = virtualPenguin->tileX - 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
                n++;
            }
            break;

        case 'd':   // (COM3)
            while(n < movementNb && !isBlocked){

                if (tileDontExist(virtualPenguin->tileY, virtualPenguin->tileX + 1) ||
                    board[virtualPenguin->tileY][virtualPenguin->tileX + 1].isAlive == 0 ||
                    board[virtualPenguin->tileY][virtualPenguin->tileX + 1].isTherePlayer == 1) { // (COM1)

                    isBlocked = 1;
                }
                else { // (COM2)
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                    virtualPenguin->tileY = virtualPenguin->tileY;
                    virtualPenguin->tileX = virtualPenguin->tileX + 1;
                    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                }
                n++;
            }
            break;


        case 'w':   // (COM3)
            while(n < movementNb && !isBlocked){

                if(virtualPenguin->tileY == 8 && movementNb > 0){
                    isBlocked = 1;
                }
                else{
                    if (virtualPenguin->tileX == 0) {
                        if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                            isBlocked = 1;
                        }
                        else { // (COM2)
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                            virtualPenguin->tileY = virtualPenguin->tileY + 1;
                            virtualPenguin->tileX = virtualPenguin->tileX;
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                        }
                    }

                    if (virtualPenguin->tileY % 2 == 0) { // handeling even lines

                        if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX - 1) ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX - 1].isAlive == 0 ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX - 1].isTherePlayer == 1) { // (COM1)
                            isBlocked = 1;
                        }
                        else { // (COM2)
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                            virtualPenguin->tileY = virtualPenguin->tileY + 1;
                            virtualPenguin->tileX = virtualPenguin->tileX - 1;
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                        }
                    }
                    else {                              // odd line
                        if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX) ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                            isBlocked = 1;
                        }
                        else { // (COM2)
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                            virtualPenguin->tileY = virtualPenguin->tileY + 1;
                            virtualPenguin->tileX = virtualPenguin->tileX;
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                        }
                    }
                }
                n++;
            }
            break;

        case 'c':   // (COM3)
            while(n < movementNb && !isBlocked){

                if(virtualPenguin->tileY == 8 && movementNb > 0){
                    isBlocked = 1;
                }
                else{
                    if (virtualPenguin->tileY % 2 == 0) { // handeling even lines
                        if (tileDontExist(virtualPenguin->tileY + 1, virtualPenguin->tileX) ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isAlive == 0 ||
                            board[virtualPenguin->tileY + 1][virtualPenguin->tileX].isTherePlayer == 1) { // (COM1)
                            isBlocked = 1;
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
                            isBlocked = 1;
                        }
                        else { // (COM2)
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
                            virtualPenguin->tileY = virtualPenguin->tileY + 1;
                            virtualPenguin->tileX = virtualPenguin->tileX + 1;
                            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
                       }
                    }
                }
                n++;
            }
            break;

        case 27:                    // 27 = escape : go back to the menu
            clear();
            exit(7);
            break;

        case KEY_RESIZE:                         // if the user resize the window, we update the window
            showIceFloe(board, player, nbPlayer);
            mvprintw(0, 50, " tour : %d", turn);
            mvprintw(5, 100, "%s", player[currentPlayer].name); // debug only
            colorPerPlayer(currentPlayer);

            for (int i = 0; i < PenguinsPerPlayer(nbPlayer); ++i) {
                if(player[currentPlayer].penguin[i].isMoveable){
                    mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                             player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                }
                else{
                    mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                }
            }
            mvprintw(13, 100, "Enter the number of movement you want to do ( between 1 and 6 ), then press enter.");
            mvprintw(14, 100, "You will move of  %d tile(s)", movementNb);
            mvprintw(15, 100, "Now move your penguin ");

            break;
    }

    showIceFloe(board, player, nbPlayer);
    if(isBlocked == 1){                         // if the penguin is blocked (ie the movement wanted by user is impossible)
        *retry = 1;                             // the user will have to choose another movement
        mvprintw(15, 100, "This move isn't valid please choose an other move");
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
        virtualPenguin->tileY = initialPosY;
        virtualPenguin->tileX = initialPosX;
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
    }

}


void deplacement(Tile **board, Player *player, Penguin *virtualPenguin, int touc, int nbPlayer, int currentPlayer, int turn) {

    int touch;              // Input from user
    mvprintw(15, 100, "                                                                                   ");

    int movementNb;
    int retry = 0;           // Did the user messed up input (pressed the wrong key)


    do {
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
        showIceFloe(board, player, nbPlayer);

        if(retry == 1){                             // If the movement requested by user is impossible
            mvprintw(16, 100, "Please press k ");   // We tell the user to press k to retry another movement
            do{
                touch = getch();
            }while(touch != 'k');
            mvprintw(15, 100, "                                                                          ");
            mvprintw(16, 100, "                                                 ");

            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;

        }

        do{
            movementNb = 0;
            mvprintw(13, 100, "Enter the number of movement you want to do ( between 1 and 6 ), then press enter.");
            refresh();
            scanw("%d", &movementNb);
            mvprintw(13, 100, "a Enter");


        }while( movementNb < 1 || movementNb > 6);


        retry = 0;

        mvprintw(14, 100, "You will move of  %d tile(s)", movementNb);
        mvprintw(15, 100, "Now move your penguin");
        refresh();

        touch = getch();
        Inputs(board, player, virtualPenguin, touch, nbPlayer, movementNb, &retry, currentPlayer, turn);

        mvprintw(13, 100, "                                                                                      ");
        mvprintw(14, 100, "                                                                                      ");

    } while (touch != 'a' && touch != 'e' && touch != 'q' && touch != 'd' && touch != 'w' && touch != 'c' || retry == 1); // While the user is pressing keys other than those expected


    mvprintw(15, 100, "Press L to confirm your deplacement or press k to remake it            ");
    refresh();
    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
}


void Score(Player *player, Tile *tile) {        // Add fishes collected by the player to his points
    player->score += tile->fish;
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
            if ((blockedCount % 4) == PenguinsPerPlayer(nbPlayer)){ // +1 to avoid /0 opération
                player[i].canPlay = 0;
            }
        }
    }
    if( blockedCount == nbPlayer * PenguinsPerPlayer(nbPlayer)){ // if the numbers of penguins blocked == the sum of every penguins of each player
        return 1;
    }
    else{
        return 0;
    }
}


void Winners(Player *player, int SIZE){
    int j = 0;
    int indwinner;
    int nbwinner = 0;
    Player win = player[0];

    Player* winners = malloc(SIZE * sizeof(Player));
    if(winners == NULL){
        exit(-1);
    }

    for(int i = 1; i < SIZE; i++){ //on cherche le score le + élevé
        if(win.score < player[i].score){
            win = player[i];
            indwinner = i; //on recupere l'indice du joueur au meilleur score pour pas le repeter dans le tableau des gagnants
        }
    }
    winners[j] = win; //on met le joueur dans le tableau des gagnants
    j++;
    nbwinner += 1;

    for(int i = 1; i < SIZE; i++){ // on verifie qu'il y a un seul gagnant et si il y en a plusieurs on les ajoutes au tableau de gagnant
        if(win.score == player[i].score && i != indwinner){
            winners[j] = player[i];
            nbwinner++;
        }
    }

    if(nbwinner == 1){    //on affiche le nom du ou des gagnants en cas d'égalité
        mvprintw(10, 110, "La gagnant est %c", winners[0].name);
    }
    else{
        mvprintw(10, 110, "Les gagnant est sont:");
        for(int i = 0; i < nbwinner; i++){
            mvprintw(10, 110, "%c\n", winners[i].name);
        }
    }
}

void Game(Tile **board, int* rematch) {                 // the main game function
    int touch;
    int turn = 0;
    int currentPlayer = 0;
    int nbPlayer;
    int selectedPenguinNb;
    int impossibleSelection;
    int nbPenguin;
    int disableL;
    int passK = 0;
    int returnMenu = 0;
    Player *player;

    HomePage();                             // shows the home page
    do {
        touch = getch();
        switch (touch) {
            case 'p':                       // Start a game
                break;
            case 27:                        // échap =  quit the game
            clear();
                exit(1);
                break;

            case KEY_RESIZE:                // If the window is resized by user, reprint the main menu
                HomePage();
                break;
        }
    } while (touch != 'p');               // While the user hasn't started a game

    clear();
    mvprintw(4, 2, "How many players? (between 2 and 6) press enter after you press the number");
    refresh();
    scanw("%d", &nbPlayer);

    player = createTabPlayers(board, nbPlayer);
    nbPenguin = PenguinsPerPlayer(nbPlayer);

    // PRINT LOOP :
    while (!isAllPlayerBlocked(board, player, nbPlayer) && !returnMenu) {          // stop conditions : see game rules

        if(player[currentPlayer].canPlay){                    // If a player cant play his turn will be skip
            // demander a l'utilisateur de press Enter pour commenceer son tour
            clear();
            showIceFloe(board, player, nbPlayer);

            mvprintw(2, 36, " tour : %d", turn);
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
                    case 'a': //a

                        if(player[currentPlayer].penguin[0].isMoveable == 1){
                            selectedPenguinNb = 0;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }

                        break;
                    case 'z': //z
                        if(player[currentPlayer].penguin[1].isMoveable == 1){
                            selectedPenguinNb = 1;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                    case 'e': //e
                        if(player[currentPlayer].penguin[2].isMoveable == 1){
                            selectedPenguinNb = 2;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                    case 'r': //r
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
                        for (int i = 0; i < nbPenguin; ++i) {
                            if(player[currentPlayer].penguin[i].isMoveable){
                                mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d", i + 1,
                                         player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                            }
                            else{
                                mvprintw(7 + i, 100, "penguins: %d  in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                                impossibleSelection = 1;
                            }
                        }

                        mvprintw(11, 100, "Choose Your Penguin");
                        break;

                    case 27: // échap =  quitter le jeu
                        returnMenu = 1;
                        break;
                }
            } while ((touch != 'a' && touch != 'z' && touch != 'e' && touch != 'r' || selectedPenguinNb > nbPenguin || impossibleSelection) && !returnMenu);


            mvprintw(12, 100, "You chose the %d penguin", selectedPenguinNb + 1);
            refresh();


            Penguin virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];
            board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 1;
            showIceFloe(board, player, nbPlayer); // Refresh the board to show the red selection tile

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



            disableL = 1;
            if(!returnMenu){
                touch = 'k';
                passK = 1;
            }


            if(!returnMenu){
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

                            board[virtualPenguin.tileY][virtualPenguin.tileX].isTherePlayer = 1;
                            board[virtualPenguin.tileY][virtualPenguin.tileX].penguinColor = currentPlayer + 1; // +1 parce que currentPlayer commence a 0 alors que le system de couleur commence a 1

                            break;

                        case 'k':

                            board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 0;
                            virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];
                            showIceFloe(board, player, nbPlayer);
                            deplacement(board, player, &virtualPenguin, touch, nbPlayer, currentPlayer, turn);
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
                                    mvprintw(7 + i, 100, "penguin %d :   in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY, player[currentPlayer].penguin[i].tileX);
                                }
                            }
                            mvprintw(11, 100, "Choose Your Penguin");
                            mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6");
                            mvprintw(15, 100, "Press L to confirm your deplacement or press k to remake it           ");
                            mvprintw(16, 100, "                                                ");
                            break;

                        case 27: // échap =  quitter le jeu
                            returnMenu = 1;
                            break;
                    }
                    passK = 0;

                } while ((touch != 'l' || disableL == 1) && !returnMenu);
            }
            }



        clear();
        player[currentPlayer].currentPenguins += (player[currentPlayer].currentPenguins + 1) % nbPenguin; // on boucle les pinguins de ce joueur
        currentPlayer = (currentPlayer + 1) % nbPlayer; // on boucle sur les joeueurs
        turn++;
    }


    clear();
    refresh();

    if(!returnMenu){
        Winners(player, nbPlayer-1);
    }

    mvprintw(10, 2, "Press 1 if you want to Rematch or press 2 if you want to leave the game, then press Enter");
    refresh();
    scanw("%d", &(*rematch));


    // Afficher le gagnant le score de chaque joueur et proposé de rematch quitteez revenir au menue ect...
}




int main() {
    int nbPlayer;
    int rematch = 0;


    Tile **board = NULL;
    Player *players = NULL;

    srand(time(NULL));                      // necessary for the rand() function to work proprely

    InitCurse();                            // initialising curses

    do{
        clear();
        refresh();
        board = createBoard();
        Game(board, &rematch);
    } while (rematch);


    //showScore(players);

    endwin();                               // close the window

    // freeing all the memory used
    free(players);
    free(board);

    return 0;

}