#include "chessboard.hpp"
#include "errors.h"
#include <iostream>

using namespace std;

int main() {
  
  cout<<"A new chess game is started!"<<endl<<endl;
  Chessboard* board = new Chessboard;
  board->display_board();
  char piece[3];
  char target[3];
  while (true) {
    try {
      cin>>piece>>target;
      board->move_piece(piece, target);
      board->display_board();
    } catch (int invalid) {
      if (!invalid) {board->display_board();return NO_ERROR;}
      continue;
    }
  }

}
