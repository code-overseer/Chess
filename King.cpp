#include "King.hpp"
#include "ChessBoard.h"

bool King::isvalid(int origin, int target, ChessBoard const* cb) const {
  // Check if the input is valid; A=10, 1=1 => A1 = 11; H=80, 8=8 => H8 = 88
  if (!intCheck(target)||origin==target) return false;
  
  // abs value used to check for both negative and positive directions
  if (abs(target-origin)==move::SE||abs(target-origin)==move::NE||
      abs(target-origin)==move::N||abs(target-origin)==move::E) {
    if (!cb->positions[rIndex(target)][fIndex(target)]||
        cb->positions[rIndex(target)][fIndex(target)]->team!=team) {
      return true;
    }
  }
  return false;
}
