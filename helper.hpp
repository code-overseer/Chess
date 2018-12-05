#ifndef helper_hpp
#define helper_hpp

#include <iostream>
#include <cstring>
#include <array>

#define CHECKMATE 1
#define INVALID_MOVE 2
#define NOT_MOVING 3
#define INVALID_INPUT 4
#define NO_PIECE_AT_POSITION 5
#define NOT_YOUR_TURN 6
#define ILLEGAL_MOVE 7
#define STALEMATE 8
#define NEXT_TURN 9
#define END_GAME 0

/*
 Used by both the chessboard and chesspieces
 Gets the corresponding row (rank) index for the positions array from integer f
 ormat:
 Example:
 B1 -> 21 -> rIndex(21) = 1 - 1=0;
 D4 -> 44 -> rIndex(44) = 4 - 1=3;
 H7 -> 87 -> rIndex(87) = 7 - 1=6;
 */
int rIndex(int pos);
/*
 Used by both the chessboard and chesspieces
 Gets the corresponding column (file) index for the positions array from integer
 format
 Example:
 B1 -> 21 -> fIndex(21) = 2 - 1=1;
 D4 -> 44 -> fIndex(44) = 4 - 1=3;
 H7 -> 87 -> fIndex(87) = 8 - 1=7;
 */
int fIndex(int pos);

#endif /* helper_hpp */
