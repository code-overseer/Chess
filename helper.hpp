#ifndef helper_hpp
#define helper_hpp

#include <iostream>
#include <cstring>
#include <vector>
#include <array>

#define CHECKMATE 1
#define INVALID_MOVE 2
#define INVALID_INPUT 3
#define NO_PIECE_AT_POSITION 4
#define NOT_YOUR_TURN 5
#define ILLEGAL_MOVE 6
#define STALEMATE 7
#define NEXT_TURN 8
#define END_GAME 0

/* Team "class" */
enum Team {black, white};

/*
 Convert the chess positions into the integer format:
 Example:
 A1 -> 11;
 D4 -> 44;
 H7->87
 */
int pos_to_int(char const* pos);
/*
 Gets the corresponding row index for the positions array from integer format:
 Example:
 B1 -> 21 -> rIndex = 1-1=0;
 D4 -> 44 -> rIndex = 4-1=3;
 H7 -> 87 -> rIndex = 7-1=6;
 */
int rIndex(int pos);
/*
 Gets the corresponding row index for the positions array from chess positions
 Example:
 B1 -> 21 -> rIndex = 1-1=0;
 D4 -> 44 -> rIndex = 4-1=3;
 H7 -> 87 -> rIndex = 7-1=6;
 */
int rIndex(char const* position);
/*
 Gets the corresponding column index for the positions array from integer format
 Example:
 B1 -> 21 -> fIndex = 2-1=1;
 D4 -> 44 -> fIndex = 4-1=3;
 H7 -> 87 -> fIndex = 8-1=7;
 */
int fIndex(int pos);
/*
 Gets the corresponding column index for the positions array from chess positions
 Example:
 B1 -> 21 -> fIndex = 2-1=1;
 D4 -> 44 -> fIndex = 4-1=3;
 H7 -> 87 -> fIndex = 8-1=7;
 */
int fIndex(char const* position);
/*
 Gets the corresponding integer format for the array index input
 Example:
 B1 -> fIndex =1, rIndex =0 -> 21;
 D4 -> fIndex =3, rIndex =3 -> 44
 H7 -> fIndex =7, rIndex =6 -> 87
 */
int index_to_int(int file, int rank);

#endif /* helper_hpp */
