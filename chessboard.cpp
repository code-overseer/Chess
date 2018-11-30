
#include "chessboard.hpp"
#include "chesspiece.hpp"
#include <ios>
#include <limits.h>

using namespace std;
Chessboard::Chessboard() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      positions[i][j]=nullptr;
    }
  }
  setupPieces();
}

Chessboard::~Chessboard() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      if (!positions[i][j]) {
        continue;
      }
      delete positions[i][j];
      positions[i][j]=nullptr;
    }
  }
}

void Chessboard::display_board() {
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

int Chessboard::canAttack(char const* target, Team t) {
  char origin[3];
  for (int i=0; i<64; i++) {
    if (!positions[i%8][i/8]||positions[i%8][i/8]->team!=t) continue;
    int_to_pos(index_to_int(i/8, i%8), origin);
    if (positions[i%8][i/8]->isvalid(origin, target, this)) {
      return index_to_int(i/8, i%8);
    }
  }
  return NULL;
}

int Chessboard::submitMove(char const* org, char const* tgt) {
  try {
    bool isCapture=false;
    bool isCastling=false;
    bool isEnpassant=false;
    
    if (org[0]<'A'||org[0]>'H'||org[1]<'1'||org[1]>'8'||
        tgt[0]<'A'||tgt[0]>'H'||tgt[1]<'1'||tgt[1]>'8') {
      throw INVALID_INPUT;
    } else if (!positions[rank_index(org)][file_index(org)]) {
      throw NO_PIECE_AT_POSITION;
    } else if (positions[rank_index(org)][file_index(org)]->team!=turn) {
      throw NOT_YOUR_TURN;
    }
    // Making the move
    if ((isEnpassant=checkEnpassant(org, tgt))){
      // Captured piece is not deleted until the move is ensured to be legal
      captured_piece=positions[rank_index(org)][file_index(tgt)];
      positions[rank_index(tgt)][file_index(tgt)]
      = positions[rank_index(org)][file_index(org)];
      positions[rank_index(org)][file_index(tgt)]=nullptr;
      positions[rank_index(org)][file_index(org)]=nullptr;
      en_passant=NULL;
    } else if ((isCastling=checkCastling(org, tgt))) {
      // This move will always be legal
      // checkCastling has an internal legality check
      positions[rank_index(tgt)][file_index(tgt)]
      = positions[rank_index(org)][file_index(org)];
      positions[rank_index(org)][file_index(org)]=nullptr;
      if (tgt[0]=='G') {
        //H move to F
        positions[rank_index(turn ? "F1":"F8")][file_index(turn ? "F1":"F8")]
        = positions[rank_index(turn ? "H1":"H8")][file_index(turn ? "H1":"H8")];
        
        positions[rank_index(turn ? "H1": "H8")][file_index(turn ? "H1":"H8")]
        = nullptr;
      } else {
        //A move to D
        positions[rank_index(turn ? "D1":"D8")][file_index(turn ? "D1":"D8")]
        = positions[rank_index(turn ? "A1":"A8")][file_index(turn ? "A1":"A8")];
        
        positions[rank_index(turn ? "A1": "A8")][file_index(turn ? "A1":"A8")]
        = nullptr;
      }
    } else if (!positions[rank_index(org)][file_index(org)]
               ->isvalid(org, tgt, this)) {
      throw INVALID_MOVE;
    } else {
      if (!positions[rank_index(tgt)][file_index(tgt)]) {
        // If empty target position
        positions[rank_index(tgt)][file_index(tgt)]
        = positions[rank_index(org)][file_index(org)];
        positions[rank_index(org)][file_index(org)]=nullptr;
      } else {
        // If capture target position
        isCapture=true;
        captured_piece=positions[rank_index(tgt)][file_index(tgt)];
        positions[rank_index(tgt)][file_index(tgt)]
        = positions[rank_index(org)][file_index(org)];
        positions[rank_index(org)][file_index(org)]=nullptr;
      }
    }
    
    //Has the king moved? If so, update the King's position (needed for ischeck)
    if (pos_to_int(org)==(turn ? whiteKing : blackKing)) {
      (turn ? whiteKing = pos_to_int(tgt) : blackKing = pos_to_int(tgt));
    }
    white_check=ischeck(white); // Is white in check?
    black_check=ischeck(black); // Is black in check?
    
    // Cannot be in check after moving, if illegal, undo move
    if (turn ? white_check : black_check) {
      undo(org, tgt, turn, isEnpassant);
      illegalMoves++;
      throw ILLEGAL_MOVE;
    }
    // Reset illegal moves count
    illegalMoves=0;
    // Output move message
    messageOutput(org, tgt, isCapture, isEnpassant, isCastling);
    // Free memory
    if (isCapture||isEnpassant) {
      delete captured_piece;
      captured_piece=nullptr;
    }
    // If the move was not castling or enpassant, check for pawn promotion
    if (!isCastling && !isEnpassant) {
      promote_pawn(tgt);
    }
    // If no capture increment the moves_since_last_capture, else reset to zero
    if (!isCapture && !isEnpassant) {
      turns_since_last_capture++;
    } else {
      turns_since_last_capture=0;
    }
    // Checkmate
    if ((turn ? black_check : white_check)&&ischeckmate(turn ? black : white)) {
      throw CHECKMATE;
    }
    // If in Check, output message
    if (turn ? black_check : white_check) {
      cout<<(turn ? "Black " : "White ")<<"is in check"<<endl;
    }
    // Fifty-move rule ask for draw (move= turns / 2)
    if (turns_since_last_capture/2>=50) {
      ask_for_draw(turns_since_last_capture/2,FIFTY);
    }
    // Threefold repetition rule ask for draw
    if (++positions[rank_index(tgt)][file_index(tgt)]
        ->num_at_position[rank_index(tgt)][file_index(tgt)]>=3) {
      ask_for_draw(positions[rank_index(tgt)][file_index(tgt)]
                   ->num_at_position[rank_index(tgt)][file_index(tgt)],
                   THREEFOLD);
    }
    // If the piece has been moved for the 1st time, first_move_made to true
    if (!positions[rank_index(tgt)][file_index(tgt)]->first_move_made) {
      if (*positions[rank_index(tgt)][file_index(tgt)]=="Pawn" &&
          tgt[1]==(turn?'4':'5')) {
        en_passant=pos_to_int(tgt);
      }
      // Increment number of times at original position
      positions[rank_index(tgt)][file_index(tgt)]
      ->num_at_position[rank_index(org)][file_index(org)]++;
      positions[rank_index(tgt)][file_index(tgt)]->first_move_made=true;
    }
    // End of turn
    turn = turn ? black : white;
    return 0;

  } catch (int status) {
    return submitMove_exceptions(status, org, tgt);
  }
}

int Chessboard::submitMove_exceptions
(int status, char const* origin, char const* target) {
  switch (status) {
    case INVALID_MOVE:
      cerr<<(turn ? "White's ":"Black's ");
      cerr<<positions[rank_index(origin)][file_index(origin)]->name;
      cerr<<" cannot move to "<<target<<'!'<<endl;
      return status;
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
      cerr<<positions[rank_index(origin)][file_index(origin)]->name;
      cerr<<" moving to "<<target<<" puts ";
      cerr<<(turn ? "White ":"Black ")<<"in check!"<<endl;
      if (illegalMoves>=3 && !(white_check||black_check)) {
        cout<<illegalMoves<<" illegal moves have been made in a row!"<<endl;
        cout<<"Check for a stalemate?"<<endl;
        cout<<"('y' for yes and 'n' to continue game)"<<endl;
        char ans=NULL;
        while (ans!='y' && ans!='n') {
          if (ans) {
            cerr<<"Invalid input!"<<endl;
            cerr<<"Would you like to check for a stalemate?"<<endl;
            cerr<<"('y' for yes and 'n' to continue game)"<<endl;
          }
          cin>>ans;
          ans = tolower(ans);
        }
        if (ans=='n') return status;
        if (checkStalemate()) {
          cout<<"\nStalemate! This game is a draw, resetting board.."<<endl;
          resetBoard();
          cout<<"A new game has been started!"<<endl;
          return STALEMATE;
        }
        cout<<"No stalemate detected"<<endl;
      }
      return status;
    default: // Checkmate
      cout<<(turn ? "Black " : "White ")<<"is in checkmate! ";
      cout<<(turn ? "White " : "Black ")<<"wins!"<<endl;
      return status;
  }
}

bool Chessboard::ischeck(Team t) {
  char king_pos[3];
  int_to_pos((t ? whiteKing:blackKing), king_pos);
  check_causer=canAttack(king_pos, t?black:white);
  if (check_causer) return true;
  
  return false;
}

bool Chessboard::ischeckmate(Team t) {
  // t is the team that is in check (i.e. team that is losing)
  char origin[3];
  char attack_pos[3];
  // If check_causer can be attacked by the team in check,
  // checkmate is not achieved
  int_to_pos(check_causer, attack_pos);
  if (canAttack(attack_pos, t?white:black)) return false;
  
  int king_moves[8]={1,-1,10,-10,11,-11,9,-9};
  char king_pos[3];
  int_to_pos((t?whiteKing:blackKing), king_pos);
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    static int tgt=(t?whiteKing:blackKing) + king_moves[j];
    int_to_pos(tgt, attack_pos);
    if (!positions[rank_index(king_pos)][file_index(king_pos)]
        ->isvalid(king_pos, attack_pos, this)) {
      continue;
    }
    
    if (!canAttack(attack_pos, t?black:white)) return false;
  }
  
  /*
   Checking for blocks
   * If the one of the position on the attack path to the King can be legally
   accessed by at least two pieces, checkmate is not achieved
   * Knight and pawn attacks are unblockable (pawn is only one position away
   while Knights can skip over pieces)
   */
  int count=0;
  int distance;
  int direction;
  if (*positions[rank_index(check_causer)][file_index(check_causer)]!="Knight"||
      *positions[rank_index(check_causer)][file_index(check_causer)]!="Pawn") {
    distance = (turn ? blackKing:whiteKing) - check_causer;
    
    if (!(abs(distance)%11)) {
      direction =11;//diagonal attack
    } else if (!(abs(distance)%9)) {
      direction =9; //diagonal attack
    } else if (!(abs(distance)%10)) {
      direction=10; // horizontal (file) attack
    } else { // condition: (!(abs(path/10-(turn ? blackKing:whiteKing)/10)))
      direction=1;  // vertical (rank) attack
    }
    
    int path=check_causer;
    for (int i=1; i<abs(distance)/direction; i++) {
      count=0;
      path += i*direction*(distance<0?-1:1);
      if (path/10<1 || path/10>8 || path%10<1 || path%10>8) continue;
      int_to_pos(path, attack_pos);
      
      for (int j=0; j<64; j++) {
        if (!positions[j%8][j/8]||positions[j%8][j/8]->team!=t) continue;
        int_to_pos(index_to_int(j/8, j%8), origin);
        if (!positions[j%8][j/8]->isvalid(origin, attack_pos, this)) {
          continue;
        } else {
          // Try to move
          /* attack_pos will always be empty, we would not be in this
           function otherwise */
          positions[rank_index(attack_pos)][file_index(attack_pos)]
          = positions[rank_index(origin)][file_index(origin)];
          positions[rank_index(origin)][file_index(origin)]=nullptr;
        }
        
        if (pos_to_int(origin)==(t ? whiteKing : blackKing)) {
          
          (t ? whiteKing = pos_to_int(attack_pos) :
           blackKing = pos_to_int(attack_pos));
          static int tmp_checker = check_causer;
          static Chesspiece* temp
          = positions[rank_index(check_causer)][file_index(check_causer)];
          positions[rank_index(check_causer)][file_index(check_causer)]=nullptr;
          check_causer=NULL;
          // Need to temporarily remove check_causer to see if the King can
          // capture it if it were to stop one position away
          if (!ischeck(t)) {
            count++;
          }
          check_causer=tmp_checker;
          positions[rank_index(check_causer)][file_index(check_causer)]=temp;
          temp=nullptr;
          
        } else {
          if (!ischeck(t)) {
            count++;
          }
        }
        undo(origin, attack_pos, t, false);
        if (count>1) return false;
      }
    }
  }
  return true; // Checkmate
}

bool Chessboard::checkCastling(const char *origin, const char *target) {
  if (strcmp(origin, turn ? "E1": "E8") ||
      (strcmp(target, turn ? "G1": "G8") &&
       strcmp(target, turn ? "C1": "C8")))
    return false;
  
  char const* rook=nullptr;
  char const* kings_cross=nullptr;
  
  if (target[0]=='G') {
    rook=turn ? "H1": "H8";
    kings_cross=turn ? "F1":"F8";
  } else {
    rook=turn ? "A1": "A8";
    kings_cross=turn ? "D1":"D8";
  }
  
  // Not King or not Rook
  if (pos_to_int(origin)!=(turn?whiteKing:blackKing)||
      *positions[rank_index(rook)][file_index(rook)]!="Rook")
    return false;
  
  // Has the first move been made for the King and Rook
  if (positions[rank_index(origin)][file_index(origin)]->first_move_made ||
      positions[rank_index(rook)][file_index(rook)]->first_move_made)
    return false;
  
  // King not in check
  if (turn ? white_check : black_check) return false;
  
  // Empty path
  for (int i=1; i<abs(file_index(rook)-file_index(origin)); i++) {
    if (!!positions[rank_index(origin)]
        [file_index(origin)+i*((file_index(rook)-file_index(origin))<0?-1:1)])
      return false;
  }
  
  char attacker[3];
  for (int i=0; i<64; i++) {
    if (!positions[i%8][i/8] || positions[i%8][i/8]->team==turn) continue;
    int_to_pos(index_to_int(i/8, i%8), attacker);
    if (positions[i%8][i/8]->isvalid(attacker, kings_cross, this)||
        positions[i%8][i/8]->isvalid(attacker, target, this)) {
      return false;
    }
  }
  return true;
}

bool Chessboard::checkEnpassant(const char *origin, const char *target) {
  if (!en_passant) return false;
  
  // Captured pawn just moved two steps in its first turn
  int captured = en_passant;
  int const rank5_white = 4;
  int const rank5_black = 3;
  // Set to NULL so en passant capture on this pawn is no longer available
  en_passant=NULL;
  
  // Capturing pawn on fifth rank
  if (rank_index(origin) != (turn ? rank5_white : rank5_black)) return false;
  
  // Captured pawn on adjacent file
  if (rank_index(captured)!=rank_index(origin) ||
      file_index(captured)!=file_index(target))
    return false;
  
  // Both pieces involved must be pawns
  if (*positions[rank_index(origin)][file_index(target)]!="Pawn" ||
      *positions[rank_index(origin)][file_index(origin)]!="Pawn")
    return false;
  
  return true;
}

bool Chessboard::checkStalemate() {
  char origin[3];
  char attack_pos[3];

  if (checkCastling(turn?"E1":"E8", turn?"G1":"G8")) return false;
  if (checkCastling(turn?"E1":"E8", turn?"C1":"C8")) return false;
  
  int const king_moves_list[8]={1,-1,10,-10,11,-11,9,-9};
  char king_pos[3];
  
  int_to_pos((turn?whiteKing:blackKing), king_pos);
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    static int tgt=(turn?whiteKing:blackKing) + king_moves_list[j];
    int_to_pos(tgt, attack_pos);
    if (!positions[rank_index(king_pos)][file_index(king_pos)]
        ->isvalid(king_pos, attack_pos, this)) {
      continue;
    }
    if (!canAttack(attack_pos, turn?black:white)) return false;
  }
  
  bool isEnpassant=false;
  int tmp_enpassant = en_passant;
  // If the King is stuck, check every other piece there is any other legal move
  for (int i=0; i<64; i++) {
    if (!!positions[i%8][i/8]&&positions[i%8][i/8]->team==turn) continue;
    int_to_pos(index_to_int(i/8, i%8), attack_pos);
    for (int j=0; j<64; j++) {
      if (!positions[j%8][j/8]||*positions[j%8][j/8]=="King"||
          positions[j%8][j/8]->team!=turn) continue;
      
      int_to_pos(index_to_int(j/8, j%8), origin);
      
      if ((isEnpassant=checkEnpassant(origin, attack_pos))) {
        captured_piece=positions[rank_index(origin)][file_index(attack_pos)];
        positions[rank_index(attack_pos)][file_index(attack_pos)]
        = positions[rank_index(origin)][file_index(origin)];
        positions[rank_index(origin)][file_index(attack_pos)]=nullptr;
        positions[rank_index(origin)][file_index(origin)]=nullptr;
      } if (!positions[j%8][j/8]->isvalid(origin, attack_pos, this)) {
        continue;
      } else {
        if (!positions[rank_index(attack_pos)][file_index(attack_pos)]) {
          // If empty target position
          positions[rank_index(attack_pos)][file_index(attack_pos)]
          = positions[rank_index(origin)][file_index(origin)];
          positions[rank_index(origin)][file_index(origin)]=nullptr;
        } else {
          // If capture target position
          captured_piece
          =positions[rank_index(attack_pos)][file_index(attack_pos)];
          positions[rank_index(attack_pos)][file_index(attack_pos)]
          = positions[rank_index(origin)][file_index(origin)];
          positions[rank_index(origin)][file_index(origin)]=nullptr;
        }
      }
      if (ischeck(turn)) {
        undo(origin, attack_pos, turn, isEnpassant);
        en_passant=tmp_enpassant;
      } else {
        undo(origin, attack_pos, turn, isEnpassant);
        en_passant=tmp_enpassant;
        return false;
      }
    }
  }
  return true;
}

void Chessboard::promote_pawn(char const* target) {
  if ((*positions[rank_index(target)][file_index(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion available!"<<endl;
    
    char piece=NULL;
    cout<<"Enter 'N' for Knight, 'B' for Bishop, 'R' for Rook; ";
    cout<<"'Q' for Queen"<<endl;
    while (piece!='Q' && piece!='B' && piece!='R' && piece!='Q') {
      if (piece) {
        cerr<<"Invalid input!"<<endl;
        cerr<<"Pawn promotion available!"<<endl;
        cerr<<"Enter 'N' for Knight, 'B' for Bishop, 'R' for Rook; ";
        cerr<<"'Q' for Queen"<<endl;
      }
      cin>>piece;
      piece = toupper(piece);
    }
    switch (piece) {
      case 'N':
        delete positions[rank_index(target)][file_index(target)];
        positions[rank_index(target)][file_index(target)] =
        new (nothrow) Knight(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Knight"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      case 'B':
        delete positions[rank_index(target)][file_index(target)];
        positions[rank_index(target)][file_index(target)] =
        new (nothrow) Bishop(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Bishop"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      case 'R':
        delete positions[rank_index(target)][file_index(target)];
        positions[rank_index(target)][file_index(target)] =
        new (nothrow) Rook(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Rook"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      default:
        delete positions[rank_index(target)][file_index(target)];
        positions[rank_index(target)][file_index(target)] =
        new (nothrow) Queen(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Queen"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
    }
  }
  return;
}

void Chessboard::undo(const char *origin, const char *target, Team t,
                      bool isEnpassant) {
  positions[rank_index(origin)][file_index(origin)]
  = positions[rank_index(target)][file_index(target)];
  // If the move wasn't en passant capture
  if (!isEnpassant) {
    positions[rank_index(target)][file_index(target)]=captured_piece;
    // If the King has moved, update the King's position
    if (pos_to_int(target)==(t ? whiteKing : blackKing)) {
      (turn ? whiteKing = pos_to_int(origin) :
       blackKing = pos_to_int(origin));
    }
    captured_piece = nullptr;
    (t ? white_check=ischeck(white) : black_check=ischeck(black));
    return;
  }
  // Regular moves (Castling has its own internal legality check)
  positions[rank_index(target)][file_index(target)]=nullptr;
  positions[rank_index(origin)][file_index(target)]=captured_piece;
  captured_piece = nullptr;
  (t ? white_check=ischeck(white) : black_check=ischeck(black));
  return;
}

void Chessboard::resetBoard() {
  turns_since_last_capture=0;
  black_check=0;
  white_check=0;
  blackKing=pos_to_int("E8");
  whiteKing=pos_to_int("E1");
  check_causer=NULL;
  en_passant=NULL;
  turn=white;
  
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      if (!positions[i][j]) continue;
      delete positions[i][j];
      positions[i][j]=nullptr;
    }
  }
  
  setupPieces();
}

void Chessboard::setupPieces() {
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
}

void Chessboard::messageOutput(char const *origin, char const* target,
                               bool capture, bool enpassant, bool castling) {
  cout<<(turn ? "White's " : "Black's ");
  cout>>*positions[rank_index(target)][file_index(target)];
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

void Chessboard::ask_for_draw(int num, int flag) {
  if (flag==THREEFOLD) {
    cout<<"This piece has been at this position "<<num<<" times! ";
  } else if (flag==FIFTY) {
    cout<<"It has been "<<num<<" turns since the last capture! ";
  }
  cout<<"Do you want to call a draw and reset the board?"<<endl;
  cout<<"('y' to reset and 'n' to continue game)"<<endl;
  char ans=NULL;
  while (ans!='y' && ans!='n') {
    if (ans) {
      cerr<<"Invalid input!"<<endl;
      cerr<<"Do you want to call a draw and reset the board?"<<endl;
      cerr<<"('y' to reset and 'n' to continue game)"<<endl;
    }
    cin>>ans;
    ans = tolower(ans);
  }
  if (ans=='n') return;
  cout<<"This game is a draw, resetting board"<<endl;
  resetBoard();
}


