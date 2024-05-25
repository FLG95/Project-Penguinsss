#include "include/display.h"


int tileDontExist(int y, int x) {       // If tile don't exist, return 0. Else, return 1

    if (x > c-1 || x < 0 || y > l-1 || y < 0) {
        return 1;
    } else {
        return 0;
    }
}

void movePreview(Tile **board, Player *player, Penguin *virtualPenguin, int touch, int nbPlayer, int movementNb, int* retry, int currentPlayer, int turn) {
    /*
    This function handle movements in a player's turn
    All verifications are done before moving a player


    Keybindings :
    /!\ in AZERTY /!\ (cocorico)
    "a" = top corner left
    "e" = top corner right
    "q" = left
    "d" = right
    "w" = bottom corner left
    "c" = bottom corner right
    */

    int n = 0;                                          // n is a variable for moving from an iceberg to another
    int isBlocked = 0;                                  // Can the penguin move ? (If the next tile is either not on the board, dead or taken by another penguin)
    int initialPosX = virtualPenguin->tileX;            // Save the original x position of the virtual penguin
    int initialPosY = virtualPenguin->tileY;            // Save the original y position of the virtual penguin

    if(!board || !player || !virtualPenguin || !retry){ // We check if the pointers are null if there are we leave the game
        exit(1);
    }

    switch (touch) {
        // In this switch, we use a "virtual penguin" to check if movements wanted by user are possible, and move the "true penguin" if possible

        case 'a': // Before moving, we check if movement is possible (COM3)

            // While n < move value inputted by user (n will increment at each loop, and we'll check if the user can actually make the move or not)
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
                else{
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
                        if (tileDontExist(virtualPenguin->tileY - 1, virtualPenguin->tileX) ||
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
                }

                n++;
            }

            break;

        case 'e': // (COM3)

            while(n < movementNb && !isBlocked){

                if (virtualPenguin->tileY % 2 == 0) {       // handling even lines

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
                    else{
                        if (virtualPenguin->tileY % 2 == 0) { // Handling even lines

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
                    if (virtualPenguin->tileY % 2 == 0) { // handling even lines
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
                    else { // Odd lipne
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

        case KEY_RESIZE:                         // If the user resize the window, we update the window
            showIceFloe(board, player, nbPlayer);
            mvprintw(2, 36, " Tour : %d", turn);
            mvprintw(5, 100, "Player %d : %s", currentPlayer+1, player[currentPlayer].name); // Debug only
            colorPerPlayer(currentPlayer); // Tell the current player the color of his penguin(s)
            for (int i = 0; i < PenguinsPerPlayer(nbPlayer); ++i) {
                if(player[currentPlayer].penguin[i].isMoveable){
                    mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d", i + 1,
                             player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                }
                else{
                    mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                }
            }
            mvprintw(13, 100, "Enter the number of movement you want to do ( between 1 and 6 )");
            mvprintw(14, 100, "You will move of  %d tile(s)", movementNb);
            mvprintw(15, 100, "Now move your penguin.");
            refresh();
            *retry = 1;
            break;

        default :
            *retry = 1;
            break;
    }

    showIceFloe(board, player, nbPlayer);
    if(isBlocked == 1){                         // if the penguin is blocked (ie the movement wanted by user is impossible)
        *retry = 1;                             // the user will have to choose another movement
        mvprintw(15, 100, "This move isn't valid \U0001f614 please choose an other move.");   // "\U0001f614" = sad emoji
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
        virtualPenguin->tileY = initialPosY;
        virtualPenguin->tileX = initialPosX;
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
    }
}

void movePenguin(Tile **board, Player *player, Penguin *virtualPenguin, int nbPlayer, int currentPlayer, int selectedPenguinNb, int turn) {

    int nbPenguin = PenguinsPerPlayer(nbPlayer);
    int touch;              // Input from user
    if(!board || !player || !virtualPenguin){
        exit(1);
    }

    mvprintw(15, 100, "                                                                                   ");
    refresh();
    int movementNb;
    int retry = 0;           // Did the user messed up input (pressed the wrong key)

    do {
        board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 1;
        showIceFloe(board, player, nbPlayer);
        if(retry == 1){                             // If the movement requested by user is impossible
            mvprintw(16, 100, "Please press 1 ");   // We tell the user to press k to retry another movement
            refresh();
            do{
                touch = getch();
            }while(touch != '1');
            mvprintw(15, 100, "                                                                          ");
            mvprintw(16, 100, "                                                 ");
            refresh();
            board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
        }

        do{
            mvprintw(13, 100, "Enter the number of movement you want to do ( between 1 and 6 )");
            refresh();
            touch = getch();
            switch(touch){
                case '1':
                    movementNb = 1;
                    break;

                case '2':
                    movementNb = 2;
                    break;

                case '3':
                    movementNb = 3;
                    break;

                case '4':
                    movementNb = 4;
                    break;

                case '5':
                    movementNb = 5;
                    break;

                case '6':
                    movementNb = 6;
                    break;

                case KEY_RESIZE:
                    showIceFloe(board, player, nbPlayer);
                    mvprintw(2, 36, " Tour : %d", turn);
                    mvprintw(5, 100, "Player %d : %s", currentPlayer+1, player[currentPlayer].name); // Debug only
                    colorPerPlayer(currentPlayer); // Tell the current player the color of his penguin(s)
                    for (int i = 0; i < nbPenguin; ++i) {
                        if(player[currentPlayer].penguin[i].isMoveable){
                            mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d", i + 1,
                                     player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                        }
                        else{
                            mvprintw(7 + i, 100, "Penguin %d :   in y: %d, x: %d NOT MOVEABLE", i + 1,
                                     player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                        }
                    }
                    mvprintw(11, 100, "Press the number of the penguin you want to play");
                    mvprintw(12, 100, "You chose the penguin %d", selectedPenguinNb+1);
                    mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6.");
                    refresh();
                    break;

                default:
                    break;
            }
        }while( touch != '1' && touch != '2' && touch != '3' && touch != '4' && touch != '5' && touch != '6');

        retry = 0;
        mvprintw(14, 100, "You will move of  %d tile(s)", movementNb);
        mvprintw(15, 100, "Now move your penguin");
        refresh();
        touch = getch();
        movePreview(board, player, virtualPenguin, touch, nbPlayer, movementNb, &retry, currentPlayer, turn);
        mvprintw(13, 100, "                                                                                      ");
        mvprintw(14, 100, "                                                                                      ");
        refresh();

    } while (touch != 'a' && touch != 'e' && touch != 'q' && touch != 'd' && touch != 'w' && touch != 'c' || retry == 1); // While the user is pressing keys other than those expected

    mvprintw(15, 100, "Press 1 to confirm your move or press 2 to remake it            ");
    refresh();
    board[virtualPenguin->tileY][virtualPenguin->tileX].isRed = 0;
}


void Score(Player *player, Tile *tile) {        // Add fishes collected by the player to his points
    player->score += tile->fish;
}

int simple(Tile** board, Penguin penguin, int y, int x) { // To make the code more readable, we have separated the conditions into several functions
    if(!board){
        exit(1);
    }
    if (tileDontExist(penguin.tileY + y, penguin.tileX + x) ||
        board[penguin.tileY + y][penguin.tileX + x].isAlive == 0 ||
        board[penguin.tileY + y][penguin.tileX + x].isTherePlayer == 1) {
        return 1;
    } else {
        return 0;
    }
}


int isPenguinMoveable(Tile** board, Penguin penguin){

    if(!board){
        exit(1);
    }

    if (penguin.tileY % 2 == 0) { // Conditions on even-numbered lines

        if (simple(board, penguin, -1, 0) && simple(board, penguin, -1, +1) && simple(board, penguin, 0, -1) &&
            simple(board, penguin, 0, +1) && simple(board, penguin, +1, 0) && simple(board, penguin, -1, +1)) { // if penguin can't move
            return 0;
        }
        else{
            return 1;
        }

    } else { // Conditions on odd-numbered lines
        if (simple(board, penguin, -1, -1) && simple(board, penguin, -1, 0) && simple(board, penguin, 0, -1) &&
            simple(board, penguin, 0, +1) && simple(board, penguin, +1, -1) && simple(board, penguin, -1, 0)) { // if penguin can't move
            return 0;
        }
        else{
            return 1;
        }
    }
}


int isAllPlayerBlocked(Tile** board, Player* player, int nbPlayer){         // Reminder : if all players can't move, the game end
    if(!board || !player){
        exit(1);
    }
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
    if( blockedCount == nbPlayer * PenguinsPerPlayer(nbPlayer)){ // If the numbers of penguins blocked == the sum of every penguin of each player
        return 1;
    }
    else{
        return 0;
    }
}


void Winners(Player *player, int SIZE){
    if(!player){
        exit(1);
    }
    int j = 0;
    int nbWinner = 0;
    Player win = player[0];

    Player* winners = malloc(SIZE * sizeof(Player));
    if(winners == NULL){
        exit(-1);
    }

    for(int i = 1; i < SIZE; i++){ // We're looking for the highest score
        if(win.score < player[i].score){
            win = player[i];
        }
        winners[j] = win; // We put the player in the winners' table
        j++;
        nbWinner += 1;

        for(int k = 1; k < SIZE; k++){ // We check that there is only one winner and if there is more than one, we add them all to the winners' table
            winners[j] = player[k];
            nbWinner++;
        }
    }

    if(nbWinner == 1){    // The name(s) of the winner(s) are displayed
        mvprintw(10, 110, "The winner is %s", winners[0].name);
    }
    else{
        mvprintw(10, 110, "The winners are : ");
        for(int i = 0; i < nbWinner; i++){
            mvprintw(10, 110, "%s\n", winners[i].name);
        }
    }
}

void Game(Tile **board, int* rematch) {                 // The main game function
    if(!board || rematch==NULL){
        exit(1);
    }
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
    HomePage();                             // Shows the home page
    do {
        touch = getch();
        switch (touch) {
            case 'p':                       // Start a game
                break;
            case 27:                        // Escape =  quit the game
                clear();
                endwin();
                exit(1);
                break;

            case KEY_RESIZE:                // If the window is resized by user, reprint the main menu
                HomePage();
                break;

            default:
                break;
        }
    } while (touch != 'p');               // While the user hasn't started a game
    clear();

    do{
        mvprintw(4, 2, "How many players? (between 2 and 6)");
        refresh();
        touch = getch();
        switch (touch) {
            case '2':
                nbPlayer = 2;
                break;

            case '3':
                nbPlayer = 3;
                break;

            case '4':

                nbPlayer = 4;
                break;

            case '5':

                nbPlayer = 5;
                break;

            case '6':
                nbPlayer = 6;
                break;

            case KEY_RESIZE:
                mvprintw(4, 2, "How many players? (between 2 and 6)");
                break;

            case 27:
                returnMenu = 1;
                break;

        }
    }while(touch != '2' && touch != '3' && touch != '4' && touch != '5' && touch != '6');

    player = createTabPlayers(board, nbPlayer);
    nbPenguin = PenguinsPerPlayer(nbPlayer);


    // PRINT LOOP :
    while (!isAllPlayerBlocked(board, player, nbPlayer) && !returnMenu) {          // Stop conditions : see game rules

        if(player[currentPlayer].canPlay){                    // If a player cant play his turn will be skipped
            clear();
            showIceFloe(board, player, nbPlayer);
            mvprintw(2, 36, " Turn : %d", turn);
            mvprintw(5, 100, "Player %d : %s", currentPlayer+1, player[currentPlayer].name); // Debug only
            colorPerPlayer(currentPlayer); // Tell the current player the color of his penguin(s)
            for (int i = 0; i < nbPenguin; ++i) {
                if(player[currentPlayer].penguin[i].isMoveable){
                    mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d", i + 1,
                             player[currentPlayer].penguin[i].tileY + 1, player[currentPlayer].penguin[i].tileX +1);
                }
                else{
                    mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d NOT MOVEABLE", i + 1, player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);  //  If the pinguin is blocked and can no longer move, "NOT MOVEABLE" is displayed next to it
                }
            }

            mvprintw(11, 100, "Press the number of the penguin you want to play");
            do { // We wait for the user to press the good key for the selection of his penguins
                touch = getch();
                switch (touch) {
                    case '1':
                        if(player[currentPlayer].penguin[0].isMoveable == 1){
                            selectedPenguinNb = 0;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                        break;

                    case '2':
                        if(player[currentPlayer].penguin[1].isMoveable == 1){
                            selectedPenguinNb = 1;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                        break;

                    case '3':
                        if(player[currentPlayer].penguin[2].isMoveable == 1){
                            selectedPenguinNb = 2;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                        break;

                    case '4':
                        if(player[currentPlayer].penguin[3].isMoveable == 1){
                            selectedPenguinNb = 3;
                            impossibleSelection = 0;
                        }
                        else{
                            impossibleSelection = 1;
                        }
                        break;

                    case KEY_RESIZE:
                        showIceFloe(board, player, nbPlayer);
                        mvprintw(2, 36, " Tour : %d", turn);
                        mvprintw(5, 100, "Player %d : %s", currentPlayer+1, player[currentPlayer].name); // Debug only
                        colorPerPlayer(currentPlayer); // Tell the current player the color of his penguin(s)
                        for (int i = 0; i < nbPenguin; ++i) {
                            if(player[currentPlayer].penguin[i].isMoveable){
                                mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d", i + 1,
                                         player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                            }
                            else{
                                mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d NOT MOVEABLE", i + 1,
                                         player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                                impossibleSelection = 1;
                            }
                        }
                        mvprintw(11, 100, "Press the number of the penguin you want to play");
                        refresh();
                        break;

                    case 27: // échap =  quit the game
                        returnMenu = 1;
                        break;

                    default:
                        break;
                }
            } while ((touch != '1' && touch != '2' && touch != '3' && touch != '4' || selectedPenguinNb > nbPenguin || impossibleSelection) && !returnMenu);
            mvprintw(12, 100, "You chose the penguin %d", selectedPenguinNb+1);
            refresh();
            Penguin virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];
            board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 1;
            showIceFloe(board, player, nbPlayer); // Refresh the board to show the red selection tile

            disableL = 1;

            if(!returnMenu){
                touch = '2';
                passK = 1;
                do {
                    if(passK != 1){
                        touch = getch();
                    }
                    switch (touch) {
                        case '1':
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

                        case '2':

                            board[virtualPenguin.tileY][virtualPenguin.tileX].isRed = 0;
                            virtualPenguin = player[currentPlayer].penguin[selectedPenguinNb];
                            showIceFloe(board, player, nbPlayer);
                            movePenguin(board, player, &virtualPenguin, nbPlayer, currentPlayer, selectedPenguinNb, turn);
                            disableL = 0;
                            break;

                        case KEY_RESIZE:
                            showIceFloe(board, player, nbPlayer);
                            mvprintw(2, 36, " Tour : %d", turn);
                            mvprintw(5, 100, "Player %d : %s", currentPlayer+1, player[currentPlayer].name); // Debug only
                            colorPerPlayer(currentPlayer); // Tell the current player the color of his penguin(s)
                            for (int i = 0; i < nbPenguin; ++i) {
                                if(player[currentPlayer].penguin[i].isMoveable){
                                    mvprintw(7 + i, 100, "Penguin %d : in y: %d, x: %d", i + 1,
                                             player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                                }
                                else{
                                    mvprintw(7 + i, 100, "Penguin %d :   in y: %d, x: %d NOT MOVEABLE", i + 1,
                                             player[currentPlayer].penguin[i].tileY+1, player[currentPlayer].penguin[i].tileX+1);
                                }
                            }
                            mvprintw(11, 100, "Press the number of the penguin you want to play");
                            mvprintw(12, 100, "You chose the penguin %d", selectedPenguinNb+1);
                            mvprintw(13, 100, "Enter the number of movement you want to do. Between 1 and 6.");
                            mvprintw(15, 100, "Press 1 to confirm your move or press 2 to remake it           "); // The "     " are here to write of some other line
                            mvprintw(16, 100, "                                                ");
                            refresh();
                            break;

                        case 27: // échap =  quit the game
                            returnMenu = 1;
                            break;

                        default:
                            break;
                    }
                    passK = 0;

                } while ((touch != '1' || disableL == 1) && !returnMenu);
            }
        }

        clear();
        player[currentPlayer].currentPenguins += (player[currentPlayer].currentPenguins + 1) % nbPenguin; // Loop over the pinguins of this player
        currentPlayer = (currentPlayer + 1) % nbPlayer; // Loop over the players
        turn++;
    }
    clear();
    refresh();

    if(!returnMenu){ // If we don't want to return to the before the end of the gale we show the winner
        Winners(player, nbPlayer-1);
    }

    do{
        mvprintw(10, 2, "Press 1 if you want to Rematch or press 2 if you want to Leave the game, then press Enter");
        refresh();
        scanw("%d", &(*rematch));
    }while(*rematch !=1 && *rematch !=2);
}


void InitCurse() {                  // curses initialization for our window
    // All the functions are part of the curses library
    setlocale(LC_ALL, "");          // Check if the shell is in "UTF-8"
    initscr();                      // Initialise the window
    start_color();                  // Starting colors
    cbreak();                       // Configure the terminal to make user input more fluid in the terminal
    noecho();                       // Avoid the user input to be displayed at the screen
    keypad(stdscr, true);
    curs_set(0);                    // Deactivate the mouse cursor
    refresh();                      // Refresh the window so all we did here is saved
}


int main() {
    int nbPlayer;
    int rematch = 0;
    Tile **board = NULL;
    Player *players = NULL;
    srand(time(NULL));                      // Necessary for the rand() function to work proprely
    InitCurse();                            // Initialising curses

    do{
        clear();
        refresh();
        board = createBoard();
        Game(board, &rematch);
    } while (rematch == 1);

    
    endwin();                               // Close the window and disable ncurses

    // Freeing all the memory used
    free(players);
    free(board);
    return 0;
}
