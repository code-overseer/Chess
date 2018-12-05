#include "Knight.hpp"
#include "ChessBoard.h"

bool Knight::isvalid(int origin, int target, ChessBoard const* cb) const {
  if (!intCheck(target)||origin==target) return false;
  
  if (abs(target-origin)==move::E+2*move::S||
      abs(target-origin)==move::E+2*move::N||
      abs(target-origin)==2*move::E+move::S||
      abs(target-origin)==2*move::E+move::N) {
    if (!cb->positions[rIndex(target)][fIndex(target)]||
        cb->positions[rIndex(target)][fIndex(target)]->team!=team) {
      return true;
    }
  }
  return false;
}
