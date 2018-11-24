#ifndef chesspiece_hpp
#define chesspiece_hpp

#include <iostream>
#include <cstring>
#include <vector>
#include "helper.hpp"

class Chessboard;

class Chesspiece {
public:
  Chesspiece(Team t, char const* sym, char const* n) :
  team(t), symbol(sym), name(n) { }
  virtual ~Chesspiece();
  
  friend std::ostream& operator <<(std::ostream&, Chesspiece&);
  friend Chessboard;
  Team team;
  char const* const symbol;
  char const* const name;
protected:
  bool first_move_made=0;
  virtual bool
  isvalid(char const* origin, char const* target, Chessboard* cb=nullptr)=0;
};

class King : public Chesspiece {
public:
  King(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x94":"\xE2\x99\x9A", "King") { }
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

class Queen : public Chesspiece {
public:
  Queen(Team t) :
    Chesspiece(t, (t==white)?"\xE2\x99\x95":"\xE2\x99\x9B", "Queen") { }
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

class Bishop : public Chesspiece {
public:
  Bishop(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x97":"\xE2\x99\x9D", "Bishop") {};
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

class Knight : public Chesspiece {
public:
  Knight(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x98":"\xE2\x99\x9E", "Knight") {};
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

class Rook : public Chesspiece {
public:
  Rook(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x96":"\xE2\x99\x9C", "Rook") {};
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

class Pawn : public Chesspiece {
public:
  Pawn(Team t) :
  Chesspiece(t, (t==white)?"\xE2\x99\x99":"\xE2\x99\x9F", "Pawn") {};
private:
  virtual bool isvalid(char const* origin,
                       char const* target, Chessboard* cb=nullptr);
};

std::ostream& operator <<(std::ostream& o, Chesspiece& cp);

#endif /* chesspiece_hpp */

