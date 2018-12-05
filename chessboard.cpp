
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
    } else if (!positions[rIndex(org)][fIndex(org)]) {
      throw NO_PIECE_AT_POSITION;
    } else if (positions[rIndex(org)][fIndex(org)]->team!=turn) {
      throw NOT_YOUR_TURN;
    }
    // Making the move
    if ((isEnpassant=checkEnpassant(org, tgt))){
      // Captured piece is not deleted until the move is ensured to be legal
      captured_piece=positions[rIndex(org)][fIndex(tgt)];
      positions[rIndex(tgt)][fIndex(tgt)]
      = positions[rIndex(org)][fIndex(org)];
      positions[rIndex(org)][fIndex(tgt)]=nullptr;
      positions[rIndex(org)][fIndex(org)]=nullptr;
      en_passant=0;
    } else if ((isCastling=checkCastling(org, tgt))) {
      // This move will always be legal
      // checkCastling has an internal legality check
      positions[rIndex(tgt)][fIndex(tgt)]
      = positions[rIndex(org)][fIndex(org)];
      positions[rIndex(org)][fIndex(org)]=nullptr;
      if (target[0]=='G') {
        //H move to F
        positions[rIndex(turn ? "F1":"F8")][fIndex(turn ? "F1":"F8")]
        = positions[rIndex(turn ? "H1":"H8")][fIndex(turn ? "H1":"H8")];
        
        positions[rIndex(turn ? "H1": "H8")][fIndex(turn ? "H1":"H8")]
        = nullptr;
      } else {
        //A move to D
        positions[rIndex(turn ? "D1":"D8")][fIndex(turn ? "D1":"D8")]
        = positions[rIndex(turn ? "A1":"A8")][fIndex(turn ? "A1":"A8")];
        
        positions[rIndex(turn ? "A1": "A8")][fIndex(turn ? "A1":"A8")]
        = nullptr;
      }
    } else if (!positions[rIndex(org)][fIndex(org)]
               ->isvalid(org, tgt, this)) {
      throw INVALID_MOVE;
    } else {
      captured_piece=positions[rIndex(tgt)][fIndex(tgt)];
      
      positions[rIndex(tgt)][fIndex(tgt)]
      = positions[rIndex(org)][fIndex(org)];
      
      positions[rIndex(org)][fIndex(org)]=nullptr;
      if (captured_piece) isCapture=true;
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
      throw ILLEGAL_MOVE;
    }
    
    // Output message
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
    if (++positions[rIndex(tgt)][fIndex(tgt)]
        ->num_at_position[rIndex(tgt)][fIndex(tgt)]>=THREEFOLD) {
      ask_for_draw(positions[rIndex(tgt)][fIndex(tgt)]
                   ->num_at_position[rIndex(tgt)][fIndex(tgt)],THREEFOLD);
    }
    // If the piece has been moved for the 1st time, first_move_made to true
    if (!positions[rIndex(tgt)][fIndex(tgt)]->first_move_made) {
      if (*positions[rIndex(tgt)][fIndex(tgt)]=="Pawn" &&
          target[1]==(turn?'4':'5')) {
        // If at fifth rank set to en_passant
        en_passant=tgt;
      }
      // Increment number of times at original position
      positions[rIndex(tgt)][fIndex(tgt)]
      ->num_at_position[rIndex(org)][fIndex(org)]++;
      positions[rIndex(tgt)][fIndex(tgt)]->first_move_made=true;
    }
    // End of turn
    turn = turn ? black : white;
    if (checkStalemate()) throw STALEMATE;
    return NEXT_TURN;
  } catch (int status) {
    return submitMove_exceptions(status, origin, target);
  }
}

int ChessBoard::submitMove_exceptions(int status, char const* origin,
                                      char const* target) {
  switch (status) {
    case INVALID_MOVE:
      cerr<<(turn ? "White's ":"Black's ");
      cerr<<positions[rIndex(origin)][fIndex(origin)]->name;
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
      cerr<<positions[rIndex(origin)][fIndex(origin)]->name;
      cerr<<" moving to "<<target<<" puts ";
      cerr<<(turn ? "White ":"Black ")<<"in check!"<<endl;
      return status;
    case STALEMATE:
      cout<<"\nStalemate! This game ended in a draw!"<<endl;
      return END_GAME;
    default: // Checkmate
      cout<<(turn ? "Black " : "White ")<<"is in checkmate! ";
      cout<<(turn ? "White " : "Black ")<<"wins!"<<endl;
      return END_GAME;
  }
}

bool ChessBoard::ischeck(Team t, int t_king) {
  if (!t_king) t_king = (t ? whiteKing:blackKing);
  checker=canAttack(t_king, t?black:white);
  if (checker) return true;
  return false;
}

int ChessBoard::legalAttack(int target, Team t) {
  int origin=0;
  int r_t=rIndex(target), f_t=fIndex(target);
  bool isEnpassant=false;
  // tmp storage
  int tmp_enpassant = en_passant;
  for (int r_o=0; r_o<8; r_o++) {
    for (int f_o=0; f_o<8; f_o++) {
      origin = index_to_int(f_o, r_o);
      if (!positions[r_o][f_o]||positions[r_o][f_o]->team!=t) continue;
    
      if ((isEnpassant=checkEnpassant(origin, target))) {
        captured_piece = positions[r_o][f_t];
        positions[r_t][f_t] = positions[r_o][f_o];
        positions[r_o][f_t]=nullptr;
        positions[r_o][f_o]=nullptr;
      } if (!positions[r_o][f_o]->isvalid(origin, target, this)) {
        continue;
      } else {
        captured_piece = positions[r_t][f_t];
        positions[r_t][f_t] = positions[r_o][f_o];
        positions[r_o][f_o]=nullptr;
      }
      if (ischeck(t,(origin==(t?whiteKing:blackKing))?target:0)) {
        undo(origin, target, t, isEnpassant);
        en_passant=tmp_enpassant;
      } else {
        undo(origin, target, t, isEnpassant);
        en_passant=tmp_enpassant;
        return false;
      }
    }
  }
  return true;
}

bool ChessBoard::ischeckmate(Team t) {
  // t is the team that is in check (i.e. team that is losing)
  int attack_pos=checker;
  // If checker can be attacked LEGALLY by the team in check,
  // checkmate is not achieved (cannot use canAttack)
  
  if (!legalAttack(attack_pos, t?white:black)) return false;
  
  // 1->rank++; -1->rank--; 10->file++; -10->file--; 11->file++,rank++;
  // -11->file--,rank--; 9->file++,rank--; -9->file--,rank++
  int king_moves[8]={1,-1,10,-10,11,-11,9,-9};
  int king_pos=(t?whiteKing:blackKing);
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    attack_pos=(t?whiteKing:blackKing) + king_moves[j];
    if (!positions[rIndex(king_pos)][fIndex(king_pos)]
        ->isvalid(king_pos, attack_pos, this)) {
      continue;
    }
    // legality doesn't matter, King cannot put itself in check
    if (!canAttack(attack_pos, t?black:white)) return false;
  }
  
  /*
   Checking for blocks
   * If one of the position on the attack path to the King can be legally
   accessed by at least two pieces, checkmate is not achieved
   * Knight and pawn attacks are unblockable (pawn is only one position away
   while Knights can skip over pieces)
   */
  int count=0;
  if (*positions[rIndex(checker)][fIndex(checker)]!="Knight"||
      *positions[rIndex(checker)][fIndex(checker)]!="Pawn") {
    int distance = (turn ? blackKing:whiteKing) - checker;
    int direction=0;
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
    
    attack_pos=checker;
    for (int i=1; i<abs(distance)/direction; i++) {
      count=0;
      attack_pos += i*direction*(distance<0?-1:1);
      /* Validity of attack_pos are handled in the piece's isvalid member
       function */
      static int first_piece=0;
      static int second_piece=0;
      static Chesspiece* tmp_holder=nullptr;
      if (!(first_piece=legalAttack(attack_pos, t?white:black))) continue;
      //else
      tmp_holder=positions[rIndex(first_piece)][fIndex(first_piece)];
      positions[rIndex(first_piece)][fIndex(first_piece)]=nullptr;
      /* attack_pos will always be empty, since it is a path to the king from
       the checker */
      positions[rIndex(attack_pos)][fIndex(attack_pos)]=
      positions[rIndex(checker)][fIndex(checker)];
      positions[rIndex(checker)][fIndex(checker)]=nullptr;
      second_piece=legalAttack(attack_pos, t?white:black);
      /* Undo what I just did */ //*********
      positions[rIndex(first_piece)][fIndex(first_piece)]=tmp_holder;
      positions[rIndex(checker)][fIndex(checker)]=
      positions[rIndex(attack_pos)][fIndex(attack_pos)];
      positions[rIndex(attack_pos)][fIndex(attack_pos)]=nullptr;
      if (!second_piece) continue;
    }
  }
  return true; // Checkmate
}

bool ChessBoard::checkCastling(int origin, int target) {
  if (origin != pos_to_int(turn ? "E1": "E8") ||
      (target != pos_to_int(turn ? "G1": "G8") &&
       target != pos_to_int(turn ? "C1": "C8")))
    return false;
  
  
  int rook=0;
  int kings_cross=0;
  if (target/10+'A'=='G') {
    rook=pos_to_int(turn ? "H1": "H8");
    kings_cross=pos_to_int(turn ? "F1":"F8");
  } else {
    rook=pos_to_int(turn ? "A1": "A8");
    kings_cross=pos_to_int(turn ? "D1":"D8");
  }
  
  // Not King or not Rook
  if (origin!=(turn?whiteKing:blackKing)||
      !positions[rIndex(rook)][fIndex(rook)]||
      *positions[rIndex(rook)][fIndex(rook)]!="Rook")
    return false;
  
  // Has the first move been made for the King and Rook
  if (positions[rIndex(origin)][fIndex(origin)]->first_move_made ||
      positions[rIndex(rook)][fIndex(rook)]->first_move_made)
    return false;
  
  // King not in check
  if (turn ? white_check : black_check) return false;
  
  // Empty path
  for (int i=1; i<abs(fIndex(rook)-fIndex(origin)); i++) {
    if (positions[rIndex(origin)]
        [fIndex(origin)+i*((fIndex(rook)-fIndex(origin))<0?-1:1)])
      return false;
  }
  // Does the king cross or stop at a position that can be attacked
  // legality doesn't matter, King cannot put itself in check
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
  // Set to 0 so en passant capture on this pawn is no longer available
  en_passant=0;
  
  // Capturing pawn on fifth rank
  if (rIndex(origin) != (turn ? rank5_white : rank5_black)) return false;
  
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

bool ChessBoard::checkStalemate() {
  if (white_check||black_check) return false;
  
  if (checkCastling(pos_to_int(turn?"E1":"E8"), pos_to_int(turn?"G1":"G8")))
    return false;
  if (checkCastling(pos_to_int(turn?"E1":"E8"), pos_to_int(turn?"C1":"C8")))
    return false;
  
  int const king_moves_list[8]={1,-1,10,-10,11,-11,9,-9};
  int king_pos=(turn?whiteKing:blackKing);
  int attack_pos=0;
  int total=0, own_piece=0;
  // Check if King is stuck
  for (int j =0; j<8; j++) {
    attack_pos=king_pos + king_moves_list[j];
    if (attack_pos/10>=1 && attack_pos/10<=8 &&
        attack_pos%10>=1 && attack_pos%10<=8) {
      total++;
      if (!positions[rIndex(king_pos)][fIndex(king_pos)]
          ->isvalid(king_pos, attack_pos, this)) {
        if (positions[rIndex(attack_pos)][fIndex(attack_pos)]->team
              == turn) own_piece++;
        continue;
      }
      // legality doesn't matter, King cannot put itself in check
      if (!canAttack(attack_pos, turn?black:white)) return false;
    }
  }
  
  // Not a stalemate if King is surrounded by its own pieces
  if (total==own_piece) return false;
  /* If the King is stuck, check every other piece if there is any other legal
   move */
  // ap: attack position; o: origin
  int r_ap=0, f_ap=0;
  for (int i=0; i<64; i++) {
    r_ap= i%8; f_ap = i/8;
    if (positions[r_ap][f_ap]&&positions[r_ap][f_ap]->team==turn) continue;
    attack_pos=index_to_int(f_ap, r_ap);
    if (!legalAttack(attack_pos, turn?white:black)) return false;
  }
  return true;
}

void ChessBoard::promote_pawn(char const* target) {
  if ((*positions[rIndex(target)][fIndex(target)]=="Pawn") &&
      target[1]==(turn ? '8' : '1')) {
    cout<<"Pawn promotion available!"<<endl;
    
    char piece='\0';
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
        delete positions[rIndex(target)][fIndex(target)];
        positions[rIndex(target)][fIndex(target)] =
        new (nothrow) Knight(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Knight"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      case 'B':
        delete positions[rIndex(target)][fIndex(target)];
        positions[rIndex(target)][fIndex(target)] =
        new (nothrow) Bishop(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Bishop"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      case 'R':
        delete positions[rIndex(target)][fIndex(target)];
        positions[rIndex(target)][fIndex(target)] =
        new (nothrow) Rook(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Rook"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
      default:
        delete positions[rIndex(target)][fIndex(target)];
        positions[rIndex(target)][fIndex(target)] =
        new (nothrow) Queen(turn?white:black);
        cout<<"Pawn on "<<target<<" promoted to a Queen"<<endl;
        turn? black_check=ischeck(black):white_check=ischeck(white);
        return;
    }
  }
  return;
}

void ChessBoard::undo(int origin, int target, Team t, bool isEnpassant) {
  positions[rIndex(origin)][fIndex(origin)]
  = positions[rIndex(target)][fIndex(target)];
  // If the move wasn't en passant capture
  if (!isEnpassant) {
    positions[rIndex(target)][fIndex(target)]=captured_piece;
    // If the King has moved, update the King's position
    if (target==(t ? whiteKing : blackKing)) {
      (turn ? whiteKing = origin : blackKing = origin);
    }
    captured_piece = nullptr;
    (t ? white_check=ischeck(white) : black_check=ischeck(black));
    return;
  }
  // Regular moves (Castling has its own internal legality check)
  positions[rIndex(target)][fIndex(target)]=nullptr;
  positions[rIndex(origin)][fIndex(target)]=captured_piece;
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
  checker=0;
  en_passant=0;
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
  cout>>*positions[rIndex(target)][fIndex(target)];
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
  char ans='\0';
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


