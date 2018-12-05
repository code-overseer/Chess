#include "ChessBoard.h"

#include <iostream>

using namespace std;

int main() {

    cout << "===========================" << endl;
    cout << "Testing the Chess Engine" << endl;
    cout << "===========================" << endl;
    cout << endl;

    ChessBoard cb;
    cout << endl;

    cb.submitMove("D7", "D6");
    cout << endl;

    cb.submitMove("D4", "H6");
    cout << endl;

    cb.submitMove("D2", "D4");
    cout << endl;

    cb.submitMove("F8", "B4");
    cout << endl;

    cout << "===========================" << endl;
    cout << "Alekhine vs. Vasic (1931)" << endl;
    cout << "===========================" << endl;
    cout << endl;

    cb.resetBoard();
    cout << endl;
  
  cb.submitMove("E2", "E3");
  cb.submitMove("A7", "A5");
  cb.submitMove("D1", "H5");
  cb.submitMove("A8", "A6");
  cb.submitMove("H5", "A5");
  cb.submitMove("H7", "H5");
  cb.submitMove("H2", "H4");
  cb.submitMove("A6", "H6");
  cb.submitMove("A5", "C7");
  cb.submitMove("F7", "F6");
  cb.submitMove("C7", "D7");
  cb.submitMove("E8", "F7");
  cb.submitMove("D7", "B7");
  cb.submitMove("D8", "D3");
  cb.submitMove("B7", "B8");
  cb.submitMove("D3", "H7");
  cb.submitMove("B8", "C8");
  cb.submitMove("F7", "G6");
  cb.submitMove("C8", "E6");
  
  
//    cb.submitMove("E2", "E4");
//    cb.submitMove("E7", "E6");
//    cout << endl;
//
//    cb.submitMove("D2", "D4");
//    cb.submitMove("D7", "D5");
//    cout << endl;
//
//    cb.submitMove("B1", "C3");
//    cb.submitMove("F8", "B4");
//    cout << endl;
//
//    cb.submitMove("F1", "D3");
//    cb.submitMove("B4", "C3");
//    cout << endl;
//
//    cb.submitMove("B2", "C3");
//    cb.submitMove("H7", "H6");
//    cout << endl;
//
//    cb.submitMove("C1", "A3");
//    cb.submitMove("B8", "D7");
//    cout << endl;
//
//    cb.submitMove("D1", "E2");
//    cb.submitMove("D5", "E4");
//    cout << endl;
//
//    cb.submitMove("D3", "E4");
//    cb.submitMove("G8", "F6");
//    cout << endl;
//
//    cb.submitMove("E4", "D3");
//    cb.submitMove("B7", "B6");
//    cout << endl;
//
//    cb.submitMove("E2", "E6");
//    cb.submitMove("F7", "E6");
//    cout << endl;
//
//    cb.submitMove("D3", "G6");
//    cout << endl;

    return 0;
}
