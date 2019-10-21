#ifndef ChessBoard_hpp
#define ChessBoard_hpp

#include "helper.hpp"

class Chesspiece;

class ChessBoard {
private:
    /*
     ischeck(Team t)
     * Function to check if Team t is in check

     * returns an integer value, if 't' is in check returns the piece attacking
     the king and 0 otherwise
     */
    int _isCheck(Team t) const;
    /*
     isGameOver(Team t)
     * Function to check if Team t is in checkmate or the game is in stalemate
     by checking if Team t has any more legal moves
     * returns a boolean value, true if the game is over and false otherwise
     */
    bool _isGameOver(Team t);

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

     * Changes the caslting data member,
     true if the move is possible and false otherwise

     * Note: This function inherently checks for legality
     */
    void _isCastling(int origin, int target, Team t);

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

     * Changes the enpassant data member,
     true if the move is possible and false otherwise
     */
    void _isEnpassant(int origin, int target, Team t);

    /*
     canAttack(char const* target, Team t, bool pawn)
     * Function to check if ANY piece in Team 't' can capture a target position
     regardless of legality (putting own king in check)
     * This function checks for capture not movement, so pawn diagonals
     are checked

     * Returns an integer value representing the position of the first attacker
     found, NULL_POS otherwise
     */
    int _canAttack(int target, Team t) const;

    /*
     isLegal(int origin, int target, Team t, bool alwaysUndo=false)
     * Function to check if a piece in Team 't' at origin can move to/attack a
     target position legally
     * In this program legality is whether or not a move puts the King in check
     * This is done by moving the piece to the target and checking for legality
     with the isCheck function
     * alwaysUndo is a flag; set to true if any move made regardless of legality
     is to be undone, set to false if only illegal moves is to be undone

     */
    void _isLegal(int origin, int target, Team t, bool alwaysUndo= false);
    /*
     isLegal(int target, Team t)
     * Overloaded function to check if ANY piece in Team 't' can move to/attack a
     target position legally
     * This is done by looping isLegal(int origin, int target, Team t,...) for
     every piece in Team t

     * Returns an integer value; the origin position is returned if the attack
     is legal and possible and NULL_POS otherwise

     * Note: This function will undo any movement after checking for legality
     */
    int _isLegal(int target, Team t);

    /*
     submitMoveExceptions(int status, char const* origin, char const* target);
     * Function used to handle exceptions in the submitMove member function
     * Contains a switch tested with the int value status to return various
     statements to the standard error and some to the standard output

     * Returns a integer value corresponding to the exception raised:

     * CHECKMATE 1: Checkmate
     * INVALID_MOVE 2: The piece cannot move to the position either due to a block
     or the piece cannot move that way
     * NOT_MOVING 3: Origin and target the same
     * INVALID_INPUT 4: Invalid position input detected
     * NO_PIECE_AT_POSITION 5: Origin position input refers to an empty position
     * NOT_YOUR_TURN 6: Piece from the wrong team is chosen
     * ILLEGAL_MOVE 7: The move made resulted in putting one's self in check
     * STALEMATE 8: Stalemate detected, no legal moves available
     * END_GAME 9: The game has ended, no more moves can be made
     * ENOMEM: Insufficient memory to allocate a new Chesspiece object

     */
    int _submitMoveExceptions(int status, char const *origin, char const *target)
    const;

    /*
     undo(char const* origin, char const* target, Team t, bool isEnpassant)
     * Function used to undo the move made if an illegal move has been detected
     * It moves the piece from the target back to the origin and retrieves the
     previous piece that was at the target from the captured_piece data member
     * Team t is used to specify the team that made the move to be undone
     passant move where the capture position does not equal the resulting position

     * No return value
     */
    void _undo(int origin, int target, Team t);
    /*
     promote_pawn(const char *target)

     * Function that promotes the pawn located at target
     * Only called when a pawn has reached its 8th rank
     * Deletes the existing pawn piece and replaces it with promoted piece
     * This function will by default promote the pawn to a queen
     * To promote to a different piece, specify the piece's algerbraic notation
     in upper case at the end of the target position for submitMove:
     Example:
     cb->submitMove("B7", "C8N")
     Output:
     White's Pawn moves from B7 to C8 taking Black's Bishop
     Pawn promotion!
     White's Pawn at C8 has been promoted to a Knight!

     * No return value
     */
    void _promotePawn(char const* target);
    /*
     setupPieces()

     * Function used to setup the pieces to the default position
     * Called when initialising or resetting the board

     * No return value
     */
    void _setupPieces();
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
    void _messageOutput(char const *origin, char const *target, bool capture) const;

    /*
     drawNotification(int num, int flag);

     * Function used to notify the players a draw could be declared
     * Only called when fifty or more moves have been made without a capture or a
     piece moves to the same position 3 or more times

     * flag is used to select which message to output, FIFTY will output the
     message for the fifty-move rule while THREEFOLD would output the Threefold
     repetiton rule message

     * No return value
     */
    void _drawNotification(int num, int flag) const;
    /*
     pos_to_int(char const* pos) const;
     Convert the chess positions into the integer format:
     Example:
     A1 -> 0; (00)
     D4 -> 33;
     H7-> 76;
     */
    static int pos_to_int(char const* pos);
    /*
     Gets the corresponding row (rank) index for the positions array from chess
     positions
     Example:
     B1 -> 10 -> rIndex("B1") = 0;
     D4 -> 33 -> rIndex("D1") = 3;
     H7 -> 76 -> rIndex("H7") = 6;
     */
    static int rIndex(char const* position);
    /*
     Gets the corresponding column (file) index for the positions array from chess
     positions
     Example:
     B1 -> 10 -> fIndex("B1") = 1;
     D4 -> 33 -> fIndex("D4") = 3;
     H7 -> 76 -> fIndex("H7") = 7;
     */
    static int fIndex(char const* position);
    /*
     Gets the corresponding integer format for the array index input
     Example:
     B1 -> fIndex("B1")=1, rIndex("B1")=0 -> index_to_int(1,0)=10;
     D4 -> fIndex("D4")=3, rIndex("D1")=3 -> index_to_int(3,3)=33;
     H7 -> fIndex("H7")=7, rIndex("H7")=6 -> index_to_int(7,6)=76;
     */
    static int index_to_int(int file, int rank);
    /*
     int rIndex(int pos);

     Used by both the chessboard and chesspieces
     Gets the corresponding row (rank) index for the positions array from integer f
     ormat:
     Example:
     B1 -> 10 -> rIndex(10) = 0;
     D4 -> 33 -> rIndex(33) = 3;
     H7 -> 76 -> rIndex(76) = 6;
     */
    static int rIndex(int pos);
    /*
     fIndex(int pos);

     Used by both the chessboard and chesspieces
     Gets the corresponding column (file) index for the positions array from integer
     format
     Example:
     B1 -> 10 -> fIndex(10) = 1;
     D4 -> 33 -> fIndex(33) = 3;
     H7 -> 76 -> fIndex(76) = 7;
     */
    static int fIndex(int pos);
    /*
     inputCheck(int input);

     A function used to validate the input position in integer format;
     Both rank and file must be between 0 and 7;

     Returns true if the format is correct and false otherwise
     */
    static bool intCheck(int input);

    /* Data members */
    /* Number of turns since last capture */
    int _turns_since_last_capture=0;
    /* Position of the black king as an integer */
    int _blackKing=pos_to_int("E8");
    /* Position of the white king as an integer */
    int _whiteKing=pos_to_int("E1");
    /* Temporarily stores captured piece */
    Chesspiece* _captured_piece=nullptr;
    /*Null position*/
    static int const NULL_POS=-1;
    /* Draw rules */
    static int const THREEFOLD = 3;
    static int const FIFTY = 50;
    /* Position of a piece that can attack the King */
    int _checker=NULL_POS;
    /* Position of a pawn that can be captured by en passant */
    int _epPawn=NULL_POS;
    /* Whose turn is it */
    Team _turn=white;
    /* Is it the end of game? */
    bool _endGame=false;
    /*No memory flag, prevents the game from starting or continuing*/
    bool _noMemory=false;
    /*Is the current move an enpassant capture?*/
    bool _enpassant=false;
    /*Is the current move castling?*/
    bool _castling=false;

    /*
     * An array of chesspiece pointers, points to nullptr if the position is
     empty and points to a piece otherwise
     * Rank is the first dimension and file is the second i.e.
     positions[rank][file]
     */
public:
    /*
     submitMove(char const* org, char const* tgt)
     * Called by the main function to make a move from the origin to target
     * This is one of the only function that accepts a character array input of
     positions
     * No errors would be raised as long as the first two characters of org and
     tgt is a valid position
     * A third character may be inputted for tgt when the submitted move results
     in a pawn promotion to promote the pawn to a specific peice (see promotePawn)

     * Returns integer values:
     * INVALID_MOVE 2: The piece cannot move to the position either due to a block
     or the piece cannot move that way
     * NOT_MOVING 3: Origin and target the same
     * INVALID_INPUT 4: Invalid position input detected
     * NO_PIECE_AT_POSITION 5: Origin position input refers to an empty position
     * NOT_YOUR_TURN 6: Piece from the wrong team is chosen
     * ILLEGAL_MOVE 7: The move made resulted in putting one's self in check
     * END_GAME 9: The game has ended, no more moves can be made
     * ENOMEM: Insufficient memory to allocate a new Chesspiece object

     */
    int submitMove(char const* origin, char const* target);

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

    inline Team const &turn() const { return _turn; }
    /* Give access to rIndex and fIndex */
    friend Chesspiece;
    std::array<std::array<Chesspiece*,8>,8> positions = std::array<std::array<Chesspiece*,8>,8>();
};

#endif /* chessboard_hpp */

