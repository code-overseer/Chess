#ifndef chesspiece_hpp
#define chesspiece_hpp

#include "ChessBoard.hpp"

/* Abstract Chesspiece class */

class Chesspiece {
  protected:
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
  bool first_move_made = false;
  /* An array containing the number of times a piece has been at a
   particular position */
  unsigned short num_at_position[8][8]{};
  /* Unicode Chess Characters */
  char const* const symbol;
  /* Piece name */
  char const* const name;
  /*Function pointer to ChessBoard::rIndex to retrieve the rank index from the
   integer format*/
  int (*rIndex)(int) = &ChessBoard::rIndex;
  /*Function pointer to ChessBoard::fIndex to retrieve the file index from the
   integer format*/
  int (*fIndex)(int) = &ChessBoard::fIndex;
  
  /*Function pointer to ChessBoard::intCheck to check the validity of the
   integer format*/
  bool (*intCheck)(int) = &ChessBoard::intCheck;
  
public:
  /* Constructor */
  Chesspiece(Team t, char const* sym, char const* n);
  /* Destructor */
  virtual ~Chesspiece();
  /* '==' Operator overload to compare the name of the piece to another
   character array */
  bool operator==(char const* str) const;
  /* '!=' Operator overload to compare the name of the piece to another
   character array */
  bool operator!=(char const* str) const;
  /* cout<<Operator overload to write the unicode symbol of the piece to the
   ostream */
  friend std::ostream& operator <<(std::ostream&, Chesspiece&);
  /* cout<<Operator overload to write the name of the piece to the
   ostream */
  friend std::ostream& operator >>(std::ostream&, Chesspiece&);
  friend ChessBoard;
  // Data members
  /* Piece colour, black or white, subclasses need access to this */
  Team team;
};

std::ostream& operator <<(std::ostream& o, Chesspiece& cp);
std::ostream& operator >>(std::ostream& o, Chesspiece& cp);

#endif /* chesspiece_hpp */

