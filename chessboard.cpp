
#include "ChessBoard.h"
#include "King.hpp"
#include "Queen.hpp"
#include "Bishop.hpp"
#include "Knight.hpp"
#include "Rook.hpp"
#include "Pawn.hpp"

using namespace std;

ChessBoard::ChessBoard() {
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      positions[r][f]=nullptr;
    }
  }
  setupPieces();
}

ChessBoard::~ChessBoard() {
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      if (positions[r][f]) {
        delete positions[r][f];
        positions[r][f]=nullptr;
      }
    }
  }
}

void ChessBoard::displayBoard() {
  cout << "    ";
  for (int i=0; i<8; i++)
    cout << (char) ('A'+i) << "   ";
  cout << endl;
  for (int i=7; i>=0; i--) {
    cout << "  +---+---+---+---+---+---+---+---+" << endl;
    cout << (char) ('1' + i) << " ";
    for (int j=0; j<8; j++) {
      cout << '|' << " ";
      if (positions[i][j]==nullptr) {
        cout<<"  ";
      } else {
        cout << *positions[i][j];
        cout << " ";
      }
    }
    cout << "|" << endl;
  }
  cout << "  +---+---+---+---+---+---+---+---+" << endl;
  cout << "    ";
  for (int i=0; i<8; i++)
    cout << (char) ('A'+i) << "   ";
  cout<<endl;
}

int ChessBoard::canAttack(int target, Team t) const {
  // Check for pawn attackers
  int pwn = target+(t?-11:11);
  if (pwn/10>=1 && pwn/10<=8 && pwn%10>=1 && pwn%10<=8 &&
      positions[rIndex(pwn)][fIndex(pwn)]) {
    if (*positions[rIndex(pwn)][fIndex(pwn)]=="Pawn" &&
        positions[rIndex(pwn)][fIndex(pwn)]->team==t) {
      return pwn;
    }
  }
  pwn = target+(t?9:-9);
  if (pwn/10>=1 && pwn/10<=8 && pwn%10>=1 && pwn%10<=8 &&
      positions[rIndex(pwn)][fIndex(pwn)]) {
    if (*positions[rIndex(pwn)][fIndex(pwn)]=="Pawn" &&
        positions[rIndex(pwn)][fIndex(pwn)]->team==t) {
      return pwn;
    }
  }
  
  // Check for others
  for (int f=0; f<8; f++) {
    for (int r=0; r<8; r++) {
      if (!positions[r][f]||positions[r][f]->team!=t) continue;
      if (positions[r][f]->isvalid(index_to_int(f, r), target, this)) {
        return index_to_int(f, r);
      }
    }
  }
  return 0;
}

int ChessBoard::isLegal(int target, Team t) {
  int origin=0;
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      origin = index_to_int(f, r);
      try {
        isLegal(origin,target,t,true);
        return origin;
      } catch (int e) {
        continue;
      }
    }
  }
  return false;
}

int ChessBoard::isLegal(int origin, int target, Team t, bool alwaysUndo) {
  bool enpassant=false;
  
  if (!positions[rIndex(origin)][fIndex(origin)]) {
    throw NO_PIECE_AT_POSITION;
  } else if (positions[rIndex(origin)][fIndex(origin)]->team!=t) {
    throw NOT_YOUR_TURN;
  }
  // Making the move
  if ((enpassant=isEnpassant(origin, target, t))){
    // Captured piece is not deleted until the move is ensured to be legal
    captured_piece=positions[rIndex(origin)][fIndex(target)];
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    positions[rIndex(origin)][fIndex(target)]=nullptr;
    positions[rIndex(origin)][fIndex(origin)]=nullptr;
  } else if (isCastling(origin, target, t)) {
    // This move will always be legal
    // checkCastling has an internal legality check
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    positions[rIndex(origin)][fIndex(origin)]=nullptr;
    // if target file is 'G'
    if (target/10==7) {
      //H move to F
      positions[rIndexChar(t ? "F1":"F8")][fIndexChar(t ? "F1":"F8")]
      = positions[rIndexChar(t ? "H1":"H8")][fIndexChar(t ? "H1":"H8")];
      
      positions[rIndexChar(t ? "H1": "H8")][fIndexChar(t ? "H1":"H8")]
      = nullptr;
    } else {
      //A move to D
      positions[rIndexChar(t ? "D1":"D8")][fIndexChar(t ? "D1":"D8")]
      = positions[rIndexChar(t ? "A1":"A8")][fIndexChar(t ? "A1":"A8")];
      
      positions[rIndexChar(t ? "A1": "A8")][fIndexChar(t ? "A1":"A8")]
      = nullptr;
    }
  } else if (positions[rIndex(origin)][fIndex(origin)]
             ->isvalid(origin, target, this)) {
    captured_piece=positions[rIndex(target)][fIndex(target)];
    
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    
    positions[rIndex(origin)][fIndex(origin)]=nullptr;
  } else {
    throw INVALID_MOVE;
  }
  
  //Has the king moved? If so, update the King's position (needed for isCheck)
  if (origin==(t ? whiteKing : blackKing)) {
    (t ? whiteKing = target : blackKing = target);
  }
  // Cannot be in check after moving, if illegal, undo move
  if (isCheck(t)) {
    undo(origin, target, t, enpassant);
    throw ILLEGAL_MOVE;
  }
  if (alwaysUndo) undo(origin, target, t, enpassant);
  
  return origin;
}

int ChessBoard::submitMove(char const* origin, char const* target) {
  try {
    // pos_to_int function checks the origin and target arguments
    int org = pos_to_int(origin);
    int tgt = pos_to_int(target);
    if (org==tgt) throw NOT_MOVING;
    
    isLegal(org, tgt, turn);
    // Set to 0 so en passant capture on enpassant pawn is no longer available
    epPawn=0;
    
    bool enpassant=isEnpassant(org, tgt, turn);
    bool castling=isCastling(org, tgt, turn);
    bool capture=(captured_piece&&!enpassant);
    // Output move message
    messageOutput(origin, target, capture, enpassant, castling);
    // If no capture increment the moves_since_last_capture, else reset to zero
    if (captured_piece) {
      delete captured_piece;
      captured_piece=nullptr;
      turns_since_last_capture=0;
    } else {
      turns_since_last_capture++;
    }
    
    // If the move was not castling or enpassant, check for pawn promotion
    if (!castling && !enpassant) {
      promotePawn(target);
    }
    checker = isCheck(turn ? black : white);
    
    // If the piece has been moved for the 1st time, set first_move_made to true
    if (!positions[rIndex(tgt)][fIndex(tgt)]->first_move_made) {
      if (*positions[rIndex(tgt)][fIndex(tgt)]=="Pawn" &&
          target[1]==(turn?'4':'5')) {
        // If pawn moves to fourth rank for the first move set epPawn to tgt
        epPawn=tgt;
      }
      // Increment number of times at original position
      positions[rIndex(tgt)][fIndex(tgt)]
      ->num_at_position[rIndex(org)][fIndex(org)]++;
      positions[rIndex(tgt)][fIndex(tgt)]->first_move_made=true;
    }
    // End of turn
    turn = turn ? black : white;
    // Check/Checkmate/Stalemate
    if (isGameOver(turn)) {
      if (checker) throw CHECKMATE;
      throw STALEMATE;
    }
    // Check notification
    if (checker) {
      cout<<(turn ? "White " : "Black ")<<"is in check"<<endl;
    }
    // Fifty-move rule ask for draw (move= turns / 2)
    if (turns_since_last_capture/2>=FIFTY) {
      drawNotification(turns_since_last_capture/2,FIFTY);
    }
    // Threefold repetition rule ask for draw
    if (++positions[rIndex(tgt)][fIndex(tgt)]
        ->num_at_position[rIndex(tgt)][fIndex(tgt)]>=THREEFOLD)
    {
      drawNotification(positions[rIndex(tgt)][fIndex(tgt)]
                   ->num_at_position[rIndex(tgt)][fIndex(tgt)], THREEFOLD);
    }
    
    return NEXT_TURN;
  } catch (int status) {
    return submitMoveExceptions(status, origin, target);
  }
}

int ChessBoard::submitMoveExceptions(int status, char const* origin,
                                      char const* target) const {
  switch (status) {
    case INVALID_MOVE:
      cerr<<(turn ? "White's ":"Black's ");
      cerr<<positions[rIndexChar(origin)][fIndexChar(origin)]->name;
      cerr<<" cannot move to "<<target<<'!'<<endl;
      return status;
    case NOT_MOVING:
      cerr<<"A move must be made!"<<endl;
    case INVALID_INPUT:
      cerr<<"Invalid input! ";
      cerr<<"Input must be in the form 'A1', where the first character is a ";
      cerr<<"letter between A-H and the second character a digit between ";
      cerr<<"1-8."<<endl;
      return status;
    case NO_PIECE_AT_POSITION:
      cerr<<"There is no piece at position "<<origin<<'!'<<endl;
      return status;
    case NOT_YOUR_TURN:
      cerr<<"It is not "<<(turn ? "Black's ": "White's ");
      cerr<<"turn to move!"<<endl;
      return status;
    case ILLEGAL_MOVE:
      cerr<<"Illegal move! ";
      cerr<<(turn ? "White's ":"Black's ");
      cerr<<positions[rIndexChar(origin)][fIndexChar(origin)]->name;
      cerr<<" moving to "<<target<<" puts ";
      cerr<<(turn ? "White ":"Black ")<<"in check!"<<endl;
      return status;
    case STALEMATE:
      cout<<"\nStalemate! This game ended in a draw!"<<endl;
      return END_GAME;
    default: // Checkmate
      cout<<(turn ? "White " : "Black ")<<"is in checkmate! ";
      cout<<(turn ? "Black " : "White ")<<"wins!"<<endl;
      return END_GAME;
  }
}

int ChessBoard::isCheck(Team t) const {
  return canAttack(t?whiteKing:blackKing, t?black:white);;
}

bool ChessBoard::isGameOver(Team t) {
  int attack_pos=0;
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      if (positions[r][f]&&positions[r][f]->team==t) continue;
      attack_pos=index_to_int(f, r);
      if (isLegal(attack_pos, t)) return false;
    }
  }
  return true; // Checkmate
}

bool ChessBoard::isCastling(int origin, int target, Team t) const {
  if (origin != pos_to_int(t ? "E1": "E8") ||
      (target != pos_to_int(t ? "G1": "G8") &&
       target != pos_to_int(t ? "C1": "C8")))
    return false;
  
  int rook=0;
  int kings_cross=0;
  // Check if target file is 'G'
  if (target/10==7) {
    rook=pos_to_int(t ? "H1": "H8");
    kings_cross=pos_to_int(t ? "F1":"F8");
  } else {
    rook=pos_to_int(t ? "A1": "A8");
    kings_cross=pos_to_int(t ? "D1":"D8");
  }
  
  // Not King or not Rook
  if (origin!=(t?whiteKing:blackKing)||
      !positions[rIndex(rook)][fIndex(rook)]||
      *positions[rIndex(rook)][fIndex(rook)]!="Rook")
    return false;
  
  // Has the first move been made for the King and Rook
  if (positions[rIndex(origin)][fIndex(origin)]->first_move_made ||
      positions[rIndex(rook)][fIndex(rook)]->first_move_made)
    return false;
  
  // King not in check
  if (isCheck(t)) return false;
  
  // Empty path
  for (int i=1; i<abs(fIndex(rook)-fIndex(origin)); i++) {
    if (positions[rIndex(origin)]
        [fIndex(origin)+i*((fIndex(rook)-fIndex(origin))<0?-1:1)])
      return false;
  }
  // Does the king cross or stop at a position that can be attacked
  // legality doesn't matter, King cannot put itself in check
  if (!canAttack(target, t?black:white)||
      !canAttack(kings_cross, t?black:white)) return false;
  
  return true;
}

bool ChessBoard::isEnpassant(int origin, int target, Team t) const {
  if (!epPawn) return false;
  
  // Captured pawn just moved two steps in its first turn
  int captured = epPawn;
  int const rank5_white = 4;
  int const rank5_black = 3;
  
  // Capturing pawn on fifth rank
  if (rIndex(origin) != (t ? rank5_white : rank5_black)) return false;
  
  // Captured pawn on adjacent file
  if (rIndex(captured)!=rIndex(origin) ||
      fIndex(captured)!=fIndex(target))
    return false;
  
  // Both pieces involved must be pawns
  if (*positions[rIndex(origin)][fIndex(target)]!="Pawn" ||
      *positions[rIndex(origin)][fIndex(origin)]!="Pawn")
    return false;
  
  return true;
}

void ChessBoard::promotePawn(char const* target) {
  if ((*positions[rIndexChar(target)][fIndexChar(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion!"<<endl;
    char piece=target[2];
    switch (piece) {
      case 'N':
        delete positions[rIndexChar(target)][fIndexChar(target)];
        positions[rIndexChar(target)][fIndexChar(target)] =
        new (nothrow) Knight(turn?white:black);
        checker= isCheck(turn?black:white);
        break;
      case 'B':
        delete positions[rIndexChar(target)][fIndexChar(target)];
        positions[rIndexChar(target)][fIndexChar(target)] =
        new (nothrow) Bishop(turn?white:black);
        checker= isCheck(turn?black:white);
        break;
      case 'R':
        delete positions[rIndexChar(target)][fIndexChar(target)];
        positions[rIndexChar(target)][fIndexChar(target)] =
        new (nothrow) Rook(turn?white:black);
        checker= isCheck(turn?black:white);
        break;
      default:
        delete positions[rIndexChar(target)][fIndexChar(target)];
        positions[rIndexChar(target)][fIndexChar(target)] =
        new (nothrow) Queen(turn?white:black);
        checker= isCheck(turn?black:white);
        break;
    }
    if (!positions[rIndexChar(target)][fIndexChar(target)]) {
      cerr<<"ERROR! Insufficient memory"<<endl;
      throw ENOMEM;
    }
    cout<<(turn ? "White's ":"Black's ");
    cout<<"Pawn at "<<target<<" has been promoted to a ";
    cout>>*positions[rIndexChar(target)][fIndexChar(target)]<<'!'<<endl;
  }
  return;
}

void ChessBoard::undo(int origin, int target, Team t, bool isEnpassant) {
  positions[rIndex(origin)][fIndex(origin)]
  = positions[rIndex(target)][fIndex(target)];
  
  if (isEnpassant) {
    positions[rIndex(target)][fIndex(target)]=nullptr;
    positions[rIndex(origin)][fIndex(target)]=captured_piece;
    captured_piece = nullptr;
    return;
  }
  // Regular moves (Castling is always legal if checkCastling returns true)
  positions[rIndex(target)][fIndex(target)]=captured_piece;
  // If the King has moved, undo the King's position
  if (target==(t ? whiteKing : blackKing))
    (t ? whiteKing = origin : blackKing = origin);
  
  captured_piece = nullptr;
  return;
}

void ChessBoard::resetBoard() {
  turns_since_last_capture=0;
  blackKing=pos_to_int("E8");
  whiteKing=pos_to_int("E1");
  checker=0;
  epPawn=0;
  turn=white;
  
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      if (!positions[r][f]) continue;
      delete positions[r][f];
      positions[r][f]=nullptr;
    }
  }
  
  setupPieces();
}

void ChessBoard::setupPieces() {
  for (int i=0; i<8; i++) {
    positions[1][i]=new (nothrow) Pawn(white);
    positions[6][i]=new (nothrow) Pawn(black);
    if (!positions[1][i]||!positions[6][i]) {
      cerr<<"Insufficient memory"<<endl;
      throw ENOMEM;
    }
  }
  
  positions[0][0]=new (nothrow) Rook(white);
  positions[0][1]=new (nothrow) Knight(white);
  positions[0][2]=new (nothrow) Bishop(white);
  positions[0][3]=new (nothrow) Queen(white);
  positions[0][4]=new (nothrow) King(white);
  positions[0][5]=new (nothrow) Bishop(white);
  positions[0][6]=new (nothrow) Knight(white);
  positions[0][7]=new (nothrow) Rook(white);
  positions[7][0]=new (nothrow) Rook(black);
  positions[7][1]=new (nothrow) Knight(black);
  positions[7][2]=new (nothrow) Bishop(black);
  positions[7][3]=new (nothrow) Queen(black);
  positions[7][4]=new (nothrow) King(black);
  positions[7][5]=new (nothrow) Bishop(black);
  positions[7][6]=new (nothrow) Knight(black);
  positions[7][7]=new (nothrow) Rook(black);
  for (int i=0; i<8; i++) {
    if (!positions[0][i]||!positions[7][i]) {
      cerr<<"Insufficient memory"<<endl;
      throw ENOMEM;
    }
  }
  cout<<"A new chess game is started!"<<endl;
}

void ChessBoard::messageOutput(char const *origin, char const* target,
                               bool capture, bool enpassant, bool castling)
const {
  cout<<(turn ? "White's " : "Black's ");
  cout>>*positions[rIndexChar(target)][fIndexChar(target)];
  cout<<" moves from "<<origin<<" to "<<target;
  
  if (capture) {
    cout<<" taking "<<(turn ? "Black's " : "White's ");
    cout>>*captured_piece;
  } else if (enpassant) {
    cout<<" taking "<<(turn ? "Black's " : "White's ");
    cout>>*captured_piece;
    cout<<" en passant";
  } else if (castling) {
    cout<<" and "<<(turn ? "White's " : "Black's ");
    cout<<"Rook";
    if (target[0]=='G') {
      cout<<" moves from "<<(turn ? "H1":"H8");
      cout<<" to "<<(turn ? "F1":"F8");
    } else {
      cout<<" moves from "<<(turn ? "A1":"A8");
      cout<<" to "<<(turn ? "D1":"D8");
    }
  }
  cout<<endl;
  return;
}

void ChessBoard::drawNotification(int num, int flag) const {
  if (flag==THREEFOLD) {
    cout<<"This piece has been at this position "<<num<<" times! ";
  } else if (flag==FIFTY) {
    cout<<"It has been "<<num<<" turns since the last capture! ";
  } else {
    cerr<<"Invalid flag supplied to drawNotification"<<endl;
    return;
  }
  cout<<endl;
  cout<<"You may declare a draw."<<endl;
  return;
}

int ChessBoard::pos_to_int(char const* pos) const {
  if (pos[0]<'A'||pos[0]>'H'||pos[1]<'1'||pos[1]>'8')
    throw INVALID_INPUT;
  int val=(pos[0]-'A'+1)*10;
  val+=(pos[1]-'0');
  return val;
}

int ChessBoard::rIndexChar(char const* position) const {
  return position[1]-'1';
}

int ChessBoard::fIndexChar(char const* position) const {
  return position[0]-'A';
}

int ChessBoard::index_to_int(int file, int rank) const {
  return (rank+1)+(file+1)*10;
}



