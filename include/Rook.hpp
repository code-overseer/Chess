#ifndef Rook_hpp
#define Rook_hpp

#include "Chesspiece.hpp"

/* Rook class */
class Rook : public Chesspiece {
public:
    explicit Rook(Team t) :
            Chesspiece(t, (t==white)?"\xE2\x99\x96":"\xE2\x99\x9C", "Rook") {};
private:
    /* Overriden function for Rook moves;
     across files, ranks if not blocked */
    bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Rook_hpp */
