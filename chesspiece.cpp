#include "ChessBoard.h"
#include "ChessPiece.hpp"

using namespace std;

Chesspiece::Chesspiece(Team t, char const* sym, char const* n) :
team(t), symbol(sym), name(n) {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      num_at_position[i][j]=0;
    }
  }
}

Chesspiece::~Chesspiece() {
  return;
}

bool Chesspiece::intCheck(int input) const {
  if (input/10<1 || input/10>8 || input%10<1 || input%10>8) return false;
  return true;
}

bool Chesspiece::pathCheck(int origin, int target, int direction,
                           ChessBoard const* cb) const {
  int path =0;
  for (int i=1; i<=(target-origin)/direction; i++) {
    path = origin + i*direction;
    if (!intCheck(path)) break;
    if (path==target) {
      if (!cb->positions[rIndex(path)][fIndex(path)]||
          cb->positions[rIndex(path)][fIndex(path)]->team!=team) return true;
      break;
    }
    if (cb->positions[rIndex(path)][fIndex(path)]) break;
  }
  return false;
}

int Chesspiece::isDiagonal(int origin, int target) const {
  // failsafe
  if (origin == target) return 0;
  
  if (!(abs(target-origin)%move::SE)) {
    // rank++ file-- or rank-- file++
    return (target-origin)<0?move::NW : move::SE;
  } else if (!(abs(target-origin)%move::NE)) {
    // file++ rank++ pr file-- rank--
    return (target-origin)<0?move::SW : move::NE;
  } else {
    return 0;
  }
}

int Chesspiece::isStraightOn(int origin, int target) const {
  // failsafe
  if (origin == target) return 0;
  
  if (rIndex(target)==rIndex(origin)) {
    // file++ or file--
    return (target-origin)<0?move::W : move::E;
  }
  if (fIndex(target)==fIndex(origin)) {
    // rank++ or rank--
    return (target-origin)<0?move::S : move::N;
  } else {
    return 0;
  }
}

std::ostream& operator <<(std::ostream& o, Chesspiece& cp) {
  return o << cp.symbol;
}

std::ostream& operator >>(std::ostream& o, Chesspiece& cp) {
  return o << cp.name;
}

bool Chesspiece::operator==(char const* name) const {
  return (!strcmp(this->name,name));
}

bool Chesspiece::operator!=(char const* name) const {
  return !(!strcmp(this->name,name));
}
