#include "hclib.h"
#include <stdio.h>
#include <stdlib.h>  // For malloc and free
#include <string.h>  // For memcpy (used in copyboard)
#include <ctype.h>

exboard_t* newboard() 
{
    exboard_t *board = (exboard_t *)malloc(sizeof(exboard_t));
    if (board == NULL) 
    {
        return NULL; // malloc failed
    }

    // Make initial board

    // White pieces ranks 1 and 2
    memcpy(board->board[0], "RNBQKBNR", 8);  // Use memcpy to copy exactly 8 characters
    memcpy(board->board[1], "PPPPPPPP", 8);
    
    // Black pieces ranks 7 and 8
    memcpy(board->board[7], "rnbqkbnr", 8);
    memcpy(board->board[6], "pppppppp", 8);

    // Empty spaces in ranks 3 to 6
    for (int i = 2; i <= 5; i++) 
    {
        memset(board->board[i], ' ', 8);  // Fill 8 spaces in each row
    }

    memset(board->bprison, 0, sizeof(board->bprison));
    memset(board->bairfield, 0, sizeof(board->bairfield));
    memset(board->wprison, 0, sizeof(board->wprison));
    memset(board->wairfield, 0, sizeof(board->wairfield));

    // Prisons and airfields initially
    board->bprison[0] = '\0';
    board->wprison[0] = '\0';
    board->bairfield[0] = '\0';
    board->wairfield[0] = '\0';

    return board;
}



exboard_t* copyboard(exboard_t* board) 
{
    if (board == NULL) 
    {
        return NULL;
    }

    exboard_t *newBoard = (exboard_t *)malloc(sizeof(exboard_t));
    if (newBoard == NULL) 
    {
        return NULL;
    }

    // Copy existing board to new one
    memcpy(newBoard, board, sizeof(exboard_t));

    return newBoard;
}

char* stringboard(exboard_t* board) 
{
    if (board == NULL) 
    {
        return NULL;
    }

    char *boardStr = malloc(sizeof(char) *162);
    if (boardStr == NULL) 
    {
        return NULL;
    }

    // Initialize the string with spaces and newlines
    memset(boardStr, ' ', 162);
    for(int i = 8; i <= 161; i += 9) 
    {
        boardStr[i] = '\n';
    }
    boardStr[161] = '\0';  // Null terminator


    // Black piece prison
    for(int i = 0; i <= 7; i++) 
    {
        if(board->bprison[i] == '\0') break;
        boardStr[i] = board->bprison[i];
    }
    for(int i = 8; i <= 15; i++) 
    {
        if(board->bprison[i - 1] == '\0') break;
        boardStr[i + 1] = board->bprison[i - 1];  
    }

    // Black piece airfield
    for(int i = 0; i <= 7; i++) 
    {
        if(board->bairfield[i] == '\0') break;
        boardStr[i + 18] = board->bairfield[i];
    }
    for(int i = 0; i <= 7; i++) 
    {
        if(board->bairfield[i + 8] == '\0') break;
        boardStr[i + 27] = board->bairfield[i + 8];
    }

    // Dashes for separation
    for(int i = 36; i <= 43; i++) 
    {
        boardStr[i] = '-';
    }

    // Copy the board pieces from the board array
    for(int i = 0; i <= 7; i++) 
    {
        boardStr[i + 45] = board->board[7][i];
        boardStr[i + 54] = board->board[6][i];
        boardStr[i + 63] = board->board[5][i];
        boardStr[i + 72] = board->board[4][i];
        boardStr[i + 81] = board->board[3][i];
        boardStr[i + 90] = board->board[2][i];
        boardStr[i + 99] = board->board[1][i];
        boardStr[i + 108] = board->board[0][i];
    }

    // Dashes for separation
    for(int i = 117; i <= 124; i++) 
    {
        boardStr[i] = '-';
    }

    // White piece airfield
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wairfield[i] == '\0') break;
        boardStr[i + 126] = board->wairfield[i];
    }
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wairfield[i + 8] == '\0') break;
        boardStr[i + 135] = board->wairfield[i + 8];
    }

    // White piece prison
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wprison[i] == '\0') break;
        boardStr[i + 144] = board->wprison[i];
    }
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wprison[i + 8] == '\0') break;
        boardStr[i + 153] = board->wprison[i + 8];
    }

    return boardStr;  // Ensure the caller frees this after use
}


exboard_t* boardstring(char *boardStr) 
{
    if (boardStr == NULL) 
    {
        return NULL;
    }

    exboard_t *board = (exboard_t *)malloc(sizeof(exboard_t));
    if (board == NULL) 
    {
        return NULL; // malloc failed
    }

    // Initialize board fields to empty
    memset(board, 0, sizeof(exboard_t));

    // Black piece prison (indices 0-7 and 9-15 in the string)
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i] == ' ') break;  // Stop at space
        board->bprison[i] = boardStr[i];
    }
    for (int i = 9; i <= 15; i++) {
        if (boardStr[i + 1] == ' ') break;  // Stop at space
        board->bprison[i - 1] = boardStr[i + 1];
    }

    // Black piece airfield (indices 18-25 and 27-34 in the string)
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 18] == ' ') break;  // Stop at space
        board->bairfield[i] = boardStr[i + 18];
    }
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 27] == ' ') break;  // Stop at space
        board->bairfield[i + 8] = boardStr[i + 27];
    }

    // Reconstruct the board (starting at index 45)
    for (int i = 0; i <= 7; i++) 
    {
        // Rank 7 (index 45) to Rank 0 (index 108)
        for (int file = 0; file < 8; file++) 
        {
            board->board[7 - i][file] = boardStr[45 + i * 9 + file];  // Each row is separated by a newline (9 chars total)
        }
    }

    // White piece airfield (indices 126-133 and 135-142 in the string)
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 126] == ' ') break;  // Stop at space
        board->wairfield[i] = boardStr[i + 126];
    }
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 135] == ' ') break;  // Stop at space
        board->wairfield[i + 8] = boardStr[i + 135];
    }

    // White piece prison (indices 144-151 and 153-160 in the string)
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 144] == ' ') break;  // Stop at space
        board->wprison[i] = boardStr[i + 144];
    }
    for (int i = 0; i <= 7; i++) {
        if (boardStr[i + 153] == ' ') break;  // Stop at space
        board->wprison[i + 8] = boardStr[i + 153];
    }

    return board;  // Ensure the caller frees this after use
}


char *fen(exboard_t *board, char *active, char *castling, char *enpassant, int half, int full) 
{
    if (board == NULL || active == NULL || castling == NULL || enpassant == NULL) 
    {
        return NULL;
    }

    char fenBoard[72];  //Maxpossible size for the board part of FEN (with slashes and digits)
    int fenIndex = 0;

    //Convert the board to FEN
    for (int rank = 7; rank >= 0; rank--) 
    {
        int emptyCount = 0;
        for (int file = 0; file < 8; file++) 
        {
            char piece = board->board[rank][file];
            if (piece == ' ') 
            {
                emptyCount++;
            } 
            else 
            {
                if (emptyCount > 0) 
                {
                    fenBoard[fenIndex++] = '0' + emptyCount;  //Convert count of empty squares to number
                    emptyCount = 0;
                }
                fenBoard[fenIndex++] = piece;  //Place the piece character
            }
        }
        if (emptyCount > 0) 
        {
            fenBoard[fenIndex++] = '0' + emptyCount;  //Add trailing empty squares if needed
        }
        if (rank > 0) 
        {
            fenBoard[fenIndex++] = '/';  //Add row separator except for last row
        }
    }
    fenBoard[fenIndex] = '\0';  // Null-terminate the board part

    //Calculate required size for the full FEN string
    int totalLength = strlen(fenBoard) + 1  // Board part + space
                      + strlen(active) + 1  // Active player + space
                      + strlen(castling) + 1  // Castling rights + space
                      + strlen(enpassant) + 1  // En passant + space
                      + snprintf(NULL, 0, "%d", half) + 1  // Halfmove clock + space
                      + snprintf(NULL, 0, "%d", full) + 1;  // Fullmove number + null terminator

    // Allocate memory for the final FEN string
    char *fenStr = (char *)malloc(totalLength);
    if (fenStr == NULL) 
    {
        return NULL;  // malloc failed
    }

    // Construct final FEN string
    snprintf(fenStr, totalLength, "%s %s %s %s %d %d", fenBoard, active, castling, enpassant, half, full);

    return fenStr;
}



exboard_t* apply_move(exboard_t* board, move_t* move) 
{
    if (board == NULL || move == NULL) 
    {
        return NULL;
    }

    // Move piece from starting position to end position
    char piece = board->board[move->from_i][move->from_j];
    char possiblePiece = board->board[move->to_i][move->to_j];  // Check destination before clearing
    board->board[move->from_i][move->from_j] = ' ';  // Clear starting position

    if (move->promotion != ' ') 
    {
        // Handle promotion if necessary
        board->board[move->to_i][move->to_j] = move->promotion;
    } 
    else 
    {
        board->board[move->to_i][move->to_j] = piece;  // Move piece
    }

    // Now check for a captured piece (only if it's of the opposite colour)
    if (possiblePiece != ' ') 
    {
        if ((piece >= 'A' && piece <= 'Z') && (possiblePiece >= 'a' && possiblePiece <= 'z')) 
        {
            // White piece captures black piece
            strncat(board->wprison, &possiblePiece, 1);
        } 
        else if ((piece >= 'a' && piece <= 'z') && (possiblePiece >= 'A' && possiblePiece <= 'Z')) 
        {
            // Black piece captures white piece
            strncat(board->bprison, &possiblePiece, 1);
        }
    }

    // Handle castling
    if ((piece == 'K' || piece == 'k') && abs(move->from_j - move->to_j) > 1) 
    {
        // King moves two spaces horizontally (castling)
        if (move->to_j == 6) 
        {  // King-side castling
            board->board[move->to_i][5] = board->board[move->to_i][7];  // Move the rook
            board->board[move->to_i][7] = ' ';  // Clear the initial rook spot
        } 
        else if (move->to_j == 2) 
        {  // Queen-side castling
            board->board[move->to_i][3] = board->board[move->to_i][0];  // Move the rook
            board->board[move->to_i][0] = ' ';  // Clear the initial rook spot
        }
    }

    // Handle en passant
    if ((piece == 'P' || piece == 'p') && abs(move->from_i - move->to_i) == 1 && abs(move->from_j - move->to_j) == 1) 
    {
        // to_j position must be empty for en passant
        if (possiblePiece == ' ') 
        {
            // En passant logic: The captured pawn is not directly on the target square
            if (piece == 'P') 
            {
                // White pawn captures a black pawn en passant
                if (board->board[move->to_i - 1][move->to_j] == 'p') 
                {
                    board->board[move->to_i - 1][move->to_j] = ' ';  // Clear captured black pawn
                    strncat(board->wprison, "p", 1);  // Send black pawn to prison
                }
            } 
            else if (piece == 'p') 
            {
                // Black pawn captures a white pawn en passant
                if (board->board[move->to_i + 1][move->to_j] == 'P') 
                {
                    board->board[move->to_i + 1][move->to_j] = ' ';  // Clear the captured white pawn
                    strncat(board->bprison, "P", 1);  // Send white pawn to prison
                }
            }
        }
    }

    // Handle drops from airfield
    if (move->from_i == -1 || move->from_i == 8) 
    {  // Airfield drop
        char *airfield = (move->from_i == -1) ? board->wairfield : board->bairfield;
        // Find and remove the promotion piece from the airfield
        for (int i = 0; i < 16; i++) {
            if (airfield[i] == move->promotion) 
            {
                for (int j = i; j < 15; j++) 
                {
                    airfield[j] = airfield[j + 1];  // Shift characters left
                }
                airfield[15] = '\0';  // Null terminate the string
                break;
            }
        }
        // Place the promotion piece on the board
        board->board[move->to_i][move->to_j] = move->promotion;
    }

    // Handle drops from prison
    if (move->from_i == -2 || move->from_i == 9) 
    {  // Prison drop
        char *prison = (move->from_i == -2) ? board->wprison : board->bprison;
        // Find and remove the promotion piece from the prison
        for (int i = 0; i < 16; i++) 
        {
            if (prison[i] == move->promotion) 
            {
                for (int j = i; j < 15; j++) 
                {
                    prison[j] = prison[j + 1];  // Shift characters left
                }
                prison[15] = '\0';  // Null terminate the string
                break;
            }
        }

        // Place the promotion piece on the board
        board->board[move->to_i][move->to_j] = move->promotion;

        // Handle hostage exchange
        char *enemyPrison = (move->from_i == -2) ? board->bprison : board->wprison;
        char *yourAirfield = (move->from_i == -2) ? board->wairfield : board->bairfield;

        // Find and remove the hostage from enemy prison and move it to your airfield
        for (int i = 0; i < 16; i++) {
            if (enemyPrison[i] == move->hostage) 
            {
                for (int j = i; j < 15; j++) 
                {
                    enemyPrison[j] = enemyPrison[j + 1];  // Shift characters left
                }
                enemyPrison[15] = '\0';  // Null terminate the string

                // Move hostage to the airfield
                for (int j = 0; j < 16; j++) 
                {
                    if (yourAirfield[j] == '\0') 
                    {
                        yourAirfield[j] = move->hostage;
                        break;
                    }
                }
                break;
            }
        }
    }
    return board;
}

move_t **moves(board_t *board, int from_i, int from_j)
{
    char piece = (*board)[from_i][from_j]; //Piece we were looking at

    // Call the appropriate move function based on the piece
    if (piece == 'P' || piece == 'p') 
    {
        // Pawn
        if (piece == 'P') 
        {
            return pawn_moves(board, from_i, from_j, 0);
        }
        else if (piece == 'p') 
        {
            return pawn_moves(board, from_i, from_j, 1);
        }
    } 
    else if (piece == 'R' || piece == 'r') 
    {
        if (piece == 'R') 
        {
            return rookmoves(board, from_i, from_j, 0);
        }
        else if (piece == 'r') 
        {
            return rookmoves(board, from_i, from_j, 1);
        }
    } 
    else if (piece == 'N' || piece == 'n') 
    {
        // Knight
        if (piece == 'N') 
        {
            return knightmoves(board, from_i, from_j, 0);
        }
        else if (piece == 'n') 
        {
            return knightmoves(board, from_i, from_j, 1);
        }
    } 
    else if (piece == 'B' || piece == 'b') 
    {
        // Bishop
        if (piece == 'B') 
        {
            return bishopmoves(board, from_i, from_j, 0);
        }
        else if (piece == 'b') 
        {
            return bishopmoves(board, from_i, from_j, 1);
        }
    } 
    else if (piece == 'Q' || piece == 'q') 
    {
        // Queen
        if (piece == 'Q') 
        {
            return queenmoves(board, from_i, from_j, 0);
        }
        else if (piece == 'q') 
        {
            return queenmoves(board, from_i, from_j, 1);
        }
    } 
    else if (piece == 'K' || piece == 'k') 
    {
        // King
        if (piece == 'K') 
        {
            return king_moves(board, from_i, from_j, 0);
        }
        else if (piece == 'k') 
        {
            return king_moves(board, from_i, from_j, 1);
        }
    }

    // If the piece is not recognized, return NULL
    return NULL;
}

int validKnightKing(board_t *board, int to_i, int to_j, int colour)
{
    if (to_i < 0 || to_i >= 8 || to_j < 0 || to_j >= 8)
    {
        return 0; //Out of bounds
    }

    char destination = (*board)[to_i][to_j];

    if (destination == ' ')
    {
        return 1; //Spot is empty
    }
    if (colour == 0 && isupper(destination))
    {
        return 0; //Invalid
    }
    if (colour == 1 && islower(destination))
    {
        return 0; //Invalid
    }

    return 1; //Move is valid but destination is currently occupied by opposite colour

}

move_t *make_move(int from_i, int from_j, int to_i, int to_j) 
{
    move_t *new_move = malloc(sizeof(move_t));
    if (!new_move) return NULL;
    new_move->from_i = from_i;
    new_move->from_j = from_j;
    new_move->to_i = to_i;
    new_move->to_j = to_j;
    new_move->promotion = ' ';
    new_move->hostage = ' ';
    return new_move;
}

int is_valid_move(char destination, int colour) 
{
    if (destination == ' ') return 1;  // Empty space is valid
    if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination))) 
    {
        return 0;  //Friendly piece encountered invalid move
    }
    return 2;  //Move is valid but destination is currently occupied by opposite colour
}

move_t **knightmoves( board_t *board, int from_i, int from_j, int colour)
{
    int knightBaseMoves[8][2] = 
    {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    move_t **moves = malloc(9 * sizeof(move_t *));
    int numOfMoves = 0;

    for (int i = 0; i < 8; i++)
    {
        int to_i = from_i + knightBaseMoves[i][0];
        int to_j = from_j + knightBaseMoves[i][1];

        if (validKnightKing(board, to_i, to_j, colour))
        {
            moves[numOfMoves++] = make_move(from_i, from_j, to_i, to_j);
        }
    }

    //Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}

void add_bishop_moves(move_t ***moves, board_t *board, int from_i, int from_j, int direction_i, int direction_j, int *numOfMoves, int colour) 
{
    for (int to_i = from_i + direction_i, to_j = from_j + direction_j;
         to_i >= 0 && to_i < 8 && to_j >= 0 && to_j < 8;
         to_i += direction_i, to_j += direction_j) 
         {
        char destination = (*board)[to_i][to_j];
        int valid = is_valid_move(destination, colour);

        if (valid == 1) 
        {  // Valid move to empty space
            (*moves)[(*numOfMoves)++] = make_move(from_i, from_j, to_i, to_j);
        } else if (valid == 2) 
        {  // Valid move to capture enemy piece
            (*moves)[(*numOfMoves)++] = make_move(from_i, from_j, to_i, to_j);
            break;  // Stop after capturing
        } else 
        {
            break;  // Ran into friendly piece
        }
    }
}

void add_rook_moves(move_t ***moves, board_t *board, int from_i, int from_j, int delta_i, int delta_j, int *numOfMoves, int colour) 
{
    for (int to_i = from_i + delta_i, to_j = from_j + delta_j;
         to_i >= 0 && to_i < 8 && to_j >= 0 && to_j < 8;
         to_i += delta_i, to_j += delta_j) 
    {
        char destination = (*board)[to_i][to_j];
        int valid = is_valid_move(destination, colour);

        if (valid == 1) 
        {  
            (*moves)[(*numOfMoves)++] = make_move(from_i, from_j, to_i, to_j);
        } 
        else if (valid == 2) 
        {  
            (*moves)[(*numOfMoves)++] = make_move(from_i, from_j, to_i, to_j);
            break;  // Stop after capturing an enemy piece
        } 
        else 
        {
            break;  // Ran into a friendly piece
        }
    }
}

move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour) {
    move_t **moves = malloc(16 * sizeof(move_t *)); // Allocate max possible moves.
    int numOfMoves = 0;

    // Add moves for each diagonal direction.
    add_bishop_moves(&moves, board, from_i, from_j, -1, -1, &numOfMoves, colour);  // Up-left
    add_bishop_moves(&moves, board, from_i, from_j, -1, +1, &numOfMoves, colour);  // Up-right
    add_bishop_moves(&moves, board, from_i, from_j, +1, -1, &numOfMoves, colour);  // Down-left
    add_bishop_moves(&moves, board, from_i, from_j, +1, +1, &numOfMoves, colour);  // Down-right

    // Realloc to exact amount needed with NULL at the end.
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}


move_t **rookmoves(board_t *board, int from_i, int from_j, int colour)
{
    move_t **moves = malloc(16 * sizeof(move_t *)); // Allocate max possible moves + NULL pointer
    int numOfMoves = 0;

    // Add moves in all four directions (up, down, left, right)
    add_rook_moves(&moves, board, from_i, from_j, -1, 0, &numOfMoves, colour);  // Up
    add_rook_moves(&moves, board, from_i, from_j, 1, 0, &numOfMoves, colour);   // Down
    add_rook_moves(&moves, board, from_i, from_j, 0, -1, &numOfMoves, colour);  // Left
    add_rook_moves(&moves, board, from_i, from_j, 0, 1, &numOfMoves, colour);   // Right

    // Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}


move_t **queenmoves(board_t *board, int from_i, int from_j, int colour)
{
    move_t **moves = malloc(28 * sizeof(move_t *)); // Allocate max possible moves + NULL pointer
    int numOfMoves = 0;

    move_t **rookMoves = rookmoves(board, from_i, from_j, colour); // Get rook moves
    // Add rook moves to queen's moves list
    for (int i = 0; rookMoves[i] != NULL; i++) 
    {
        moves[numOfMoves++] = rookMoves[i];  //Add pointers to queen's moves
    }
    free(rookMoves);  // Free temporary array holding rook moves

    move_t **bishopMoves = bishopmoves(board, from_i, from_j, colour); // Get bishop moves
    // Append bishop moves to queen's moves list
    for (int i = 0; bishopMoves[i] != NULL; i++) 
    {
        moves[numOfMoves++] = bishopMoves[i];  //Add pointers to queen's moves
    }
    free(bishopMoves);  // Free temporary array holding bishop moves

    //Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}

move_t **king_moves(board_t *board, int from_i, int from_j, int colour)
{
    int kingBaseMoves[8][2] = 
    {
        {1, -1}, {1, 0}, {1, 1}, {0, -1},
        {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}
    };

    move_t **moves = malloc(9 * sizeof(move_t *));
    int numOfMoves = 0;

    for (int i = 0; i < 8; i++)
    {
        int to_i = from_i + kingBaseMoves[i][0];
        int to_j = from_j + kingBaseMoves[i][1];

        if (validKnightKing(board, to_i, to_j, colour))
        {
            moves[numOfMoves++] = make_move(from_i, from_j, to_i, to_j);
        }
    }

    //Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;   
}

move_t **pawn_moves(board_t *board, int from_i, int from_j, int colour)
{
    move_t **moves = malloc(5 * sizeof(move_t *));
    int numOfMoves = 0;
    int path;

    // Determine the movement path based on colour
    if (colour == 0) 
    {
        path = 1;  // White pawns move down (increase i index)
    }
    else
    {
        path = -1;   // Black pawns move up (decrease i index)
    }

    // Check if pawn can move forward 1 square
  
    if ((*board)[from_i + path][from_j] == ' ') 
    {
        moves[numOfMoves++] = make_move(from_i, from_j, from_i + path, from_j);

        // Check if pawn can move forward 2 squares (only if it is on starting rank)
        if ((colour == 0 && from_i == 1) || (colour == 1 && from_i == 6)) 
        {
            if ((*board)[from_i + 2 * path][from_j] == ' ') 
            {
                moves[numOfMoves++] = make_move(from_i, from_j, from_i + 2 * path, from_j);
            }
        }
    }

    // Check if pawn can capture diagonally (left)
    if (from_j - 1 >= 0) 
    {
        char captureLeft = (*board)[from_i + path][from_j - 1];
        if ((colour == 0 && islower(captureLeft)) || (colour == 1 && isupper(captureLeft))) 
        {
            moves[numOfMoves++] = make_move(from_i, from_j, from_i + path, from_j - 1);
        }
    }

    // Check if pawn can capture diagonally (right)
    if (from_j + 1 < 8) 
    {
        char captureRight =(*board)[from_i + path][from_j + 1];
        if ((colour == 0 && islower(captureRight)) || (colour == 1 && isupper(captureRight))) 
        {
            moves[numOfMoves++] = make_move(from_i, from_j, from_i + path, from_j - 1);
        }
    }

    // Realloc to exact number of moves and add NULL at end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}


