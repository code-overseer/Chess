#include "Pawn.hpp"
#include "ChessBoard.h"

bool Pawn::isvalid(int origin, int target, ChessBoard const* cb) const {
  if (!intCheck(target)||origin==target) return false;
  // White can only move north, black can only move south
  if (target-origin==(team ? 2*move::N:2*move::S)) {
    if (!first_move_made) {
      if (!cb->positions[rIndex(target)][fIndex(target)] &&
          !cb->positions[rIndex(origin+(team ? move::N:move::S))]
          [fIndex(origin+(team ? move::N:move::S))]) {
        return true;
      }
    }
    return false;
  }
  
  if (target-origin==(team ? move::N:move::S)) {
    if (!cb->positions[rIndex(target)][fIndex(target)]) return true;
    return false;
  }
  
  if ((target-origin==(team ? move::NE:move::SE))||
      (target-origin==(team ? move::NW:move::SW))) {
    if (!cb->positions[rIndex(target)][fIndex(target)]) {
      return false;
    } else if (cb->positions[rIndex(target)][fIndex(target)]->team!=team) {
      return true;
    }
  }
  return false;
}
