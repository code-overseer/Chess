chess: ChessMain.o chesspiece.o chessboard.o helper.o
	g++ -g -Wall ChessMain.o chesspiece.o chessboard.o -o chess
chesspiece.o: chesspiece.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall chesspiece.cpp -c
chessboard.o: chessboard.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall chessboard.cpp -c
ChessMain.o: ChessMain.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall ChessMain.cpp -c
helper.o: helper.cpp helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall helper.cpp -c
