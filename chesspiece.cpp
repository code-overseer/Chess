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

bool King::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (abs(tgt-org)==9||abs(tgt-org)==11||
      abs(tgt-org)==1||abs(tgt-org)==10) {
    if (!cb->positions[tgt%10-1][tgt/10-1]) {
      return true;
    } else if (cb->positions[tgt%10-1][tgt/10-1]->team!=team) {
      return true;
    }
  }
  
  return false;
}

bool Queen::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  int path=0;
  int dir=0;
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (!(abs(tgt-org)%9)) {
    dir=9;  // rank++ file-- or rank-- file++
  } else if (!(abs(tgt-org)%11)) {
    dir=11; // file++ rank++ pr file-- rank--
  } else if (!(abs(tgt/10-org/10))) {
    dir=1;  // rank++ or rank--
  } else if (!(abs(tgt-org)%10)) {
    dir=10; // file++ or file--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(tgt-org)/dir; i++) {
    path = org + i*dir*(tgt-org<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==tgt) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Bishop::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  int path=0;
  int dir=0;
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (!(abs(tgt-org)%9)) {
    dir=9;  // rank++ file-- or rank-- file++
  } else if (!(abs(tgt-org)%11)) {
    dir=11; // file++ rank++ pr file-- rank--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(tgt-org)/dir; i++) {
    path = org + i*dir*(tgt-org<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==tgt) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Knight::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (abs(tgt-org)==8||abs(tgt-org)==12||
      abs(tgt-org)==19||abs(tgt-org)==21) {
    if (!cb->positions[tgt%10-1][tgt/10-1]) {
      return true;
    } else if (cb->positions[tgt%10-1][tgt/10-1]->team!=team) {
      return true;
    }
  }
  return false;
}

bool Rook::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  int path=0;
  int dir=0;
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (!(abs(tgt/10-org/10))) {
    dir=1;  // rank++ or rank--
  } else if (!(abs(tgt-org)%10)) {
    dir=10; // file++ or file--
  } else {
    return false;
  }
  
  for (int i=1; i<=abs(tgt-org)/dir; i++) {
    path = org + i*dir*(tgt-org<0?-1:1);
    if (path/10<1 || path/10>8 || path%10<1 || path%10>8) break;
    if (path==tgt) {
      if (!cb->positions[path%10-1][path/10-1]) return true;
      if (cb->positions[path%10-1][path/10-1]->team!=team) return true;
      break;
    }
    if (!!cb->positions[path%10-1][path/10-1]) break;
  }
  return false;
}

bool Pawn::isvalid(char const* origin, char const* target, ChessBoard* cb) {
  int tgt = pos_to_int(target);
  int org = pos_to_int(origin);
  if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8) return false;
  
  if (tgt-org==(team ? 2:-2)) {
    if (!first_move_made) {
      if (!cb->positions[rank_index(tgt)][file_index(tgt)] &&
          !cb->positions[rank_index(org+(team ? 1:-1))]
            [file_index(org+(team ? 1:-1))]) {
          return true;
      }
    }
    return false;
  }
  
  if (tgt-org==(team ? 1:-1)) {
    if (!cb->positions[rank_index(tgt)][file_index(tgt)]) {
      return true;
    }
    return false;
  }
  
  if ((tgt-org==(team ? 11:9)) || (tgt-org==(team ? -9:-11))) {
    if (!cb->positions[rank_index(tgt)][file_index(tgt)]) {
      return false;
    } else if (cb->positions[rank_index(tgt)][file_index(tgt)]->team!=team) {
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
