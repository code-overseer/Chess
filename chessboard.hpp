/*
 K: King
 Q: Queen
 B: Bishop
 N: Knight
 R: Rook
 P: Pawn
 */
#ifndef chessboard_hpp
#define chessboard_hpp

#include <iostream>
#include <cstring>
#include <array>
#include <vector>
#include "helper.hpp"

class Chesspiece;

class Chessboard {
private:
  bool black_check=0;
  bool white_check=0;
  bool ischeck(Team t);
  bool ischeckmate(Team t);
  bool iscastling(const char *origin, const char *target);
  bool isenpassant(const char *origin, const char *target);
  void promote_pawn(const char *target);
  int blackKing=pos_to_int("E8");
  int whiteKing=pos_to_int("E1");
  int check_causer=NULL; // Position of a piece that can attack the King
  int en_passant=NULL; // Position of a pawn that can be captured by en passant
public:
  std::array<std::array<Chesspiece*,8>,8> positions;
  Team turn=white; //(rand()%2? white : black);
  void move_piece(char const* org, char const* tgt);
  void display_board();
  Chessboard();
  ~Chessboard();
};

#endif /* chessboard_hpp */

