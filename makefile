chess: ChessMain.o chesspiece.o chessboard.o
	g++ -g -Wall main.o chesspiece.o chessboard.o -o chess
chesspiece.o: chesspiece.cpp errors.h helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall chesspiece.cpp -c
chessboard.o: chessboard.cpp errors.h helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall ChessBoard.cpp -c
ChessMain.o: ChessMain.cpp errors.h helper.hpp ChessBoard.h chesspiece.hpp
	g++ -g -Wall ChessMain.cpp -c
