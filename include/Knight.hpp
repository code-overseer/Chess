#ifndef Knight_hpp
#define Knight_hpp

#include "ChessPiece.hpp"

/* Knight class */
class Knight : public Chesspiece {
public:
  Knight(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x98":"\xE2\x99\x9E", "Knight") {};
private:
  /* Overriden function for Knight moves;
   L-shaped movement with 8 possible movements */
  bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Knight_hpp */
