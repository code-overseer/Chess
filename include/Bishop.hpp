#ifndef Bishop_hpp
#define Bishop_hpp

#include "Chesspiece.hpp"

/* Bishop class */
class Bishop : public Chesspiece {
public:
    explicit Bishop(Team t) :
            Chesspiece(t, (t==white)?"\xE2\x99\x97":"\xE2\x99\x9D", "Bishop") {};
private:
    /* Overriden function for Bishop moves;
     across diagonals only if not blocked */
    bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* Bishop_hpp */
