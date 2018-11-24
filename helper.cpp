//
//  helper.cpp
//  Chess
//
//  Created by Bryan Wong on 22/11/2018.
//  Copyright © 2018 Bryan Wong. All rights reserved.
//

#include <iostream>
#include "helper.hpp"
using namespace std;

int pos_to_int(char const* pos) {
    int val=(pos[0]-'A'+1)*10;
    val+=(pos[1]-'0');
    return val;
}

void int_to_pos(int pos, char* position) {
    position[0] = pos/10-1+'A';
    position[1] = pos%10+'0';
    position[2] = '\0';
    return;
}

int rank_index(int pos) {
  return pos%10-1;
}

int rank_index(char const* position) {
  return position[1]-'1';
}

int file_index(int pos) {
  return pos/10-1;
}

int file_index(char const* position) {
  return position[0]-'A';
}

int index_to_int(int file, int rank) {
  return (rank+1)+(file+1)*10;
}
