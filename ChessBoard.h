#ifndef ChessBoard_hpp
#define ChessBoard_hpp

#include "helper.hpp"
#include "ChessPiece.hpp"

int const THREEFOLD = 3;
int const FIFTY = 50;

class ChessBoard {
private:
  /*
   ischeck(Team t)
   * Function to check if Team t is in check
   
   * returns an integer value, if 't' is in check returns the piece attacking
   the king and 0 otherwise
   */
  int isCheck(Team t) const;

  /*
   isGameOver(Team t)
   * Function to check if Team t is in checkmate or the game is in stalemate
   by checking if Team t has any more legal moves
   * returns a boolean value, true if the game is over and false otherwise
   */
  bool isGameOver(Team t);
  
  /*
   checkCastling(const char *origin, const char *target, Team t)
   * Function to check if Team t castling from position origin to target is possible
   * Checks through all the requirements:
   * 1. The king and the chosen rook are on the player's first rank.[3]
   * 2. Neither the king nor the chosen rook has previously moved.
   * 3. There are no pieces between the king and the chosen rook.
   * 4. The king is not currently in check.
   * 5. The king does not pass through a square that is attacked by an
   enemy piece.
   * 6. The king does not end up in check. (True of any legal move.)
   
   * Returns a boolean value, true if the move is possible and false otherwise
   
   * Note: This function inherently checks for legality
   */
  bool isCastling(int origin, int target, Team t) const;

  /*
   isEnpassant(const char *origin, const char *target, Team t);
   * Function to check if the en passant capture by moving a Team t pawn from
   origin to target is possible
   * Checks through all the requirements:
   * 1. The capturing pawn must be on its fifth rank
   * 2. The captured pawn must be on an adjacent file and must have just
   moved two squares in a single move (i.e. a double-step move)
   * 3. The capture can only be made on the move immediately after the opposing
   pawn makes the double-step move; otherwise, the right to capture it en
   passant is lost
   
   * Returns a boolean value, true if the move is possible and false otherwise
   
   * Note: This function does NOT change the positions of the pieces,
   it is only a check
   */
  bool isEnpassant(int origin, int target, Team t) const;
  
  /*
   canAttack(char const* target, Team t, bool pawn)
   * Function to check if ANY piece in Team 't' can capture a target position
   regardless of legality (putting own king in check)
   * This function checks for capture not movement, so pawn diagonals
   are checked
   * Returns an integer value representing the position of the first attacker
   found, 0 otherwise
   */
  int canAttack(int target, Team t) const;
  
  /*
   isLegal(int origin, int target, Team t, bool alwaysUndo=false)
   * Function to check if a piece in Team 't' at origin can move to/attack a
   target position legally
   * This is done by moving the piece to the target and checking for legality
   with the isCheck function
   * alwaysUndo is a flag; set to true if any move made regardless of legality
   is to be undone, set to false if only illegal moves is to be undone
   
   * Returns an integer value; the origin position is returned if the
   attack/movement is legal and 0 otherwise
   */
  int isLegal(int origin, int target, Team t, bool alwaysUndo=false);
  
  /*
   isLegal(int target, Team t)
   * Overloaded function to check if ANY piece in Team 't' can move to/attack a
   target position legally
   * This is done by looping isLegal(int origin, int target, Team t, true) for
   every piece in Team t
   
   * Returns an integer value; the origin position is returned if the attack
   is legal and possible and 0 otherwise
   
   * Note: This function undos any movement after checking for legality
   */
  int isLegal(int target, Team t);
  
  /*
   submitMoveExceptions(int status, char const* origin, char const* target);
   * Function used to handle exceptions in the submitMove member function
   * Contains a switch tested with the int value status to return various
   statements to the standard error and some to the standard output
   
   * Returns a integer value corresponding to the exception raised:
   
   * INVALID_MOVE 2: The piece cannot move to the position either due to a block
   or the piece cannot move that way
   * NOT_MOVING: Origin and target the same
   * INVALID_INPUT 4: Invalid position input detected
   * NO_PIECE_AT_POSITION 5: Origin position input refers to an empty position
   * NOT_YOUR_TURN 6: Piece from the wrong team is chosen
   * ILLEGAL_MOVE 7: The move made resulted in putting one's self in check
   * STALEMATE 8: Stalemate detected, no legal moves available
   * CHECKMATE 1: Checkmate
   */
  int submitMoveExceptions(int status, char const *origin, char const *target)
  const;
  
  /*
   undo(char const* origin, char const* target, Team t, bool isEnpassant)
   * Function used to undo the move made if an illegal move has been detected
   * It moves the piece from the target back to the origin and retrieves the
   previous piece that was at the target from the captured_piece data member
   * Team t is used to specify the team that made the move to be undone
   * isEnpassant is a boolean flag used to deal with the undoing of an en
   passant move where the capture position does not equal the resulting position
   
   * No return value
   */
  void undo(int origin, int target, Team t, bool isEnpassant);
  /*
   promote_pawn(const char *target)
   
   * Function that promotes the pawn located at target
   * Only called when a pawn has reached its 8th rank
   * Deletes the existing pawn piece and replaces it with promoted piece
   * It will ask for a character input to select the pieces to promote to
   
   * No return value
   */
  void promotePawn(char const* target);
  /*
   setupPieces()
   
   * Function used to setup the pieces to the default position
   * Called when initialising or resetting the board
   
   * No return value
   */
  void setupPieces();
  /*
   messageOutput(char const *origin, char const* target, bool capture,
   bool enpassant, bool castling);
   
   * Function used to output a message statement to the standard output when a
   successful move has been made
   * The origin and target are used to report the move while the boolean values
   are flags that details the move type
   * capture: normal capture
   * enpassant: en passant capture
   * castling: castling move
   
   * No return value
   */
  void messageOutput(char const *origin, char const *target,
                     bool capture, bool enpassant, bool castling) const;
  
  /*
   drawNotification(int num, int flag);
   
   * Function used to notify the players a draw is possible
   * Only called when fifty or more moves have been made without a capture or a
   piece moves to the same position 3 or more times
   
   * flag is used to select which message to output, FIFTY will output the
   message for the fifty-move rule while THREEFOLD would output the Threefold
   repetiton rule message
   
   * No return value
   */
  void drawNotification(int num, int flag) const;
  /*
   pos_to_int(char const* pos) const;
   Convert the chess positions into the integer format:
   Example:
   A1 -> 11;
   D4 -> 44;
   H7-> 87
   */
  int pos_to_int(char const* pos) const;
  /*
   Gets the corresponding row (rank) index for the positions array from chess
   positions
   Example:
   B1 -> 21 -> rIndexChar("B1") = 1-1=0;
   D4 -> 44 -> rIndexChar("D1") = 4-1=3;
   H7 -> 87 -> rIndexChar("H7") = 7-1=6;
   */
  int rIndexChar(char const* position) const;
  /*
   Gets the corresponding column (file) index for the positions array from chess
   positions
   Example:
   B1 -> 21 -> fIndexChar("B1") = 2-1=1;
   D4 -> 44 -> fIndexChar("D4") = 4-1=3;
   H7 -> 87 -> fIndexChar("H7") = 8-1=7;
   */
  int fIndexChar(char const* position) const;
  /*
   Gets the corresponding integer format for the array index input
   Example:
   B1 -> fIndexChar("B1")=1, rIndexChar("B1")=0 -> index_to_int(1,0)=21;
   D4 -> fIndexChar("D4")=3, rIndexChar("D1")=3 -> index_to_int(3,3)=44;
   H7 -> fIndexChar("H7")=7, rIndexChar("H7")=6 -> index_to_int(7,6)=87;
   */
  int index_to_int(int file, int rank) const;
  
  /* Data members */
  /* Number of turns since last capture */
  int turns_since_last_capture=0;
  /* Position of the black king as an integer */
  int blackKing=pos_to_int("E8");
  /* Position of the white king as an integer */
  int whiteKing=pos_to_int("E1");
  /* Temporarily stores captured piece */
  Chesspiece* captured_piece=nullptr;
  /* Position of a piece that can attack the King, 0 means no piece */
  int checker=0;
  /* Position of a pawn that can be captured by en passant */
  int epPawn=0;
  /* Whose turn is it */
  Team turn=white;
public:
  /*
   submitMove(char const* org, char const* tgt)
   * Called by the main function to make a move from the origin to target
   * This is one of the only function that accepts a character array input of
   positions
   
   * Returns integer values:
   INVALID_MOVE 2
   NOT_MOVING 3
   INVALID_INPUT 4
   NO_PIECE_AT_POSITION 5
   NOT_YOUR_TURN 6
   ILLEGAL_MOVE 7
   NEXT_TURN 9
   END_GAME 0
   
   */
  int submitMove(char const* org, char const* tgt);
  
  /*
   display_board()
   * Displays the board using Unicode-8 symbols
   */
  void displayBoard();
  
  /*
   resetBoard()
   * resets the board, by clearing all the positions before calling the
   setupPieces function
   */
  void resetBoard();
  ChessBoard();
  ~ChessBoard();
  
  // Data members
  /*
   * An array of chesspiece pointers, points to nullptr if the position is
   empty and points to a piece otherwise
   * Rank is the first dimension and file is the second i.e.
   positions[rank][file]
   */
  std::array<std::array<Chesspiece*,8>,8> positions;
};

#endif /* chessboard_hpp */

