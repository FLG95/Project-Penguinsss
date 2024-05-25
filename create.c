#include "include/create.h"



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

    board = malloc(c * sizeof(Tile *));     // Tab 2D of Tiles
    if (!board) {                           // If board == NULL || NB : this notation will often be used
        exit(2);
    }

    for (int i = 0; i < l; ++i) {           // y <=> line
        board[i] = malloc(l * sizeof(Tile));    // Tab of Tiles <=> the line of the tab
        if (!board[i]) {
            exit(2);
        }
        for (int j = 0; j < c; ++j) {       // x <=> columns
            // Now we can generate Tile inside this tab of tab
            if (i % 2 == 0) {               // "Even" line
                board[i][j] = createTile(y + i * tileHeight - 1, x + j * tileWidth);
            } else {                        // "Odd" line
                board[i][j] = createTile(y + i * tileHeight - 1, x + 4 + j * tileWidth);
            }
            // We are doing this because we have to generate a hexagonal board
            // So one line in two have to be shifted slightly to the right
        }
        //y -= 1;                             // Reduce distance between tiles

    }
    return board;
}


int PenguinsPerPlayer(int n) {
    int nbPenguins =  0;
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
        case 5:                                 // 5 players = 1 penguins per player, no need to break twice if we have 5 or 6 the opérations is the same
            nbPenguins = 1;
        case 6:                                 // 6 players = 1 penguins per player
            nbPenguins = 1;
            break;

        default:                                // If we have a value not between 2 and 6 its bug the game should close
            exit(5);
    }
    return nbPenguins;
}


Penguin createPenguin(Tile **board, int color) {
    int randX, randY;
    Penguin penguin;

    if(!board){
        exit(1);
    }

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
    unsigned long length;           // Len of player name
    char name[100];
    Player *tabPlayers;                 // tab of players

    if(!board){
        exit(1);
    }

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
        do{
            mvprintw(5+i, 2, "Enter the name of the player %d :\n", i + 1); // Print this message in the window
            refresh();                                                      // We update the window, because otherwise we can't see the difference
            scanw("%s", name);
            length = strlen(name);
        }while(length == 0);

        // We use dynamic allocation for the player's name
        tabPlayers[i].name = malloc(length+1 * sizeof(char));// +1 to get the end line character
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
