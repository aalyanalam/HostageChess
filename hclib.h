#define hclib_h

//Board definition
typedef char board_t[8][8];

//Extended board struct
typedef struct
{
char bprison[16]; //Black pieces' prison
char bairfield[16]; //Black pieces' airfield
board_t board; //Game board
char wprison[16]; //White pieces' prison
char wairfield[16]; //White pieces' airfield
} exboard_t;

//Board movement struct
typedef struct
{
int from_i, from_j; //Starting spot of a piece
int to_i, to_j; //End spot of piece
char promotion; //Promotion piece
char hostage; //Hostage piece
} move_t;

//Prototype functions

//New game board function (returns a pointer to an exboard_t)
exboard_t* newboard();

//Copy the existing board (returns a pointer to a new exboard_t)
exboard_t* copyboard(exboard_t* board);

//Make string version of board
char* stringboard(exboard_t* board);

//Apply a move to the board (returns pointer to updated board)
exboard_t* apply_move(exboard_t* board, move_t* move);

//Return possible moves for a piece at given spot
move_t **moves( board_t *board, int from_i, int from_j);

//Specific move functions for each piece type
move_t **knightmoves(board_t *board, int from_i, int from_j, int colour);
move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour);
move_t **rookmoves(board_t *board, int from_i, int from_j, int colour);
move_t **queenmoves(board_t *board, int from_i, int from_j, int colour);
move_t **king_moves(board_t *board, int from_i, int from_j, int colour);
move_t **pawn_moves(board_t *board, int from_i, int from_j, int colour);
