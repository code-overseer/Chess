#ifndef King_hpp
#define King_hpp

#include "Chesspiece.hpp"

/* King class */
class King : public Chesspiece {
public:
    explicit King(Team t) :
            Chesspiece(t, (t==white)?"\xE2\x99\x94":"\xE2\x99\x9A", "King") {};
private:
    /* Overriden function for King moves;
     single file, rank or diagonal movement */
    bool isvalid(int origin, int target, ChessBoard const* cb) const override;
};

#endif /* King_hpp */
