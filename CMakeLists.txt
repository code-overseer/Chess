cmake_minimum_required(VERSION 3.14)
project(TripleTriad)
set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(include)

add_executable(TripleTriad
        main.cpp
        include/Knight.h
        include/Bishop.h
        include/Rook.h
        include/Queen.h
        include/King.h
        include/helper.h
        include/ChessBoard.h
        include/ChessPiece.h
        src/Knight.cpp
        src/Bishop.cpp
        src/Position.cpp
        src/Rook.cpp
        src/Queen.cpp
        src/King.cpp
        src/ChessBoard.cpp
        src/ChessPiece.cpp)