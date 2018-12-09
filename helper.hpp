#ifndef helper_hpp
#define helper_hpp

#include <iostream>
#include <cstring>
#include <array>

#define END_GAME 0
#define NEXT_TURN 1
#define INVALID_MOVE 2
#define NOT_MOVING 3
#define INVALID_INPUT 4
#define NO_PIECE_AT_POSITION 5
#define NOT_YOUR_TURN 6
#define ILLEGAL_MOVE 7

/* Team */
enum Team {black, white};

/* The ChessBoard and Chesspiece class only deal with positions as integers, e.g.
 B5 will be processed as the integer 14
 H6 will be processed as the integer 75
 Both the first and second digit should be between 0 and 7 only;
 As such the movements of the piece are characterised as addition/subtraction,
 A1 -> A2 increase in rank "North" is a +1 operation
 A2 -> A1 decrease in rank "South" is -1
 A1 -> B1 increase in file "East" is +10
 B1 -> A1 decrease in file "West" is -10
 A1 -> B2 "Northeast" direction is +11
 B2 -> A1 "Southwest" direction is -11
 B1 -> A2 "Northwest" direction is -9
 A2 -> B1 "Southeast" direction is +9
 */
namespace move {
  int const N=1;
  int const S=-1;
  int const E=10;
  int const W=-10;
  int const NE=11;
  int const NW=-9;
  int const SE=9;
  int const SW=-11;
}

#endif /* helper_hpp */
