
#include "chessboard.hpp"
#include "chesspiece.hpp"
#include "errors.h"

using namespace std;
Chessboard::Chessboard() {
  for (int i=0; i<8; i++) {
    positions[1][i]=new (nothrow) Pawn(white);
    positions[6][i]=new (nothrow) Pawn(black);
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
        cout << *positions[i][j] << " ";
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

void Chessboard::move_piece(char const* org, char const* tgt) {
  try {
    Chesspiece* captured_piece=nullptr;
    bool iscapture=false;
    bool castling=false;
    bool is_enpassant=false;
    
    if (org[0]<'A'||org[0]>'H'||org[1]<'1'||org[1]>'8'||
        tgt[0]<'A'||tgt[0]>'H'||tgt[1]<'1'||tgt[1]>'8') {
      throw INVALID_INPUT;
    } else if (!positions[rank_index(org)][file_index(org)]) {
      throw NO_PIECE_AT_POSITION;
    } else if (positions[rank_index(org)][file_index(org)]->team!=turn) {
      throw NOT_YOUR_TURN;
    }
    
    if (isenpassant(org, tgt)){
      is_enpassant=true;
      captured_piece=positions[rank_index(org)][file_index(tgt)];
      positions[rank_index(tgt)][file_index(tgt)]
      = positions[rank_index(org)][file_index(org)];
      positions[rank_index(org)][file_index(tgt)]=nullptr;
      positions[rank_index(org)][file_index(org)]=nullptr;
      en_passant=NULL;
      
    } else if (iscastling(org,tgt)) {
      castling=true;
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
      if (!positions[rank_index(tgt)][file_index(tgt)]) { // If empty target position
        positions[rank_index(tgt)][file_index(tgt)]
        = positions[rank_index(org)][file_index(org)];
        
        positions[rank_index(org)][file_index(org)]=nullptr;
      } else { // If capture target position
        iscapture=true;
        captured_piece=positions[rank_index(tgt)][file_index(tgt)];
        positions[rank_index(tgt)][file_index(tgt)]
        = positions[rank_index(org)][file_index(org)];
        
        positions[rank_index(org)][file_index(org)]=nullptr;
      }
    }
    
    /* Has the king moved? If so, update the King's position */
    if (pos_to_int(org)==(turn ? whiteKing : blackKing)) {
      (turn ? whiteKing = pos_to_int(tgt) : blackKing = pos_to_int(tgt));
    }
    
    white_check=ischeck(white);
    black_check=ischeck(black);
    
    if (turn ? white_check : black_check) {
      positions[rank_index(org)][file_index(org)]
      = positions[rank_index(tgt)][file_index(tgt)];
      if (!en_passant) {
        positions[rank_index(tgt)][file_index(tgt)]=captured_piece;
        /* Has the king moved? If so, update the King's position */
        if (pos_to_int(tgt)==(turn ? whiteKing : blackKing)) {
          (turn ? whiteKing = pos_to_int(org) : blackKing = pos_to_int(org));
        }
        if (!!captured_piece) captured_piece = nullptr;
        (turn ? white_check=ischeck(white) : black_check=ischeck(black));
        throw ILLEGAL_MOVE;
      }
      positions[rank_index(tgt)][file_index(tgt)]=nullptr;
      positions[rank_index(org)][file_index(tgt)]=captured_piece;
      captured_piece = nullptr;
      (turn ? white_check=ischeck(white) : black_check=ischeck(black));
      throw ILLEGAL_MOVE;
    }
    
    cout<<(turn ? "White's " : "Black's ");
    cout>>*positions[rank_index(tgt)][file_index(tgt)];
    cout<<" moves from "<<org<<" to "<<tgt;
    
    if (iscapture) {
      cout<<" taking "<<(turn ? "Black's " : "White's ");
      cout>>*captured_piece;
      delete captured_piece;
      captured_piece=nullptr;
    }
    if (is_enpassant) {
      cout<<" taking "<<(turn ? "Black's " : "White's ");
      cout>>*captured_piece;
      cout<<" en passant";
      delete captured_piece;
      captured_piece=nullptr;
    }
    
    if (castling) {
      cout<<" and "<<(turn ? "White's " : "Black's ");
      cout<<"Rook";
      if (tgt[0]=='G') {
        cout<<" moves from "<<(turn ? "H1":"H8");
        cout<<" to "<<(turn ? "F1":"F8");
      } else {
        cout<<" moves from "<<(turn ? "A1":"A8");
        cout<<" to "<<(turn ? "D1":"D8");
      }
    }
    cout<<endl;
    
    // Pawn Promotion
    if (!castling && !is_enpassant) promote_pawn(tgt);
    
    if ((black_check||white_check)&&ischeckmate(turn ? black : white)) {
      throw NO_ERROR;
    }
    
    if (turn ? black_check : white_check) {
      cout<<(turn ? "Black " : "White ")<<"is in check"<<endl;
    }
    
    if (!positions[rank_index(tgt)][file_index(tgt)]->first_move_made) {
      
      if (!strcmp(positions[rank_index(tgt)][file_index(tgt)]->name,"Pawn") &&
          tgt[1]==(turn?'4':'5')) {
        en_passant=pos_to_int(tgt);
      }
      
      positions[rank_index(tgt)][file_index(tgt)]->first_move_made=true;
    }
    
    turn = turn ? black : white;
    
  } catch (int status) {
    switch (status) {
      case INVALID_MOVE:
        cerr<<(turn ? "White's ":"Black's ");
        cerr<<positions[rank_index(org)][file_index(org)]->name;
        cerr<<" cannot move to "<<tgt<<'!'<<endl;
        throw status;
        break;
      case INVALID_INPUT:
        cerr<<"Invalid input! ";
        cerr<<"Input must be in the form 'A1', where the first character is a ";
        cerr<<"letter between A-H and the second character a digit between ";
        cerr<<"1-8."<<endl;
        throw status;
        break;
      case NO_PIECE_AT_POSITION:
        cerr<<"There is no piece at position "<<org<<'!'<<endl;
        throw status;
        break;
      case NOT_YOUR_TURN:
        cerr<<"It is not "<<(turn ? "Black's ": "White's ");
        cerr<<"turn to move!"<<endl;
        throw status;
        break;
      case ILLEGAL_MOVE:
        cerr<<"Illegal move! ";
        cerr<<(turn ? "White's ":"Black's ");
        cerr<<positions[rank_index(org)][file_index(org)]->name;
        cerr<<" moving to "<<tgt<<" puts ";
        cerr<<(turn ? "White ":"Black ")<<"in check!"<<endl;
        throw status;
        break;
      default:
        cout<<(turn ? "Black " : "White ")<<"is in checkmate! ";
        cout<<(turn ? "White " : "Black ")<<"wins!"<<endl;
        throw status;
        break;
    }
  }
}

bool Chessboard::ischeck(Team t) {
  char origin[3];
  char king_pos[3];
  int_to_pos((t ? whiteKing:blackKing), king_pos);
  
  for (int i=0; i<64; i++) {
    if (!positions[i%8][i/8]||positions[i%8][i/8]->team==t) continue;
    int_to_pos(index_to_int(i/8, i%8), origin);
    if (positions[i%8][i/8]->isvalid(origin, king_pos, this)) {
      check_causer=index_to_int(i/8, i%8);
      return true;
    }
  }
  
  if (check_causer &&
      positions[rank_index(check_causer)][file_index(check_causer)]->team!=t) {
      check_causer=NULL;
  }
  return false;
}

bool Chessboard::ischeckmate(Team t) {
  // t is the team that is in check (i.e. team that is losing)
  char origin[3];
  char attack_pos[3];
  
  // If check_causer can be attacked by the team in check,
  // checkmate is not achieved
  int_to_pos(check_causer, attack_pos);
  for (int i=0; i<64; i++) {
    if (!positions[i%8][i/8]||positions[i%8][i/8]->team!=t) continue;
    int_to_pos(index_to_int(i/8, i%8), origin);
    if (positions[i%8][i/8]->isvalid(origin, attack_pos, this)) {
      return false;
    }
  }
  int king_moves[8]={1,-1,10,-10,11,-11,9,-9};
  char king_pos[3];
  bool can_attack=false;
  int_to_pos((t?whiteKing:blackKing), king_pos);
  
  // Check if King is surrounded i.e. can the loosing team's King escape from
  // check on its own
  
  for (int j =0; j<8; j++) {
    can_attack=false;
    int_to_pos((t?whiteKing:blackKing) + king_moves[j], attack_pos);
    if (!positions[rank_index(king_pos)][file_index(king_pos)]
        ->isvalid(king_pos, attack_pos, this)) {
      continue;
    }
    
    for (int i=0; i<64; i++) {
      if (!positions[i%8][i/8]||positions[i%8][i/8]->team==t) continue;
      int_to_pos(index_to_int(i/8, i%8), origin);
      if (positions[i%8][i/8]->isvalid(origin, attack_pos, this)) {
        can_attack=true;
        break;
      }
    }
    if (!can_attack) return false;
  }
  // King is now surrounded
  
  int count=0;
  int distance;
  int direction;
  int path=check_causer;
  /*
   Checking for blocks
   * If the one of the position on the attack path to the King can be accessed
   by at least two pieces, checkmate is not achieved
   * Knight and pawn attacks are unblockable (pawn is only one position away
   while Knights can skip over pieces)
   */
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
    
    for (int i=1; i<abs(distance)/direction; i++) {
      count=0;
      path += i*direction*(distance<0?-1:1);
      int_to_pos(path, attack_pos);
      for (int j=0; j<64; j++) {
        if (!positions[j%8][j/8]||positions[j%8][j/8]->team!=t) continue;
        int_to_pos(index_to_int(j/8, j%8), origin);
        if (positions[j%8][j/8]->isvalid(origin, attack_pos, this)) {
          count++;
          if (count>1) return false;
        }
      }
    }
  }
  return true; // Checkmate
}

bool Chessboard::iscastling(const char *origin, const char *target) {
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

bool Chessboard::isenpassant(const char *origin, const char *target) {
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

void Chessboard::promote_pawn(char const* target) {
  char piece;
  if ((*positions[rank_index(target)][file_index(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion available!"<<endl;
    
    cout<<"Enter 'N' for Knight, 'B' for Bishop, 'R' for Rook; ";
    cout<<"any other input would promote the pawn to a Queen";
    cout<<endl;
    cin.get(); // Clears input buffer
    cin.get(piece);
    cin.get(); // Clears input buffer
    toupper(piece);
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
