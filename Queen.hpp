#ifndef Queen_hpp
#define Queen_hpp

#include "ChessPiece.hpp"

/* Queen class */
class Queen : public Chesspiece {
public:
  Queen(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x95":"\xE2\x99\x9B", "Queen") {};
private:
  /* Overriden function for Queen moves;
   across files, ranks or diagonals if not blocked */
  bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Queen_hpp */
