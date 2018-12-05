#ifndef ChessBoard_hpp
#define ChessBoard_hpp

#include "helper.hpp"
#include "chesspiece.hpp"

int const THREEFOLD = 3;
int const FIFTY = 50;

class ChessBoard {
private:
  /*
   ischeck(Team t)
   * Function to check if Team t is in check
   * t_king is a hypothetical king position (defaulted to 0 which will set
   the actual King position within the function)
   * returns a boolean value, true if 't' is in check and false otherwise
   */
  int ischeck(Team t);
  /*
   ischeckmate(Team t)
   * Function to check if Team t is in checkmate
   * returns a boolean value, true if 't' is in checkmate and false otherwise
   */
  bool ischeckmate(Team t);
  /*
   checkCastling(const char *origin, const char *target)
   * Function to check if castling from position origin to target is possible
   * Checks through all the requirements:
   * 1. The king and the chosen rook are on the player's first rank.[3]
   * 2. Neither the king nor the chosen rook has previously moved.
   * 3. There are no pieces between the king and the chosen rook.
   * 4. The king is not currently in check.
   * 5. The king does not pass through a square that is attacked by an
   enemy piece.
   * 6. The king does not end up in check. (True of any legal move.)
   
   * Returns a boolean value, true if the move is possible and false otherwise
   
   * Note: This check inherently checks for legality
   */
  bool checkCastling(int origin, int target) const;
  /*
   checkEnpassant(const char *origin, const char *target);
   * Function to check if the en passant capture by moving a pawn from
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
  bool checkEnpassant(int origin, int target);
  /*
   checkStalemate();
   * Function to check if the board is at a stalemate
   * This is called when too many illegal moves have been detected in a row
   when the player is not in check
   * First by checking if the King is stuck in place
   * followed by checking every other piece for any other legal moves
   * This function will aslo set the en_passant data member to 0
   (no en_passant piece)
   
   * Returns a boolean value, true if it is a stalemate and false otherwise
   */
  bool checkStalemate();
  /*
   canAttack(char const* target, Team t)
   * Function to check if ANY piece in Team 't' can *attack* a target position
   regardless of legality (putting own king in check)
   
   * Returns a integer value, if the attack is legal position of the attacker is
   returned and 0 otherwise
   
   * Note: This function does NOT change the positions of the pieces,
   it is only a check
   */
  int canAttack(int target, Team t) const;
  /*
   isLegal(int origin, int target, bool alwaysUndo=false)
   * Function to check if a piece at origin  can attack a target
   position legally
   * This function moves the pieces into position before checking if it is legal
   * It then undos the move made
   * alwaysUndo flag is defaulted to false, set to true, to undo any moves made;
   false flag only undos illegal moves
   
   * Returns a integer value, if the attack is legal position of the attacker is
   returned and 0 otherwise
   
   */
  int isLegal(int origin, int target);
  /*
   isLegal(int target, Team t)
   * Function to check if ANY piece in Team 't' can attack a target position
   legally
   * This function moves the pieces into position before checking if it is legal
   * It then undos the move made
   
   * Returns a integer value, if the attack is legal position of the attacker is
   returned and 0 otherwise
   
   */
  int legalAttack(int target, Team t);
  /*
   submitMove_exceptions(int status, char const* origin, char const* target);
   * Function used to handle exceptions in the submitMove member function
   * Contains a switch tested with the int value status to return various
   statements to the standard error and some to the standard output
   
   * Returns a integer value corresponding to the exception raised:
   
   * INVALID_MOVE 2: The piece cannot move to the position either due to a block
   or the piece cannot move that way
   * INVALID_INPUT 3: Invalid position input detected
   * NO_PIECE_AT_POSITION 4: Origin position input refers to an empty position
   * NOT_YOUR_TURN 5: Piece from the wrong team is chosen
   * ILLEGAL_MOVE 6: The move made resulted in putting one's self in check
   * STALEMATE 7: Stalemate detected, no legal moves available
   * CHECKMATE 1: Checkmate
   
   * Note: STALEMATE is a special case of ILLEGAL MOVE, when at least 3 illegal
   moves have been made in a row in the same turn, the program will ask if the
   player would like to check for a stalemate, if it is true, then it will
   return STALEMATE
   */
  int submitMove_exceptions(int status, char const *origin, char const *target);
  
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
  void promote_pawn(char const* target);
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
                     bool capture, bool enpassant, bool castling);
  
  /*
   ask_for_draw(int num, int flag);
   
   * Function used to ask the player if they would agree to a draw
   * Only called when fifty or more moves have been made without a capture or a
   piece moves to the same position 3 or more times
   
   * flag is used to select which message to output, FIFTY will output the
   message for the fifty-move rule while THREEFOLD would output the Threefold
   repetiton rule message
   
   * No return value
   */
  void ask_for_draw(int num, int flag);
  
  /* Data members */
  /* Number of turns since last capture */
  int turns_since_last_capture=0;
  /* Is black in check? */
  bool black_check=0;
  /* Is white in check? */
  bool white_check=0;
  /* Position of the black king as an integer */
  int blackKing=pos_to_int("E8");
  /* Position of the white king as an integer */
  int whiteKing=pos_to_int("E1");
  /* Temporarily stores captured piece */
  Chesspiece* captured_piece=nullptr;
  /* Position of a piece that can attack the King, 0 means no piece */
  int checker=0;
  /* Position of a pawn that can be captured by en passant, 0 means no piece */
  int en_passant=0;
  /* Whose turn is it */
  Team turn=white;
public:
  /*
   submitMove(char const* org, char const* tgt)
   * Called by the main function to make a move from the origin to target
   
   * Returns 0 if no exceptions raised otherwise it returns an integer value
   depending on the exception raised
   */
  int submitMove(char const* org, char const* tgt);
  
  /*
   display_board()
   * Displays the board using Unicode-8 symbols
   */
  void display_board();
  
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
   */
  std::array<std::array<Chesspiece*,8>,8> positions;
};

#endif /* chessboard_hpp */

