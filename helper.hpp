#ifndef helper_hpp
#define helper_hpp

int const MAX_FILE=8;
int const MAX_RANK=8;

enum Team {black, white};

int pos_to_int(char const* pos);
void int_to_pos(int pos, char* position);
int rank_index(int pos);
int rank_index(char const* position);
int file_index(int pos);
int file_index(char const* position);
int index_to_int(int file, int rank);

#endif /* helper_hpp */
