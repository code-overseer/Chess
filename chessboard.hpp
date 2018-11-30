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

#include "helper.hpp"

int const THREEFOLD = 3;
int const FIFTY = 50;

class Chesspiece;

class Chessboard {
private:
  bool ischeck(Team t);
  bool ischeckmate(Team t);
  bool checkCastling(const char *origin, const char *target);
  bool checkEnpassant(const char *origin, const char *target);
  bool checkStalemate();
  int canAttack(char const* target, Team t);
  int submitMove_exceptions(int status, char const* origin, char const* target);
  void undo(char const* origin, char const* target, Team t, bool isEnpassant);
  void promote_pawn(const char *target);
  void setupPieces();
  void messageOutput(char const *origin, char const* target,
                     bool capture, bool enpassant, bool castling);
  void ask_for_draw(int num, int flag);
  
  /* Data members */
  int moves_since_last_capture=0;
  int illegalMoves=0;
  bool black_check=0;
  bool white_check=0;
  int blackKing=pos_to_int("E8");
  int whiteKing=pos_to_int("E1");
    // Temporarily stores captured piece
  Chesspiece* captured_piece=nullptr;
    // Position of a piece that can attack the King
  int check_causer=NULL;
    // Position of a pawn that can be captured by en passant
  int en_passant=NULL;
public:
  int submitMove(char const* org, char const* tgt);
  void display_board();
  void resetBoard();
  Chessboard();
  ~Chessboard();
  
  // Data members
  std::array<std::array<Chesspiece*,MAX_FILE>,MAX_RANK> positions;
  Team turn=white;
};

#endif /* chessboard_hpp */

