cmake_minimum_required(VERSION 3.14)
project(TripleTriad)
set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(include)

add_executable(TripleTriad
        main.cpp
        include/Knight.hpp
        include/Bishop.hpp
        include/Rook.hpp
        include/Pawn.hpp
        include/Queen.hpp
        include/King.hpp
        include/helper.hpp
        include/ChessBoard.hpp
        include/Chesspiece.hpp
        include/Tests.hpp
        src/Knight.cpp
        src/Bishop.cpp
        src/Rook.cpp
        src/Pawn.cpp
        src/Queen.cpp
        src/King.cpp
        src/ChessBoard.cpp
        src/Chesspiece.cpp
        tests/Tests.cpp)