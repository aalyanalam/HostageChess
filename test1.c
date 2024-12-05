#include "hclib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_newboard() {
    exboard_t *board = newboard();
    if (board == NULL) {
        printf("test_newboard: FAILED (board is NULL)\n");
        return;
    }

    // Check initial positions of pawns
    for (int i = 0; i < 8; i++) {
        if (board->board[1][i] != 'P' || board->board[6][i] != 'p') {
            printf("test_newboard: FAILED (pawns not in correct positions)\n");
            free(board);
            return;
        }
    }

    // Check initial positions of other pieces
    if (board->board[0][0] != 'R' || board->board[7][0] != 'r' ||
        board->board[0][1] != 'N' || board->board[7][1] != 'n' ||
        board->board[0][2] != 'B' || board->board[7][2] != 'b' ||
        board->board[0][3] != 'Q' || board->board[7][3] != 'q' ||
        board->board[0][4] != 'K' || board->board[7][4] != 'k' ||
        board->board[0][5] != 'B' || board->board[7][5] != 'b' ||
        board->board[0][6] != 'N' || board->board[7][6] != 'n' ||
        board->board[0][7] != 'R' || board->board[7][7] != 'r') {
        printf("test_newboard: FAILED (pieces not in correct positions)\n");
        free(board);
        return;
    }

    printf("test_newboard: PASSED\n");
    free(board);
}

void test_copyboard() {
    exboard_t *board = newboard();
    exboard_t *copy = copyboard(board);

    if (copy == NULL) {
        printf("test_copyboard: FAILED (copy is NULL)\n");
        free(board);
        return;
    }

    if (memcmp(board, copy, sizeof(exboard_t)) != 0) {
        printf("test_copyboard: FAILED (boards do not match)\n");
        free(board);
        free(copy);
        return;
    }

    printf("test_copyboard: PASSED\n");
    free(board);
    free(copy);
}

void test_stringboard() {
    exboard_t *board = newboard();
    char *stringified = stringboard(board);

    if (stringified == NULL) {
        printf("test_stringboard: FAILED (stringified is NULL)\n");
        free(board);
        return;
    }

    // Check if the stringified board has the correct length
    if (strlen(stringified) != 161) {
        printf("test_stringboard: FAILED (incorrect string length)\n");
        free(board);
        free(stringified);
        return;
    }

    printf("%s\n", stringified);
    printf("test_stringboard: PASSED\n");
    free(board);
    free(stringified);
}

void test_boardstring() {
  exboard_t *board = newboard();
  board->bprison[0] = 'Q';
  board->bprison[1] = 'K';
  board->bprison[2] = '\0';

  board->bairfield[0] = 'n';
  board->bairfield[1] = 'r';
  board->bairfield[2] = '\0';

  board->wprison[0] = 'b';
  board->wprison[1] = 'p';
  board->wprison[2] = '\0';

  board->wairfield[0] = 'N';
  board->wairfield[1] = 'Q';
  board->wairfield[2] = '\0';

  board->board[1][3] = ' ';
  board->board[3][3] = 'P';

  board->board[0][3] = ' ';
  board->board[2][5] = 'Q';

  board->board[7][1] = ' ';
  board->board[5][0] = 'n';

  board->board[0][4] = ' ';
  board->board[0][3] = 'K';

  board->board[6][7] = ' ';
  board->board[5][7] = 'p';

  char *stringified = stringboard(board);
  exboard_t *backToBoard = boardstring(stringified);

  if (backToBoard == NULL) {
    printf("test_boardstring: FAILED (backToBoard is NULL)\n");
    free(board);
    free(stringified);
    free(backToBoard);
    return;
  }

  else
  {
    printf("Black Prison: ");
    for(int i = 0; i < 16 && backToBoard->bprison[i] != '\0'; i++)
    {
        printf("%d: ", i);
        printf("%c | ", backToBoard->bprison[i]);
    }
    printf("\n");
    printf("Black Airfield: ");
    for(int i = 0; i < 16 && backToBoard->bairfield[i] != '\0'; i++)
    {
        printf("%d: ", i);
        printf("%c | ", backToBoard->bairfield[i]);
    }
    printf("\n");
    printf("White Prison: ");
    for(int i = 0; i < 16 && backToBoard->wprison[i] != '\0'; i++)
    {
        printf("%d: ", i);
        printf("%c | ", backToBoard->wprison[i]);
    }
    printf("\n");
    printf("White Airfield: ");
    for(int i = 0; i < 16 && backToBoard->wairfield[i] != '\0'; i++)
    {
        printf("%d: ", i);
        printf("%c | ", backToBoard->wairfield[i]);
    }
    printf("\n");
    printf("Board (black should be on top): \n");
    for (int i=7; i>=0; i--)
    {
        printf("%c", backToBoard->board[i][0]);
        printf("%c", backToBoard->board[i][1]);
        printf("%c", backToBoard->board[i][2]);
        printf("%c", backToBoard->board[i][3]);
        printf("%c", backToBoard->board[i][4]);
        printf("%c", backToBoard->board[i][5]);
        printf("%c", backToBoard->board[i][6]);
        printf("%c", backToBoard->board[i][7]);
        printf("\n");
    }
  }

  printf("test_boardstring: PASSED\n");
  free(board);
  free(stringified);
  free(backToBoard);
}


void test_fen() {
    exboard_t *board = newboard();
    if (board == NULL) {
        printf("test_fen: FAILED (board is NULL)\n");
        return;
    }

    // Set up additional board state
    board->board[3][4] = 'P'; // White pawn on e4
    board->board[4][4] = 'p'; // Black pawn on e5

    char *active = "w";
    char *castling = "KQkq";
    char *enpassant = "-";
    int half = 0;
    int full = 1;

    char *fen_string = fen(board, active, castling, enpassant, half, full);
    if (fen_string == NULL) {
        printf("test_fen: FAILED (fen_string is NULL)\n");
        free(board);
        return;
    }

    // Expected FEN string for the given board state
    char *expected_fen = "rnbqkbnr/pppppppp/8/4p3/4P3/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    if (strcmp(fen_string, expected_fen) != 0) {
        printf("test_fen: FAILED (FEN string does not match expected)\n");
        printf("Expected: %s\n", expected_fen);
        printf("Got: %s\n", fen_string);
        free(board);
        free(fen_string);
        return;
    }

    printf("test_fen: PASSED\n");
    printf("Got: %s\n", fen_string);
    free(board);
    free(fen_string);
}

void test_apply_move_basic() {
    exboard_t *board = newboard();
    move_t move = {1, 0, 3, 0, ' ', ' '}; // Move white pawn from a2 to a4

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_basic: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[3][0] != 'P' || new_board->board[1][0] != ' ') {
        printf("test_apply_move_basic: FAILED (piece not moved correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_basic: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_capture() {
    exboard_t *board = newboard();
    move_t move = {1, 0, 6, 0, ' ', ' '}; // Move white pawn from a2 to a7 (capture black pawn)

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_capture: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[6][0] != 'P' || new_board->board[1][0] != ' ' || new_board->wprison[0] != 'p') {
        printf("test_apply_move_capture: FAILED (capture not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_capture: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_promotion() {
    exboard_t *board = newboard();
    board->board[6][0] = ' '; // Clear initial position of black pawn
    board->board[1][0] = ' '; // Clear initial position of white pawn
    board->board[6][0] = 'P'; // Place white pawn at a7
    move_t move = {6, 0, 7, 0, 'Q', ' '}; // Promote white pawn at a7 to queen

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_promotion: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[7][0] != 'Q' || new_board->board[6][0] != ' ') {
        printf("test_apply_move_promotion: FAILED (promotion not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_promotion: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_invalid() {
    exboard_t *board = newboard();
    move_t move = {3, 0, 4, 0, ' ', ' '}; // Attempt to move from an empty square

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_invalid: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (memcmp(board, new_board, sizeof(exboard_t)) != 0) {
        printf("test_apply_move_invalid: FAILED (board should not change)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_invalid: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_castling_white_kingside() {
    exboard_t *board = newboard();
    board->board[0][5] = ' '; // Clear f1
    board->board[0][6] = ' '; // Clear g1
    move_t move = {0, 4, 0, 6, ' ', ' '}; // White kingside castling

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_castling_white_kingside: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[0][6] != 'K' || new_board->board[0][5] != 'R' || new_board->board[0][4] != ' ' || new_board->board[0][7] != ' ') {
        printf("test_apply_move_castling_white_kingside: FAILED (castling not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_castling_white_kingside: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_castling_white_queenside() {
    exboard_t *board = newboard();
    board->board[0][1] = ' '; // Clear b1
    board->board[0][2] = ' '; // Clear c1
    board->board[0][3] = ' '; // Clear d1
    move_t move = {0, 4, 0, 2, ' ', ' '}; // White queenside castling

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_castling_white_queenside: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[0][2] != 'K' || new_board->board[0][3] != 'R' || new_board->board[0][4] != ' ' || new_board->board[0][0] != ' ') {
        printf("test_apply_move_castling_white_queenside: FAILED (castling not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_castling_white_queenside: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_castling_black_kingside() {
    exboard_t *board = newboard();
    board->board[7][5] = ' '; // Clear f8
    board->board[7][6] = ' '; // Clear g8
    move_t move = {7, 4, 7, 6, ' ', ' '}; // Black kingside castling

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_castling_black_kingside: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[7][6] != 'k' || new_board->board[7][5] != 'r' || new_board->board[7][4] != ' ' || new_board->board[7][7] != ' ') {
        printf("test_apply_move_castling_black_kingside: FAILED (castling not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_castling_black_kingside: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_castling_black_queenside() {
    exboard_t *board = newboard();
    board->board[7][1] = ' '; // Clear b8
    board->board[7][2] = ' '; // Clear c8
    board->board[7][3] = ' '; // Clear d8
    move_t move = {7, 4, 7, 2, ' ', ' '}; // Black queenside castling

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_castling_black_queenside: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[7][2] != 'k' || new_board->board[7][3] != 'r' || new_board->board[7][4] != ' ' || new_board->board[7][0] != ' ') {
        printf("test_apply_move_castling_black_queenside: FAILED (castling not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_castling_black_queenside: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_en_passant_white() {
    exboard_t *board = newboard();
    board->board[4][1] = 'p'; // Place black pawn at b5
    board->board[4][2] = 'P'; // Place white pawn at d5
    move_t move = {4, 2, 5, 1, ' ', ' '}; // White pawn captures black pawn en passant

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_en_passant_white: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[5][1] != 'P' || new_board->board[4][2] != ' ' || new_board->board[4][1] != ' ' || new_board->wprison[0] != 'p') {
        printf("test_apply_move_en_passant_white: FAILED (en passant not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_en_passant_white: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_en_passant_black() {
    exboard_t *board = newboard();
    board->board[3][1] = 'P'; // Place white pawn at b4
    board->board[3][2] = 'p'; // Place black pawn at d4
    move_t move = {3, 2, 2, 1, ' ', ' '}; // Black pawn captures white pawn en passant

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_en_passant_black: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[2][1] != 'p' || new_board->board[3][2] != ' ' || new_board->board[3][1] != ' ' || new_board->bprison[0] != 'P') {
        printf("test_apply_move_en_passant_black: FAILED (en passant not handled correctly)\n");
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_en_passant_black: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_drop_from_white_airfield() {
    exboard_t *board = newboard();
    board->wairfield[0] = 'B'; // Place a knight in the white airfield
    board->wairfield[1] = 'N'; // Place a knight in the white airfield
    board->wairfield[2] = 'R'; // Place a knight in the white airfield
    move_t move = {-1, 0, 4, 4, 'N', ' '}; // Drop white knight from airfield to e5

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_drop_from_white_airfield: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[4][4] != 'N' || new_board->wairfield[2] != '\0') {
        printf("test_apply_move_drop_from_white_airfield: FAILED (piece not dropped correctly)\n");
        printf("%s\n", stringboard(new_board));
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_drop_from_white_airfield: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_drop_from_black_airfield() {
    exboard_t *board = newboard();
    board->bairfield[0] = 'b'; // Place a knight in the black airfield
    board->bairfield[1] = 'n'; // Place a knight in the black airfield
    board->bairfield[2] = 'r'; // Place a knight in the black airfield
    move_t move = {8, 0, 3, 3, 'n', ' '}; // Drop black knight from airfield to d4

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_drop_from_black_airfield: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[3][3] != 'n' || new_board->bairfield[2] != '\0') {
        printf("test_apply_move_drop_from_black_airfield: FAILED (piece not dropped correctly)\n");
        printf("%s\n", stringboard(new_board));
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_drop_from_black_airfield: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_drop_from_black_prison() {
    exboard_t *board = newboard();
    board->bprison[0] = 'B'; // Place a bishop in the black prison
    board->bprison[1] = 'N'; // Place a knight in the black prison
    board->bprison[2] = 'R'; // Place a rook in the black prison
    board->wprison[0] = 'q'; // Place a queen in the white prison
    move_t move = {9, 0, 3, 3, 'N', 'q'}; // Drop black knight from prison to d4

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_drop_from_black_prison: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[3][3] != 'N' || new_board->bprison[2] != '\0' || new_board->bairfield[0] != 'q') {
        printf("test_apply_move_drop_from_black_prison: FAILED (piece not dropped correctly)\n");
        printf("%s\n", stringboard(new_board));
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_drop_from_black_prison: PASSED\n");
    free(board);
    free(new_board);
}

void test_apply_move_drop_from_white_prison() {
    exboard_t *board = newboard();
    board->wprison[0] = 'b'; // Place a bishop in the black prison
    board->wprison[1] = 'n'; // Place a knight in the black prison
    board->wprison[2] = 'r'; // Place a rook in the black prison
    board->bprison[0] = 'Q'; // Place a queen in the white prison
    move_t move = {-2, 0, 3, 3, 'n', 'Q'}; // Drop black knight from prison to d4

    exboard_t *new_board = apply_move(board, &move);
    if (new_board == NULL) {
        printf("test_apply_move_drop_from_white_prison: FAILED (new_board is NULL)\n");
        free(board);
        return;
    }

    if (new_board->board[3][3] != 'n' || new_board->wprison[2] != '\0' || new_board->wairfield[0] != 'Q') {
        printf("test_apply_move_drop_from_white_prison: FAILED (piece not dropped correctly)\n");
        printf("%s\n", stringboard(new_board));
        free(board);
        free(new_board);
        return;
    }

    printf("test_apply_move_drop_from_white_prison: PASSED\n");
    free(board);
    free(new_board);
}

void test_king_moves() {
    exboard_t *board = newboard();
    board->board[3][0] = 'k'; // Place white king at e1
    if (!board) {
        printf("test_king_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test king moves
    move_t **king_moves = moves(&board->board, 3, 0);
    if (king_moves == NULL) {
        printf("test_king_moves: FAILED (king moves is NULL)\n");
        free(board);
        return;
    }

    // Print the moves
    for (int i = 0; king_moves[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", king_moves[i]->from_i, king_moves[i]->from_j, king_moves[i]->to_i, king_moves[i]->to_j);
    }

    printf("test_king_moves: PASSED\n");
    for (int i = 0; king_moves[i] != NULL; i++) {
        free(king_moves[i]);
    }
    free(king_moves);
    free(board);
}

void test_knight_moves() {
    exboard_t *board = newboard();
    board->board[0][1] = 'N'; // Place white king at e1
    if (!board) {
        printf("test_knight_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test king moves
    move_t **knight_moves = moves(&board->board, 0, 1);
    if (knight_moves == NULL) {
        printf("test_knight_moves: FAILED (king moves is NULL)\n");
        free(board);
        return;
    }

    // Print the moves
    for (int i = 0; knight_moves[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", knight_moves[i]->from_i, knight_moves[i]->from_j, knight_moves[i]->to_i, knight_moves[i]->to_j);
    }

    printf("test_knight_moves: PASSED\n");
    for (int i = 0; knight_moves[i] != NULL; i++) {
        free(knight_moves[i]);
    }
    free(knight_moves);
    free(board);
}

void test_bishop_moves() {
    exboard_t *board = newboard();
    board->board[7][5] = 'B'; // Place white king at e
    if (!board) {
        printf("test_bishop_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test king moves
    move_t **bishop_moves = moves(&board->board, 7, 5);
    if (bishop_moves == NULL) {
        printf("test_bishop_moves: FAILED (king moves is NULL)\n");
        free(board);
        return;
    }

    // Print the moves
    for (int i = 0; bishop_moves[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", bishop_moves[i]->from_i, bishop_moves[i]->from_j, bishop_moves[i]->to_i, bishop_moves[i]->to_j);
    }

    printf("test_bishop_moves: PASSED\n");
    for (int i = 0; bishop_moves[i] != NULL; i++) {
        free(bishop_moves[i]);
    }
    free(bishop_moves);
    free(board);
}

void test_rook_moves() {
    exboard_t *board = newboard();
    board->board[0][0] = 'R'; // Place white king at e1
    if (!board) {
        printf("test_rook_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test king moves
    move_t **rook_moves = moves(&board->board, 0, 0);
    if (rook_moves == NULL) {
        printf("test_rook_moves: FAILED (king moves is NULL)\n");
        free(board);
        return;
    }

    // Print the moves
    for (int i = 0; rook_moves[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", rook_moves[i]->from_i, rook_moves[i]->from_j, rook_moves[i]->to_i, rook_moves[i]->to_j);
    }

    printf("test_rook_moves: PASSED\n");
    for (int i = 0; rook_moves[i] != NULL; i++) {
        free(rook_moves[i]);
    }
    free(rook_moves);
    free(board);
}

void test_queen_moves() {
    exboard_t *board = newboard();
    board->board[4][2] = 'q'; // Place white king at e1
    board->board[3][1] = 'Q'; // Place white king at e1
    if (!board) {
        printf("test_queen_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test king moves
    move_t **queen_moves = moves(&board->board, 4, 2);
    if (queen_moves == NULL) {
        printf("test_queen_moves: FAILED (queen moves is NULL)\n");
        free(board);
        return;
    }

    // Print the moves
    for (int i = 0; queen_moves[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", queen_moves[i]->from_i, queen_moves[i]->from_j, queen_moves[i]->to_i, queen_moves[i]->to_j);
    }

    printf("test_queen_moves: PASSED\n");
    for (int i = 0; queen_moves[i] != NULL; i++) {
        free(queen_moves[i]);
    }
    free(queen_moves);
    free(board);
}

void test_pawn_moves() {
    exboard_t *board = newboard();
    
    // Test a white pawn at its initial position
    board->board[1][5] = 'P'; // Place white pawn at d2
    // board->board[4][6] = 'B';
    // board->board[4][4] = 'r';
    // board->board[2][6] = 'k';
    // board->board[2][4] = 'Q';
    // board->board[2][5] = 'P';
    if (!board) {
        printf("test_pawn_moves: FAILED (board is NULL)\n");
        return;
    }

    // Test white pawn moves from d2
    move_t **pawn_moves_white = moves(&board->board, 1, 5); // White pawn at d2
    if (pawn_moves_white == NULL) {
        printf("test_pawn_moves: FAILED (white pawn moves is NULL)\n");
        free(board);
        return;
    }

    // Print white pawn moves
    printf("White pawn moves from d2:\n");
    for (int i = 0; pawn_moves_white[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", pawn_moves_white[i]->from_i, pawn_moves_white[i]->from_j, pawn_moves_white[i]->to_i, pawn_moves_white[i]->to_j);
    }

    // Free white pawn moves
    for (int i = 0; pawn_moves_white[i] != NULL; i++) {
        free(pawn_moves_white[i]);
    }
    free(pawn_moves_white);

    // // Test a black pawn at its initial position
    board->board[6][2] = 'p'; // Place black pawn at d7
    // board->board[5][3] = 'B';
    // board->board[5][1] = 'n';

    // Test black pawn moves from d7
    move_t **pawn_moves_black = moves(&board->board, 6, 2); // Black pawn at d7
    if (pawn_moves_black == NULL) {
        printf("test_pawn_moves: FAILED (black pawn moves is NULL)\n");
        free(board);
        return;
    }

    // Print black pawn moves
    printf("Black pawn moves from d7:\n");
    for (int i = 0; pawn_moves_black[i] != NULL; i++) {
        printf("from: %d %d, to: %d %d\n", pawn_moves_black[i]->from_i, pawn_moves_black[i]->from_j, pawn_moves_black[i]->to_i, pawn_moves_black[i]->to_j);
    }

    // Free black pawn moves
    for (int i = 0; pawn_moves_black[i] != NULL; i++) {
        free(pawn_moves_black[i]);
    }
    free(pawn_moves_black);
    
    // Cleanup
    free(board);
}


// Add the test_king_moves function call to main
int main(int argc, char **argv) {
    test_newboard();
    test_copyboard();
    test_stringboard();
    test_boardstring();
    // test_boardstring2();
    test_fen();
    test_apply_move_basic();
    test_apply_move_capture();
    test_apply_move_promotion();
    test_apply_move_invalid();
    test_apply_move_castling_white_kingside();
    test_apply_move_castling_white_queenside();
    test_apply_move_castling_black_kingside();
    test_apply_move_castling_black_queenside();
    test_apply_move_en_passant_white();
    test_apply_move_en_passant_black();
    test_apply_move_drop_from_white_airfield();
    test_apply_move_drop_from_black_airfield();
    test_apply_move_drop_from_black_prison();
    test_apply_move_drop_from_white_prison();
    test_king_moves();
    test_knight_moves();
    test_bishop_moves();
    test_rook_moves();
    test_queen_moves();
    test_pawn_moves();
    return 0;
}
