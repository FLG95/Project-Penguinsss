#include "include/display.h"


void colorPerPlayer(int currentPlayer) {     // At the beginning of the game, we give a penguin color to every player
    switch (currentPlayer) {
        // mvprintw is part of the ncurses library
        case 0: //Black
            mvprintw(6, 100, "You are playing the black penguin(s)");        // Print at the (y=6;x=100) coords the message
            break;
        case 1: // Blue
            mvprintw(6, 100, "You are playing the blue penguin(s)");
            break;
        case 2: // Magenta
            mvprintw(6, 100, "You are playing the magenta penguin(s)");
            break;
        case 3: // Red
            mvprintw(6, 100, "You are playing the red penguin(s)");
            break;
        case 4: // Yellow
            mvprintw(6, 100, "You are playing the yellow penguin(s)");
            break;
        case 5: // Green
            mvprintw(6, 100, "You are playing the green penguin(s)");
            break;
        default:              // if the player isn't in range we should quit the game it is a bug
            exit(5);
            break;
    }
}

void ColorPenguins(Tile tile, Player *player, int nbPlayer, int y, int x) {
    // This function displays each player's penguins with a different background to differentiate them from each other.
    if(!player){
        exit(1);
    }
    switch ((tile.penguinColor)) {
        // Each color corresponds to the colour of a player's penguins
        // init_pair, attron, mvprintw, refresh an attroff are part of the ncurses library
        // Case 1 is the only case commented because the code here is redundant
        case 1:
            init_pair(1, COLOR_WHITE, COLOR_BLACK);         // We initialise the window background with those colors and attribute them the number 1
            attron(COLOR_PAIR(1));                          // We start using the color 1
            mvprintw(y + 1, x + 2, "🐧");                   // We print the penguin
            refresh();                                      // We update the window, because otherwise we can't see the difference
            attroff(COLOR_PAIR(1));                         // We stop using the color 1
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


int showTile(Tile tile, Player *player, int nbPlayer) {     // Print a "tile" with the coordinates stored in the "Tile" parameter
    int x = tile.posX;
    int y = tile.posY;
    int color;
    if(!player){
        exit(1);
    }
    if (tile.isAlive == 0) {
        init_pair(27, COLOR_WHITE, COLOR_CYAN);

        /* We initialise the window background with those colors and attribute them the number 27.
        The ice floe is shown in cyan to represent the ocean after the ice pack has melted */

        attron(COLOR_PAIR(27));   // We start using those colors*

        mvprintw(y, x + 2, "   ");
        mvprintw(y + 1, x, "       "); // If tile not alive, then don't go further : no tile draw
        mvprintw(y + 2, x, "       ");
        mvprintw(y + 3, x + 2, "   ");

        attroff(COLOR_PAIR(27));    // We stop using those colors
        return 0;
    }

    // height =  4 ; width = 7

    if (tile.isRed == 1) {      //If the player is going to move to this square or the penguin he has selected is on this square, the square is shown in red
        color = 10;
    }
    else {
        color = 11;
    }

    init_pair(10, COLOR_WHITE, COLOR_RED); // We initialise the window background with those colors and attribute them the number 10
    init_pair(11, COLOR_WHITE, COLOR_WHITE); // We initialise the window background with those colors and attribute them the number 11


    attron(COLOR_PAIR(color));  // We start using those colors

    if (tile.isTherePlayer == 0) {      // if no penguins on tile
        // Can be replaced by a switch
        if (tile.fish == 1) {
            mvprintw(y, x + 2, "   ");
            mvprintw(y + 1, x, "   \U0001f41f  "); // The blue fish emoji is displayed thanks to its unicode character
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

        attroff(COLOR_PAIR(color));  // We stop using this color
        ColorPenguins(tile, player, nbPlayer, y, x); // The penguin is not displayed on the tile after the player has played
    }


    attroff(COLOR_PAIR(color));   // We stop using those colors

    return 1;
}


void showIceFloe(Tile **board, Player *player, int nbPlayer) {              // Shows the ice floe and score

    if(!player || !board){
        exit(1);
    }

    for (int i = 0; i < l; i++) { // Loop over the number of lines
        for (int j = 0; j < c; j++) { // Loop over the number of columns of rows
            showTile(board[i][j], player, nbPlayer); // We print the tile using the data stored in board
        }
    }

    // At each frame, the score for each player is updated
    mvprintw(5, 150, "Score : ");
    for (int i = 0; i < nbPlayer; ++i) {
        mvprintw(6 + i, 150, "%s : %d", player[i].name, player[i].score);
    }

    for (int i = 0; i < l; ++i) {
        mvprintw( startTilesTabY + i * tileHeight, 3, "%d", i+1);
    }

    for (int i = 0; i < c; ++i) {
        mvprintw( 3, startTilesTabX + 3 + i * tileWidth, "%d", i+1);
    }

    refresh();
}


void HomePage(){                    // This is the home page which is displayed when the game is launched

    int title_length = strlen("      CCCCCCCCCCCCCYYYYYYY       YYYYYYY     FFFFFFFFFFFFFFFFFFFFFFIIIIIIIIII   SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH");  //We retrieve the length of the longest line in the title
    int title_width = (getmaxx(stdscr) - title_length) / 2;  // We take the maximum width of the window and divide it by the maximum length of the title to be able to centre it later.


    init_pair(11, COLOR_MAGENTA, COLOR_BLACK);  // We initialise the window background with those colors and attribute them the number 10
    init_pair(12, COLOR_WHITE, COLOR_BLACK);   // We initialise the window background with those colors and attribute them the number 10

    attron(COLOR_PAIR(11));  // We start using those colors
    mvprintw(1, title_width, "       CCCCCCCCCCCCCYYYYYYY       YYYYYYY     FFFFFFFFFFFFFFFFFFFFFFIIIIIIIIII   SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH");  //Each time y is increased by 1 (i.e. the line), always in the same column, which is title length, and the title is displayed in magenta
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

    attroff(COLOR_PAIR((11)));  // We stop using those colors
    attron(COLOR_PAIR(12));


    // Repeat the same steps to display the menu
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