#ifndef helper_hpp
#define helper_hpp

#include <iostream>
#include <cstring>
#include <vector>
#include <array>

#define INVALID_MOVE 2
#define INVALID_INPUT 3
#define NO_PIECE_AT_POSITION 4
#define NOT_YOUR_TURN 5
#define ILLEGAL_MOVE 6
#define STALEMATE 7
#define CHECKMATE 1
#define END_GAME 0
// Max number of files
int const MAX_FILE=8;
// Max number of ranks
int const MAX_RANK=8;

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
 B1 -> 21 -> rank_index = 1-1=0;
 D4 -> 44 -> rank_index = 4-1=3;
 H7 -> 87 -> rank_index = 7-1=6;
 */
int rank_index(int pos);
/*
 Gets the corresponding row index for the positions array from chess positions
 Example:
 B1 -> 21 -> rank_index = 1-1=0;
 D4 -> 44 -> rank_index = 4-1=3;
 H7 -> 87 -> rank_index = 7-1=6;
 */
int rank_index(char const* position);
/*
 Gets the corresponding column index for the positions array from integer format
 Example:
 B1 -> 21 -> file_index = 2-1=1;
 D4 -> 44 -> file_index = 4-1=3;
 H7 -> 87 -> file_index = 8-1=7;
 */
int file_index(int pos);
/*
 Gets the corresponding column index for the positions array from chess positions
 Example:
 B1 -> 21 -> file_index = 2-1=1;
 D4 -> 44 -> file_index = 4-1=3;
 H7 -> 87 -> file_index = 8-1=7;
 */
int file_index(char const* position);
/*
 Gets the corresponding integer format for the array index input
 Example:
 B1 -> file_index =1, rank_index =0 -> 21;
 D4 -> file_index =3, rank_index =3 -> 44
 H7 -> file_index =7, rank_index =6 -> 87
 */
int index_to_int(int file, int rank);

#endif /* helper_hpp */
