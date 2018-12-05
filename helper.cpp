#include <iostream>
#include "helper.hpp"
using namespace std;

int pos_to_int(char const* pos) {
  int val=(pos[0]-'A'+1)*10;
  val+=(pos[1]-'0');
  return val;
}

int rIndex(int pos) {
  return pos%10-1;
}

int rIndex(char const* position) {
  return position[1]-'1';
}

int fIndex(int pos) {
  return pos/10-1;
}

int fIndex(char const* position) {
  return position[0]-'A';
}

int index_to_int(int file, int rank) {
  return (rank+1)+(file+1)*10;
}

