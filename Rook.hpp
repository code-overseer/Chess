//
//  Rook.hpp
//  Chess
//
//  Created by Bryan Wong on 05/12/2018.
//  Copyright © 2018 Bryan Wong. All rights reserved.
//

#ifndef Rook_hpp
#define Rook_hpp

#include "ChessPiece.hpp"

/* Rook class */
class Rook : public Chesspiece {
public:
  Rook(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x96":"\xE2\x99\x9C", "Rook") {};
private:
  /* Overriden function for Rook moves;
   across files, ranks if not blocked */
  bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Rook_hpp */
