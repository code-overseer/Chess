#include "ChessBoard.hpp"
#include <iostream>

using namespace std;

int main() {
    auto board = new ChessBoard();

    board->displayBoard();
    char piece[3];
    char target[3];
    while (true) {
        try {
            cin>>piece;
            cin>>target;
            toupper(piece[0]);
            toupper(target[0]);
            if (board->submitMove(piece, target)==END_GAME)
                throw END_GAME;
            board->displayBoard();
        } catch (int invalid) {
            if (invalid==END_GAME) {
                board->displayBoard();
                delete board;
                return 0;
            }
            continue;
        }
    }

}
