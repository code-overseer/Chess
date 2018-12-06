HEADER=ChessPiece.hpp ChessBoard.h helper.hpp King.hpp Queen.hpp Bishop.hpp Knight.hpp Rook.hpp Pawn.hpp

COMPILER=c++

OPTIONS=-g -Wall

chess: ChessMain.o ChessPiece.o ChessBoard.o King.o Queen.o Bishop.o Knight.o Rook.o Pawn.o
	$(COMPILER) -g -Wall ChessMain.o ChessPiece.o ChessBoard.o King.o Queen.o Bishop.o Knight.o Rook.o Pawn.o -o chess

ChessMain.o: ChessMain.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) ChessMain.cpp -c
ChessPiece.o: ChessPiece.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) ChessPiece.cpp -c
ChessBoard.o: ChessBoard.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) ChessBoard.cpp -c
King.o: King.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) King.cpp -c
Queen.o: Queen.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) Queen.cpp -c
Bishop.o: Bishop.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) Bishop.cpp -c
Knight.o: Knight.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) Knight.cpp -c
Rook.o: Rook.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) Rook.cpp -c
Pawn.o: Pawn.cpp $(HEADER)
	$(COMPILER) $(OPTIONS) Pawn.cpp -c