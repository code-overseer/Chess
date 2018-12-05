#ifndef Pawn_hpp
#define Pawn_hpp

#include "ChessPiece.hpp"

/* Pawn class */
class Pawn : public Chesspiece {
public:
  Pawn(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x99":"\xE2\x99\x9F", "Pawn") {};
private:
  /* Overriden function for Pawn moves;
   single rank increase only if not the first move
   double rank increase is possible for the first move
   captures diagonally in a single position forward
   */
  bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Pawn_hpp */
