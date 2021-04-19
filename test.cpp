#include <bits/stdc++.h>
#include "config.h"
#include "custom_bot.h"

int board[HEIGHT][WIDTH];

int main() {

    Gomoku gomoku_bot;

    for (int row = 0; row < HEIGHT; ++ row) {
        for (int col = 0; col < WIDTH; ++ col) {
            board[row][col] = 0;
        }
    }
    
    board[14][25] = 1;
    board[15][25] = 1;
    board[16][24] = 1;
    board[16][25] = -1;
    board[16][26] = 1;
    board[17][24] = -1;
    board[18][22] = 1;
    board[18][23] = -1;  
    board[19][22] = -1;  

    gomoku_bot.initBoard(board);
    std::cout << std::setprecision(5) << std::fixed;
    std::cout << gomoku_bot.boardVal(-1) << std::endl;

    return 0;
}