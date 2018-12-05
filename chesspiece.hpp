#ifndef chesspiece_hpp
#define chesspiece_hpp

#include "helper.hpp"

class ChessBoard;
/* Abstract Chesspiece class */
class Chesspiece {
protected:
  /*
   isvalid(int origin, int target, ChessBoard* cb)
   Checks if the move from origin to target is valid, i.e. not blocked,
   target is empty or has an enemy piece
   returns true if the move is valid
   
   Note: this does not check for legality i.e. whether the move will put the
   piece's king in check
   */
  virtual bool isvalid(int origin, int target, ChessBoard* cb)=0;
  // Data member
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

/* King class */
class King : public Chesspiece {
public:
  King(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x94":"\xE2\x99\x9A", "King") {};
private:
  /* Overriden function for King moves;
   single file, rank or diagonal movement */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};
/* Queen class */
class Queen : public Chesspiece {
public:
  Queen(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x95":"\xE2\x99\x9B", "Queen") {};
private:
  /* Overriden function for Queen moves;
   across files, ranks or diagonals if not blocked */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};
/* Bishop class */
class Bishop : public Chesspiece {
public:
  Bishop(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x97":"\xE2\x99\x9D", "Bishop") {};
private:
  /* Overriden function for Bishop moves;
   across diagonals only if not blocked */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};
/* Knight class */
class Knight : public Chesspiece {
public:
  Knight(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x98":"\xE2\x99\x9E", "Knight") {};
private:
  /* Overriden function for Knight moves;
   L-shaped movement with 8 possible movements */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};
/* Rook class */
class Rook : public Chesspiece {
public:
  Rook(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x96":"\xE2\x99\x9C", "Rook") {};
private:
  /* Overriden function for Rook moves;
   across files, ranks if not blocked */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};
/* Pawn class */
class Pawn : public Chesspiece {
public:
  Pawn(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x99":"\xE2\x99\x9F", "Pawn") {};
private:
  /* Overriden function for Pawn moves;
   single rank increase only if not the first move
   double rank increase is possible for the first move
   captures diagonally in a single position forward
   */
  bool isvalid(int origin, int target, ChessBoard* cb=nullptr) override;
};

std::ostream& operator <<(std::ostream& o, Chesspiece& cp);
std::ostream& operator >>(std::ostream& o, Chesspiece& cp);

#endif /* chesspiece_hpp */

