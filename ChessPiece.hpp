#ifndef chesspiece_hpp
#define chesspiece_hpp

#include "helper.hpp"

/* Team */
enum Team {black, white};

class ChessBoard;
/* Abstract Chesspiece class */
/* This class only deal with positions as integers, e.g.
 B5 will be processed as the integer 25
 H6 will be processed as the integer 86
 Both the first and second digit should be between 1 and 8 only;
 As such the movements of the piece are characterised as addition/subtraction,
 A1 -> A2 increase in rank (North) is a +1 operation
 A2 -> A1 decrease in rank (South) is -1
 A1 -> B1 increase in file (East) is +10
 B1 -> A1 decrease in file (West) is -10
 A1 -> B2 Northeast direction is +11
 B2 -> A1 Southwest direction is -11
 B1 -> A2 Northwest direction is -9
 A2 -> B1 Southeast direction is +9
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

class Chesspiece {
  protected:
  /*
   inputCheck(int input);
   
   A function used to validate the input position in integer format;
   Both rank and file must be between 1 and 8;
   
   Returns true if the format is correct and false otherwise
   */
  bool intCheck(int input) const;
  /*
   pathCheck(int origin, int target, int direction);
   A function to check if the path from the origin to the target is clear of
   other pieces
   
   Returns true if it is clear and false otherwise
   */
  bool pathCheck(int origin, int target, int direction,
                 ChessBoard const* cb) const;
  /*
   isDiagonal(int origin, int target);
   A function to check if the path from the origin to the target is a diagonal
   move
   
   Returns an integer value; the direction NE, NW, SE, NW if true
   and 0 otherwise
   */
  int isDiagonal(int origin, int target) const;
  /*
   isStraightOn(int origin, int target);
   A function to check if the path from the origin to the target is a straight
   move (i.e. rook)
   
   Returns an integer value; the direction N, S, E, W if true
   and 0 otherwise
   */
  int isStraightOn(int origin, int target) const;;
  
  /*
   isvalid(int origin, int target, ChessBoard* cb)
   Checks if the move from origin to target is valid, i.e. not blocked,
   target is empty or has an enemy piece
   returns true if the move is valid
   
   Note: this does not check for legality i.e. whether the move will put the
   piece's king in check
   */
  virtual bool isvalid(int origin, int target, ChessBoard const* cb) const=0;
  
  // Data members
  /* Boolean value, true if first move has been made */
  bool first_move_made=0;
  /* An array containing the number of times a piece has been at a
   particular position */
  unsigned short num_at_position[8][8];
  /* Unicode Chess Characters */
  char const* const symbol;
  /* Piece name */
  char const* const name;
  
public:
  /* Constructor */
  Chesspiece(Team t, char const* sym, char const* n);
  /* Destructor */
  virtual ~Chesspiece();
  /* '==' Operator overload to compare the name of the piece to another
   character array */
  bool operator==(char const* name) const;
  /* '!=' Operator overload to compare the name of the piece to another
   character array */
  bool operator!=(char const* name) const;
  /* cout<<Operator overload to write the unicode symbol of the piece to the
   ostream */
  friend std::ostream& operator <<(std::ostream&, Chesspiece&);
  /* cout<<Operator overload to write the name of the piece to the
   ostream */
  friend std::ostream& operator >>(std::ostream&, Chesspiece&);
  friend ChessBoard;
  // Data members
  /* Piece colour, black or white */
  Team team;
};

std::ostream& operator <<(std::ostream& o, Chesspiece& cp);
std::ostream& operator >>(std::ostream& o, Chesspiece& cp);

#endif /* chesspiece_hpp */

