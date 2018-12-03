chess: ChessMain.o chesspiece.o chessboard.o
	g++ -g -Wall main.o chesspiece.o chessboard.o -o chess
chesspiece.o: chesspiece.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall chesspiece.cpp -c
chessboard.o: chessboard.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall ChessBoard.cpp -c
ChessMain.o: ChessMain.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall ChessMain.cpp -c
