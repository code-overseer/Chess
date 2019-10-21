
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
  int pwn = target+(t?move::SW:move::NE);
  if (intCheck(pwn) && positions[rIndex(pwn)][fIndex(pwn)]) {
    if (*positions[rIndex(pwn)][fIndex(pwn)]=="Pawn" &&
        positions[rIndex(pwn)][fIndex(pwn)]->team==t) {
      return pwn;
    }
  }
  pwn = target+(t?move::SE:move::NW);
  if (intCheck(pwn) && positions[rIndex(pwn)][fIndex(pwn)]) {
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
  return NULL_POS;
}

int ChessBoard::isLegal(int target, Team t) {
  int origin=0;
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      origin = index_to_int(f, r);
      try {
        isLegal(origin, target, t, true);
        return origin;
      } catch (...) {
        continue;
      }
    }
  }
  return NULL_POS;
}

void ChessBoard::isLegal(int origin, int target, Team t, bool alwaysUndo) {
  
  if (!positions[rIndex(origin)][fIndex(origin)]) {
    throw NO_PIECE_AT_POSITION;
  } else if (positions[rIndex(origin)]
             [fIndex(origin)]->team!=t) {
    throw NOT_YOUR_TURN;
  }
  isEnpassant(origin, target, t);
  isCastling(origin, target, t);
  // Making the move
  if (enpassant) {
    // Captured piece is not deleted until the move is ensured to be legal
    captured_piece = positions[rIndex(origin)][fIndex(target)];
    
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    
    positions[rIndex(origin)][fIndex(target)]=nullptr;
    positions[rIndex(origin)][fIndex(origin)]=nullptr;
  } else if (castling) {
    // This move will always be legal
    // checkCastling has an internal legality check
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    
    positions[rIndex(origin)][fIndex(origin)]=nullptr;
    // if target file is 'G'
    if (fIndex(target)==fIndex("G")) {
      //H move to F
      positions[rIndex(t ? "F1":"F8")][fIndex(t ? "F1":"F8")]
      = positions[rIndex(t ? "H1":"H8")][fIndex(t ? "H1":"H8")];
      
      positions[rIndex(t ? "H1": "H8")][fIndex(t ? "H1":"H8")] = nullptr;
    } else {
      //A move to D
      positions[rIndex(t ? "D1":"D8")][fIndex(t ? "D1":"D8")]
      = positions[rIndex(t ? "A1":"A8")][fIndex(t ? "A1":"A8")];
      
      positions[rIndex(t ? "A1": "A8")][fIndex(t ? "A1":"A8")] = nullptr;
    }
  } else if (positions[rIndex(origin)][fIndex(origin)]
             ->isvalid(origin, target, this)) {
    captured_piece = positions[rIndex(target)][fIndex(target)];
    
    positions[rIndex(target)][fIndex(target)]
    = positions[rIndex(origin)][fIndex(origin)];
    
    positions[rIndex(origin)][fIndex(origin)] = nullptr;
  } else {
    throw INVALID_MOVE;
  }
  
  //Has the king moved? If so, update the King's position (needed for isCheck)
  if (origin==(t ? whiteKing : blackKing))
    (t ? whiteKing = target : blackKing = target);
  
  // Cannot be in check after moving, if illegal, undo move
  if (isCheck(t)!=NULL_POS) {
    undo(origin, target, t);
    throw ILLEGAL_MOVE;
  }
  if (alwaysUndo) undo(origin, target, t);
  
  return;
}

int ChessBoard::submitMove(char const* origin, char const* target) {
  try {
    if (noMemory) throw ENOMEM;
    if (endGame) throw END_GAME;
    // pos_to_int function checks the origin and target arguments
    int org = pos_to_int(origin);
    int tgt = pos_to_int(target);
    if (org==tgt) throw NOT_MOVING;
    
    isLegal(org, tgt, turn);
    // en passant capture on epPawn is no longer available
    epPawn=NULL_POS;
    bool capture=(captured_piece&&!enpassant);
    
    // Output move message
    messageOutput(origin, target, capture);
    // If no capture increment the moves_since_last_capture, else reset to zero
    if (captured_piece) {
      delete captured_piece;
      captured_piece=nullptr;
      turns_since_last_capture=0;
    } else {
      turns_since_last_capture++;
    }
    
    // If the move was not castling or enpassant, check for pawn promotion
    if (!castling && !enpassant) promotePawn(target);
    
    checker = isCheck(turn ? black : white);
    
    // If the piece has been moved for the 1st time, set first_move_made to true
    if (!positions[rIndex(tgt)][fIndex(tgt)]
        ->first_move_made) {
      if (*positions[rIndex(tgt)][fIndex(tgt)]=="Pawn"&&
          target[1]==(turn?'4':'5')) {
        // If pawn moves to fourth rank for the first move set epPawn to tgt
        epPawn=tgt;
      }
      // Increment number of times at original position
      positions[rIndex(tgt)][fIndex(tgt)]
      ->num_at_position[rIndex(org)][fIndex(org)]++;
      
      positions[rIndex(tgt)][fIndex(tgt)]
      ->first_move_made=true;
    }
    // End of turn
    turn = turn ? black : white;
    enpassant=false;
    castling=false;
    // Check/Checkmate/Stalemate
    
    if (isGameOver(turn)) {
      endGame=true;
      if (checker!=NULL_POS) {
        cout<<(turn ? "White " : "Black ")<<"is in checkmate! ";
        cout<<(turn ? "Black " : "White ")<<"wins!"<<endl;
        endGame=true;
        return END_GAME;
      }
      cout<<"Stalemate! This game ended in a draw!"<<endl;
      endGame=true;
      return END_GAME;
    }
    // Check notification
    if (checker!=NULL_POS) {
      cout<<(turn ? "White " : "Black ")<<"is in check"<<endl;
    }
    // Fifty-move rule ask for draw (move= turns / 2)
    if (turns_since_last_capture/2>=FIFTY) {
      drawNotification(turns_since_last_capture/2,FIFTY);
    }
    // Threefold repetition rule ask for draw
    if (++positions[rIndex(tgt)][fIndex(tgt)]
        ->num_at_position[rIndex(tgt)][fIndex(tgt)]>=THREEFOLD) {
      drawNotification(positions[rIndex(tgt)][fIndex(tgt)]
       ->num_at_position[rIndex(tgt)][fIndex(tgt)],THREEFOLD);
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
      cerr<<positions[rIndex(origin)][fIndex(origin)]->name;
      cerr<<" cannot move to "<<target[0]<<target[1]<<"!"<<endl;
      return status;
    case NOT_MOVING:
      cerr<<"A move must be made!"<<endl;
    case INVALID_INPUT:
      cerr<<"Invalid input! ";
      cerr<<"The first character is an upper case letter between A-H and the ";
      cerr<<"second character a digit between 1-8."<<endl;
      return status;
    case NO_PIECE_AT_POSITION:
      cerr<<"There is no piece at position "<<origin[0]<<origin[1]<<'!'<<endl;
      return status;
    case NOT_YOUR_TURN:
      cerr<<"It is not "<<(turn ? "Black's ": "White's ");
      cerr<<"turn to move!"<<endl;
      return status;
    case ILLEGAL_MOVE:
      cerr<<"Illegal move! ";
      cerr<<(turn ? "White's ":"Black's ");
      cerr<<positions[rIndex(origin)][fIndex(origin)]->name;
      cerr<<" moving to "<<target[0]<<target[1]<<" puts ";
      cerr<<(turn ? "White ":"Black ")<<"in check!"<<endl;
      return status;
    case ENOMEM:
      cerr<<"ERROR! Bad allocation was caught previously";
      cerr<<", the game cannot proceed. "<<endl;
      return status;
    default:
      cerr<<"The game has ended, no more moves can be made!\n";
      cerr<<"Please reset the board..."<<endl;
      return status;
  }
}

int ChessBoard::isCheck(Team t) const {
  return canAttack(t?whiteKing:blackKing, t?black:white);;
}

bool ChessBoard::isGameOver(Team t) {
  int attack_pos=NULL_POS;
  for (int r=0; r<8; r++) {
    for (int f=0; f<8; f++) {
      if (positions[r][f]&&positions[r][f]->team==t) continue;
      attack_pos=index_to_int(f, r);
      if (isLegal(attack_pos, t)!=NULL_POS) return false;
    }
  }
  return true;
}

void ChessBoard::isCastling(int origin, int target, Team t) {
  if (origin != pos_to_int(t ? "E1": "E8") ||
      (target != pos_to_int(t ? "G1": "G8") &&
       target != pos_to_int(t ? "C1": "C8")))
    return;
  
  int rook=NULL_POS;
  int kings_cross=NULL_POS;
  // Check if target file is 'G'
  if (fIndex(target)==fIndex("G")) {
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
    return;
  
  // Has the first move been made for the King and Rook
  if (positions[rIndex(origin)][fIndex(origin)]->first_move_made ||
      positions[rIndex(rook)][fIndex(rook)]->first_move_made)
    return;
  
  // King not in check
  if (isCheck(t)!=NULL_POS) return;
  
  // Empty path
  // Moving up a file or down a file? If G is the target move up else move down
  int dir = (fIndex(target)==fIndex("G"))?1:-1;
  for (int i=1; i<((fIndex(target)==fIndex("G"))?3:4); i++) {
    if (positions[rIndex(origin)][fIndex(origin)+i*dir]) return;
  }
  // Does the king cross or stop at a position that can be attacked
  // legality doesn't matter here, King cannot put itself in check
  if (canAttack(target, t?black:white)!=NULL_POS||
      canAttack(kings_cross, t?black:white)!=NULL_POS) return;
  
  castling=true;
  return;
}

void ChessBoard::isEnpassant(int origin, int target, Team t) {
  if (epPawn==NULL_POS) return;
  
  // Captured pawn just moved two steps in its first turn
  int captured = epPawn;
  int const rank5_white = 4;
  int const rank5_black = 3;
  
  // Capturing pawn on its fifth rank
  if (rIndex(origin) != (t ? rank5_white : rank5_black)) return;
  
  // Captured pawn on adjacent file
  if (rIndex(captured)!=rIndex(origin) ||
      fIndex(captured)!=fIndex(target))
    return;
  
  // Both pieces involved must be pawns
  if (*positions[rIndex(origin)][fIndex(target)]!="Pawn" ||
      *positions[rIndex(origin)][fIndex(origin)]!="Pawn")
    return;
  
  enpassant=true;
  return;
}

void ChessBoard::promotePawn(char const* target) {
  if ((*positions[rIndex(target)][fIndex(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion!"<<endl;
    delete positions[rIndex(target)][fIndex(target)];
    try {
      switch (target[2]) {
        case 'N':
          positions[rIndex(target)][fIndex(target)] = new Knight(turn);
          break;
        case 'B':
          positions[rIndex(target)][fIndex(target)] = new Bishop(turn);
          break;
        case 'R':
          positions[rIndex(target)][fIndex(target)] = new Rook(turn);
          break;
        default:
          positions[rIndex(target)][fIndex(target)] = new Queen(turn);
          break;
      }
    } catch (bad_alloc& error) {
      cerr<<"Error! "<<error.what()<<endl;
      noMemory=true;
      return;
    }
    checker= isCheck(turn?black:white);
    cout<<(turn ? "White's ":"Black's ");
    cout<<"Pawn at "<<target[0]<<target[1]<<" has been promoted to a ";
    cout>>*positions[rIndex(target)][fIndex(target)]<<'!'<<endl;
  }
  return;
}

void ChessBoard::undo(int origin, int target, Team t) {
  positions[rIndex(origin)][fIndex(origin)]
  = positions[rIndex(target)][fIndex(target)];
  
  if (enpassant) {
    positions[rIndex(target)][fIndex(target)] = nullptr;
    
    positions[rIndex(origin)][fIndex(target)] = captured_piece;
    
    captured_piece = nullptr;
    enpassant = false;
    return;
  }
  // Regular moves (Castling is always legal if checkCastling returns true)
  positions[rIndex(target)][fIndex(target)] = captured_piece;
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
  noMemory=false;
  endGame=false;
  enpassant=false;
  castling=false;
  checker=NULL_POS;
  epPawn=NULL_POS;
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
  try {
    for (int i=0; i<8; i++) {
      positions[1][i]=new Pawn(white);
      positions[6][i]=new Pawn(black);
    }
    positions[0][0]=new Rook(white);
    positions[0][1]=new Knight(white);
    positions[0][2]=new Bishop(white);
    positions[0][3]=new Queen(white);
    positions[0][4]=new King(white);
    positions[0][5]=new Bishop(white);
    positions[0][6]=new Knight(white);
    positions[0][7]=new Rook(white);
    positions[7][0]=new Rook(black);
    positions[7][1]=new Knight(black);
    positions[7][2]=new Bishop(black);
    positions[7][3]=new Queen(black);
    positions[7][4]=new King(black);
    positions[7][5]=new Bishop(black);
    positions[7][6]=new Knight(black);
    positions[7][7]=new Rook(black);
  } catch (bad_alloc& error) {
    noMemory=true;
    cerr<<"Error! "<<error.what()<<endl;
    return;
  }
  cout<<"A new chess game is started!"<<endl;
}

void ChessBoard::messageOutput(char const *origin, char const* target,
                               bool capture) const {
  cout<<(turn ? "White's " : "Black's ");
  cout>>*positions[rIndex(target)][fIndex(target)];
  /* Target elements are specifically called due to possible third character
   during pawn promotion */
  cout<<" moves from "<<origin<<" to "<<target[0]<<target[1];
  
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

int ChessBoard::pos_to_int(char const* pos) {
  // Inout may have three characters for pawn promotion
  if (pos[2]!='\0'&&pos[3]!='\0') throw INVALID_INPUT;
  
  int val=index_to_int(fIndex(pos), rIndex(pos));
  if (intCheck(val)) return val;
  throw INVALID_INPUT;
}

int ChessBoard::rIndex(char const* position) {
  return position[1]-'1';
}

int ChessBoard::fIndex(char const* position) {
  return position[0]-'A';
}

int ChessBoard::rIndex(int position) {
  return position%10;
}

int ChessBoard::fIndex(int position) {
  return position/10;
}

int ChessBoard::index_to_int(int file, int rank) {
  int val = (rank)+(file)*10;
  
  if (intCheck(val)) return val;
  throw INVALID_INPUT;
}

bool ChessBoard::intCheck(int input) {
  if (fIndex(input)<0 || fIndex(input)>7 ||
      rIndex(input)<0 || rIndex(input)>7) return false;
  return true;
}



