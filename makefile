chess: main.o chesspiece.o chessboard.o
	g++ -g -Wall main.o chesspiece.o chessboard.o -o chess
chesspiece.o: chesspiece.cpp errors.h helper.hpp chessboard.hpp chesspiece.hpp
	g++ -g -Wall chesspiece.cpp -c
chessboard.o: chessboard.cpp errors.h helper.hpp chessboard.hpp chesspiece.hpp
	g++ -g -Wall chessboard.cpp -c
main.o: main.cpp errors.h helper.hpp chessboard.hpp chesspiece.hpp
	g++ -g -Wall main.cpp -c
