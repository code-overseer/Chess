#include "ChessBoard.h"
#include <iostream>

using namespace std;

int main() {
  cout<<"A new chess game is started!"<<endl<<endl;
  ChessBoard* board = new ChessBoard;
  
  board->display_board();
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
      board->display_board();
    } catch (int invalid) {
      if (invalid==END_GAME) {
        board->display_board();
        delete board;
        return 0;
      }
      continue;
    }
  }

}
