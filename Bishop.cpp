#include "Bishop.hpp"

bool Bishop::isvalid(int origin, int target, ChessBoard const* cb) const {
  if (!intCheck(target)||origin==target) return false;
  
  int direction=isDiagonal(origin, target);
  
  if (!direction) return false;
  
  if (pathCheck(origin, target, direction, cb)) return true;
  return false;
}
