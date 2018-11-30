
#include "ChessBoard.h"

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

void ChessBoard::display_board() {
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

int ChessBoard::canAttack(int target, Team t) {
  for (int f=0; f<8; f++) {
    for (int r=0; r<8; r++) {
      if (!positions[r][f]||positions[r][f]->team!=t) continue;
      if (positions[r][f]->isvalid(index_to_int(f, r), target, this)) {
        return index_to_int(f, r);
      }
    }
  }
  return NULL;
}

int ChessBoard::submitMove(char const* origin, char const* target) {
  try {
    bool isCapture=false;
    bool isCastling=false;
    bool isEnpassant=false;
    int org = pos_to_int(origin);
    int tgt = pos_to_int(target);
    
    if (tgt/10<1 || tgt/10>8 || tgt%10<1 || tgt%10>8 ||
        org/10<1 || org/10>8 || org%10<1 || org%10>8) {
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
      if (target[0]=='G') {
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
    if (org==(turn ? whiteKing : blackKing)) {
      (turn ? whiteKing = tgt : blackKing = tgt);
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
    messageOutput(origin, target, isCapture, isEnpassant, isCastling);
    // Free memory
    if (isCapture||isEnpassant) {
      delete captured_piece;
      captured_piece=nullptr;
    }
    // If the move was not castling or enpassant, check for pawn promotion
    if (!isCastling && !isEnpassant) {
      promote_pawn(target);
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
    if (turns_since_last_capture/2>=FIFTY) {
      ask_for_draw(turns_since_last_capture/2,FIFTY);
    }
    // Threefold repetition rule ask for draw
    if (++positions[rank_index(tgt)][file_index(tgt)]
        ->num_at_position[rank_index(tgt)][file_index(tgt)]>=THREEFOLD) {
      ask_for_draw(positions[rank_index(tgt)][file_index(tgt)]
                   ->num_at_position[rank_index(tgt)][file_index(tgt)],
                   THREEFOLD);
    }
    // If the piece has been moved for the 1st time, first_move_made to true
    if (!positions[rank_index(tgt)][file_index(tgt)]->first_move_made) {
      if (*positions[rank_index(tgt)][file_index(tgt)]=="Pawn" &&
          target[1]==(turn?'4':'5')) {
        // If at fifth rank set to en_passant
        en_passant=tgt;
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
    return submitMove_exceptions(status, origin, target);
  }
}

int ChessBoard::submitMove_exceptions(int status, char const* origin,
                                      char const* target) {
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

bool ChessBoard::ischeck(Team t) {
  check_causer=canAttack((t ? whiteKing:blackKing), t?black:white);
  if (check_causer) return true;
  return false;
}

bool ChessBoard::ischeckmate(Team t) {
  // t is the team that is in check (i.e. team that is losing)
  int attack_pos=check_causer;
  // If check_causer can be attacked by the team in check,
  // checkmate is not achieved
  if (canAttack(attack_pos, t?white:black)) return false;
  // 1->rank++; -1->rank--; 10->file++; -10->file--; 11->file++,rank++;
  // -11->file--,rank--; 9->file++,rank--; -9->file--,rank++
  int king_moves[8]={1,-1,10,-10,11,-11,9,-9};
  int king_pos=(t?whiteKing:blackKing);
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    attack_pos=(t?whiteKing:blackKing) + king_moves[j];
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
    
    // 1->rank++; -1->rank--; 10->file++; -10->file--; 11->file++,rank++;
    // -11->file--,rank--; 9->file++,rank--; -9->file--,rank++
    if (!(abs(distance)%11)) {
      direction =11;//diagonal attack
    } else if (!(abs(distance)%9)) {
      direction =9; //diagonal attack
    } else if (!(abs(distance)%10)) {
      direction=10; // horizontal (file) attack
    } else {
      direction=1;  // vertical (rank) attack
    }
    
    attack_pos=check_causer;
    for (int i=1; i<abs(distance)/direction; i++) {
      count=0;
      attack_pos += i*direction*(distance<0?-1:1);
      if (attack_pos/10<1 || attack_pos/10>8 ||
          attack_pos%10<1 || attack_pos%10>8) continue;
      
      for (int f=0; f<8; f++) {
        for (int r=0; r<8; r++) {
          if (!positions[r][f]||positions[r][f]->team!=t) continue;
          static int origin = index_to_int(f, r);
          if (!positions[r][f]->isvalid(origin, attack_pos, this)) {
            continue;
          } else {
            // Try to move
            /* attack_pos will always be empty, we would not be in this
             function otherwise */
            positions[rank_index(attack_pos)][file_index(attack_pos)]
            = positions[rank_index(origin)][file_index(origin)];
            positions[rank_index(origin)][file_index(origin)]=nullptr;
          }
          
          if (origin==(t ? whiteKing : blackKing)) {
            (t ? whiteKing = attack_pos : blackKing = attack_pos);
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
  }
  return true; // Checkmate
}

bool ChessBoard::checkCastling(int origin, int target) {
  if (origin != pos_to_int(turn ? "E1": "E8") ||
      (target != pos_to_int(turn ? "G1": "G8") &&
       target != pos_to_int(turn ? "C1": "C8")))
    return false;
  
  
  int rook=NULL;
  int kings_cross=NULL;
  if (target/10+'A'=='G') {
    rook=pos_to_int(turn ? "H1": "H8");
    kings_cross=pos_to_int(turn ? "F1":"F8");
  } else {
    rook=pos_to_int(turn ? "A1": "A8");
    kings_cross=pos_to_int(turn ? "D1":"D8");
  }
  
  // Not King or not Rook
  if (origin!=(turn?whiteKing:blackKing)||
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
  // Does the king cross or stop at a position that can be attacked
  if (!canAttack(target, turn?black:white)||
      !canAttack(kings_cross, turn?black:white)) return false;
  
  return true;
}

bool ChessBoard::checkEnpassant(int origin, int target) {
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

bool ChessBoard::checkStalemate() {
  if (checkCastling(pos_to_int(turn?"E1":"E8"), pos_to_int(turn?"G1":"G8")))
    return false;
  if (checkCastling(pos_to_int(turn?"E1":"E8"), pos_to_int(turn?"C1":"C8")))
    return false;
  
  int const king_moves_list[8]={1,-1,10,-10,11,-11,9,-9};
  int king_pos=(turn?whiteKing:blackKing);
  int attack_pos=NULL;
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    attack_pos=(turn?whiteKing:blackKing) + king_moves_list[j];
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
    attack_pos=index_to_int(i/8, i%8);
    for (int j=0; j<64; j++) {
      if (!positions[j%8][j/8]||*positions[j%8][j/8]=="King"||
          positions[j%8][j/8]->team!=turn) continue;
      
      static int origin=index_to_int(j/8, j%8);
      
      if ((isEnpassant=checkEnpassant(origin, attack_pos))) {
        captured_piece=positions[rank_index(origin)][file_index(attack_pos)];
        positions[rank_index(attack_pos)][file_index(attack_pos)]
        = positions[rank_index(origin)][file_index(origin)];
        positions[rank_index(origin)][file_index(attack_pos)]=nullptr;
        positions[rank_index(origin)][file_index(origin)]=nullptr;
      } if (!positions[rank_index(origin)][file_index(origin)]
            ->isvalid(origin, attack_pos, this)) {
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

void ChessBoard::promote_pawn(char const* target) {
  if ((*positions[rank_index(target)][file_index(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion available!"<<endl;
    
    char piece=NULL;
    cout<<"Enter 'N' for Knight, 'B' for Bishop, 'R' for Rook; ";
    cout<<"'Q' for Queen"<<endl;
    while (piece!='Q' && piece!='B' && piece!='R' && piece!='N') {
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

void ChessBoard::undo(int origin, int target, Team t, bool isEnpassant) {
  positions[rank_index(origin)][file_index(origin)]
  = positions[rank_index(target)][file_index(target)];
  // If the move wasn't en passant capture
  if (!isEnpassant) {
    positions[rank_index(target)][file_index(target)]=captured_piece;
    // If the King has moved, update the King's position
    if (target==(t ? whiteKing : blackKing)) {
      (turn ? whiteKing = origin : blackKing = origin);
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

void ChessBoard::resetBoard() {
  turns_since_last_capture=0;
  black_check=0;
  white_check=0;
  blackKing=pos_to_int("E8");
  whiteKing=pos_to_int("E1");
  check_causer=NULL;
  en_passant=NULL;
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
}

void ChessBoard::messageOutput(char const *origin, char const* target,
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

void ChessBoard::ask_for_draw(int num, int flag) {
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


