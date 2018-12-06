#ifndef chesspiece_hpp
#define chesspiece_hpp

#include "helper.hpp"

class ChessBoard;
/* Abstract Chesspiece class */

class Chesspiece {
  protected:
  /*
   inputCheck(int input);
   
   A function used to validate the input position in integer format;
   Both rank and file must be between 1 and 8;
   
   Returns true if the format is correct and false otherwise
   */
  static bool intCheck(int input);
  /*
   Used by both the chessboard and chesspieces
   Gets the corresponding row (rank) index for the positions array from integer f
   ormat:
   Example:
   B1 -> 21 -> rIndex(21) = 1 - 1=0;
   D4 -> 44 -> rIndex(44) = 4 - 1=3;
   H7 -> 87 -> rIndex(87) = 7 - 1=6;
   */
  static int rIndex(int pos);
  /*
   Used by both the chessboard and chesspieces
   Gets the corresponding column (file) index for the positions array from integer
   format
   Example:
   B1 -> 21 -> fIndex(21) = 2 - 1=1;
   D4 -> 44 -> fIndex(44) = 4 - 1=3;
   H7 -> 87 -> fIndex(87) = 8 - 1=7;
   */
  static int fIndex(int pos);
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

