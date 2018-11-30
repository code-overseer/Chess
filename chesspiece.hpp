#ifndef chesspiece_hpp
#define chesspiece_hpp

#include "helper.hpp"

class Chessboard;

class Chesspiece {
private:
  // Data member
  unsigned short num_at_position[8][8];

protected:
  virtual bool
  isvalid(char const* origin, char const* target, Chessboard* cb=nullptr)=0;
  // Data member
  bool first_move_made=0;
  
public:
  Chesspiece(Team t, char const* sym, char const* n);
  virtual ~Chesspiece();
  bool operator==(char const* name) const;
  bool operator!=(char const* name) const;
  friend std::ostream& operator <<(std::ostream&, Chesspiece&);
  friend std::ostream& operator >>(std::ostream&, Chesspiece&);
  friend Chessboard;
  // Data members
  Team team;
  char const* const symbol; // Unicode Chess Characters
  char const* const name;
};

class King : public Chesspiece {
public:
  King(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x94":"\xE2\x99\x9A", "King") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

class Queen : public Chesspiece {
public:
  Queen(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x95":"\xE2\x99\x9B", "Queen") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

class Bishop : public Chesspiece {
public:
  Bishop(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x97":"\xE2\x99\x9D", "Bishop") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

class Knight : public Chesspiece {
public:
  Knight(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x98":"\xE2\x99\x9E", "Knight") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

class Rook : public Chesspiece {
public:
  Rook(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x96":"\xE2\x99\x9C", "Rook") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

class Pawn : public Chesspiece {
public:
  Pawn(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x99":"\xE2\x99\x9F", "Pawn") {};
private:
  bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr) override;
};

std::ostream& operator <<(std::ostream& o, Chesspiece& cp);
std::ostream& operator >>(std::ostream& o, Chesspiece& cp);

#endif /* chesspiece_hpp */

