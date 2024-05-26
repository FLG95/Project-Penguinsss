<h1 align='center'>
  🐧🐧 Penguinsss Project 🐧🐧
</h1>

<!---![LINUX](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white) [![Licence](https://img.shields.io/github/license/Ileriayo/markdown-badges?style=for-the-badge)](./LICENSE)-->

<p align='center'>
  <a>
    <img alt="LINUX" src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black">
    <img alt="C" src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">  
    <img alt="Licence : MIT" src="https://img.shields.io/github/license/Ileriayo/markdown-badges?style=for-the-badge">   
  </a>&nbsp;&nbsp;
</p>

<p align='center'>
  A revolutionnary 2-6 players game about penguins, fishes and icerbegs.<br>
  Try to collect as many fish as you can ! 
</p>

## Pictures
![Game menu](https://i.imgur.com/dFwgshV.png)
![In-game picture](https://i.imgur.com/yvEfVcp.png)

## Installation and running

> [!CAUTION]
> The game is only available on Linux at the moment.

> [!WARNING]
> The `ncurses` library is needed for the game to work.

1. Clone the repository :
```
git clone https://github.com/FLG95/Project-Pinguinsss.git
```

2. Open your terminal where you cloned the repository :

    - (**OPTIONAL** : if you are a teacher that wants to check if our MakeFile is correct) :

        Start the MakeFile :
        ```
        make
        ```

4. Launch the game :
```
.\NFS_Penguins
```
4. Enjoy ! 🐧

## How to play ?

> [!IMPORTANT]
> Make sure your keyboard is set to AZERTY before playing.

- Start the game. Yeah, thanks you, Captain Obvious.

- Press `P` to start a game, or `ESC` to quit.

- You need between 2 and 6 players to play the game.

- The game will ask for each players's username.

The game share pinguins between players based on this chart :
| Number of players | Number of pinguins per player |
| :-------- | :------- |
| 2 | 4 |
| 3 | 3 |
| 4 | 2 |
| 5 | 1 |
| 6 | 1 |

- The game will then display the grid, consisting of icebergs with fishes on them, and penguins on icebergs. 

- All penguins will spawn randomly on the grid.

You can now play by following these rules :
- The player who collects the most fish with his penguins wins the game.
- **Turn-by-turn**, each player will have to make a move, by choosing a direction **for one** of his penguins, and by choosing a distance (1 or more icebergs, admitting there is no penguins on an iceberg between start and destination). You can preview your move before doing it. 
- An iceberg can only be occupied **by one penguin at a time**. A penguin **cannot cross** an iceberg occupied by another penguin.
- Due to global warming, when a penguin leaves an iceberg, the latter will melt. You can't go on a melted iceberg.
- Players can collect fishes by moving on an iceberg with fishes. Depending on the number of fishes on this iceberg, the player's score will change.
- If a player cannot move any of their penguins on the grid (i.e stuck because all icebergs around them is melted), their turn is skipped until the end of the game.
- The game ends when all players are stuck.

Everything else is explained in game.

| Keybind (AZERTY) | Action |
| :-------- | :------- |
| `A` | Move your penguin the **top left** iceberg |
| `E` | Move your penguin to the **top right** iceberg |
| `Q` | Move your penguin to the **left** iceberg |
| `D` | Move your penguin to the **right** iceberg |
| `W` | Move your penguin to the **down left** iceberg |
| `C` | Move your penguin to the **down right** iceberg |
| `1` | When asked for, select your **first** penguin |
| `2` | When asked for, select your **second** penguin |
| `3` | When asked for, select your **third** penguin |
| `4` | When asked for, select your **fourth** penguin |
| `ESC` | Quit the game at any moment |

At the end of the game, the name of the winner and the scores of each player will be shown.

You'll have the choice to either quit the game or start another one. Press `P` to restart a game, or `ESC` to quit the game.

## System Requirements

To assure the best performance in-game, your system should follow the following **minimal** configuration :
| Type | Minimal configuration     |
| :-------- | :------- |
| OS | Linux |
| CPU | Intel Xeon Platinum 8280L |
| GPU | Nvidia Titan RTX 24GB GDDR6 |
| RAM | 528 GB |
| Storage | 2,5 To |
| Bandwidth | 10GBPs |


## Critics

IGN :
> *A breathtakingly realistic game that challenges the current video games industry*
> 
> 10/10

jeuxvideo.com :
> *Extraordinaire et puissant chef d'oeuvre, une scénario digne d'un film de Tarantino et des graphismes époustouflants*
> 
> 9,99999/10

GameSpot :
> *If the word "gaming" was a game, it would defenitly be Pinguinsss Project*
>
> 11/10


## Creators

Made with ❤️ and 🐧 in Cergy-Pontoise, France.

<a href="https://github.com/FLG95/Project-Pinguinsss/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=FLG95/Project-Pinguinsss" />
</a>
<br><br>
Absolutly no penguins were harmed in the making of this game.

<!---
Credits :

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/licenses/)
[![AGPL License](https://img.shields.io/badge/license-AGPL-blue.svg)](http://www.gnu.org/licenses/agpl-3.0)



https://github.com/Naereen/badges
https://github.com/alexandresanlim/Badges4-README.md-Profile/
https://contrib.rocks/preview?repo=FLG95%2FProject-Pinguinsss
https://readme.so/fr/editor

