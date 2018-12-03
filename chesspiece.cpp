#include "chesspiece.hpp"
#include "ChessBoard.h"

using namespace std;

Chesspiece::Chesspiece(Team t, char const* sym, char const* n) :
team(t), symbol(sym), name(n) {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      num_at_position[j][i]=0;
    }
  }
}

Chesspiece::~Chesspiece() {
  return;
}

bool King::isvalid(int origin, int target, ChessBoard* cb) {
  // Check if the input is valid; A=10, 1=1 => A1 = 11; H=80, 8=8 => H8 = 88
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (abs(target-origin)==9||abs(target-origin)==11||
      abs(target-origin)==1||abs(target-origin)==10) {
    if (!cb->positions[target%10-1][target/10-1]) {
      return true;
    } else if (cb->positions[target%10-1][target/10-1]->team!=team) {
      return true;
    }
  }
  
  return false;
}

bool Queen::isvalid(int origin, int target, ChessBoard* cb) {
  int path=0;
  int dir=0;
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (!(abs(target-origin)%9)) {
    dir=9;  // rank++ file-- or rank-- file++
  } else if (!(abs(target-origin)%11)) {
    dir=11; // file++ rank++ pr file-- rank--
  } else if (!(abs(target/10-origin/10))) {
    dir=1;  // rank++ or rank--
  } else if (!(abs(target-origin)%10)) {
    dir=10; // file++ or file--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(target-origin)/dir; i++) {
    path = origin + i*dir*(target-origin<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==target) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Bishop::isvalid(int origin, int target, ChessBoard* cb) {
  int path=0;
  int dir=0;
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (!(abs(target-origin)%9)) {
    dir=9;  // rank++ file-- or rank-- file++
  } else if (!(abs(target-origin)%11)) {
    dir=11; // file++ rank++ pr file-- rank--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(target-origin)/dir; i++) {
    path = origin + i*dir*(target-origin<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==target) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Knight::isvalid(int origin, int target, ChessBoard* cb) {
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (abs(target-origin)==8||abs(target-origin)==12||
      abs(target-origin)==19||abs(target-origin)==21) {
    if (!cb->positions[target%10-1][target/10-1]) {
      return true;
    } else if (cb->positions[target%10-1][target/10-1]->team!=team) {
      return true;
    }
  }
  return false;
}

bool Rook::isvalid(int origin, int target, ChessBoard* cb) {
  int path=0;
  int dir=0;
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (!(abs(target/10-origin/10))) {
    dir=1;  // rank++ or rank--
  } else if (!(abs(target-origin)%10)) {
    dir=10; // file++ or file--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(target-origin)/dir; i++) {
    path = origin + i*dir*(target-origin<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==target) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Pawn::isvalid(int origin, int target, ChessBoard* cb) {
  if (target/10<1 || target/10>8 || target%10<1 || target%10>8) return false;
  
  if (target-origin==(team ? 2:-2)) {
    if (!first_move_made) {
      if (!cb->positions[rIndex(target)][fIndex(target)] &&
          !cb->positions[rIndex(origin+(team ? 1:-1))]
            [fIndex(origin+(team ? 1:-1))]) {
          return true;
      }
    }
    return false;
  }
  
  if (target-origin==(team ? 1:-1)) {
    if (!cb->positions[rIndex(target)][fIndex(target)]) {
      return true;
    }
    return false;
  }
  
  if ((target-origin==(team ? 11:9)) || (target-origin==(team ? -9:-11))) {
    if (!cb->positions[rIndex(target)][fIndex(target)]) {
      return false;
    } else if (cb->positions[rIndex(target)][fIndex(target)]->team!=team) {
      return true;
    }
  }
  return false;
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
