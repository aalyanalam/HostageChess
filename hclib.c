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
        printf("newboard: FAILED (malloc failed)\n");
        return NULL; // malloc failed
    }

    // Make initial board
    printf("newboard: Initializing board setup...\n");

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

    printf("newboard: Board initialization complete.\n");
    return board;
}



exboard_t* copyboard(exboard_t* board) 
{
    if (board == NULL) 
    {
        printf("copyboard: FAILED (input board is NULL)\n");
        return NULL;
    }

    exboard_t *newBoard = (exboard_t *)malloc(sizeof(exboard_t));
    if (newBoard == NULL) 
    {
        printf("copyboard: FAILED (malloc failed)\n");
        return NULL;
    }

    // Copy existing board to new one
    memcpy(newBoard, board, sizeof(exboard_t));

    printf("copyboard: Board copied successfully.\n");
    return newBoard;
}

char* stringboard(exboard_t* board) 
{
    if (board == NULL) 
    {
        printf("stringboard: FAILED (board is NULL)\n");
        return NULL;
    }

    char *boardStr = (char *)malloc(162 * sizeof(char));
    if (boardStr == NULL) 
    {
        printf("stringboard: FAILED (malloc failed)\n");
        return NULL;
    }

    printf("stringboard: Converting board to string...\n");

    // Black piece prison
    memset(boardStr, ' ', 161);

    //Add \n in the string
    for(int i = 8; i <= 161; i += 9) 
    {
        boardStr[i] = '\n';
    }

    //Add black side prison to string
    for(int i = 0; i <= 7; i++) 
    {
        if(board->bprison[i] == '\0')    
        {
            for(int j = i; j <= 7; j++) 
            {
                boardStr[j] = ' ';
            }
            break;
        }
        boardStr[i] = board->bprison[i];
    } // 0-7 is filled, 8 is nl


    for(int i = 8; i <= 15; i++) 
    {
        if(board->bprison[i] == '\0') 
        {
        for(int j = i; j <= 15; j++) 
        {
            boardStr[j + 1] = ' ';
        }
        break;
        }
        boardStr[i + 1] = board->bprison[i];  
    }// 9-16 is filled, 17 is nl


    // Black piece airfield
    for(int i = 0; i <= 7; i++) 
    {
        if(board->bairfield[i] == '\0') 
        {
            for(int j = i; j <= 7; j++) 
            {
                boardStr[j + 18] = ' ';
            }
            break;
        }
        boardStr[i + 18] = board->bairfield[i];
    }// 18-25 is filled, 26 is nl

    for (int i = 0; i <= 7; i++) 
    {
        if (board->bairfield[i + 8] == '\0') 
        {
        for (int j = i; j <= 7; j++) 
        {
            boardStr[j + 27] = ' ';  
        }
        break;
        }
        boardStr[i + 27] = board->bairfield[i + 8];
    } //27-34 is filled, 35 is newline

    // Separate with dashes
    for(int i = 36; i <= 43; i++) 
    {
        boardStr[i] = '-';
    } // 36-43 is filled, 44 is nl

    // Black pieces from rank 8
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
    } // 45-115 is filled, 116 is nl

    // Separate with dashes
    for(int i = 117; i <= 124; i++) 
    {
        boardStr[i] = '-';
    } // 117-124 is filled, 125 is nl

    // White piece airfield
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wairfield[i] == '\0') 
        {
            for(int j = i; j <= 7; j++) 
            {
                boardStr[j + 126] = ' ';
            }
            break;
        }
        boardStr[i + 126] = board->wairfield[i];
    } // 126-133 is filled, 134 is nl

    for(int i = 8; i <= 15; i++) 
    {
        if(board->wairfield[i] == '\0') 
        {
            for(int j = i; j <= 15; j++) 
            {
                boardStr[j + 135] = ' ';
            }
            break;
        }
        boardStr[i + 135] = board->wairfield[i];
    } // 135-142 is filled, 143 is nl

    // White piece prison
    for(int i = 0; i <= 7; i++) 
    {
        if(board->wprison[i] == '\0') 
        {
            for(int j = i; j <= 7; j++) 
            {
                boardStr[j + 144] = ' ';
            }
            break;
        }
        boardStr[i + 144] = board->wprison[i];
    } // 144-151 is filled, 152 is nl

    for(int i = 8; i <= 15; i++) 
    {
        if(board->wprison[i] == '\0') 
        {
            for(int j = i; j <= 15; j++) 
            {
                boardStr[j + 153] = ' ';
            }
            break;
        }
        boardStr[i + 153] = board->wprison[i];
    } boardStr[161] = '\0';// 153-160 is filled, 161 is nl

    return boardStr;
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

move_t **knightmoves( board_t *board, int from_i, int from_j, int colour )
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
            move_t *nMove = malloc(sizeof(move_t));
            nMove->from_i = from_i;
            nMove->from_j = from_j;
            nMove->to_i = to_i;
            nMove->to_j = to_j;
            nMove->promotion = ' ';
            nMove->hostage = ' ';

            moves[numOfMoves] = nMove;
            numOfMoves++;
        }
    }

    //Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}

move_t *create_move(int from_i, int from_j, int to_i, int to_j) 
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

int isValidMove(char destination, int colour)
{
    return NULL;
}



move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour)
{
    move_t **moves = malloc(16 * sizeof(move_t *)); // Allocate max possible moves + NULL pointer
    int numOfMoves = 0;

    //Move up and left  (-1, -1)
    for (int to_i = from_i - 1, to_j = from_j - 1; to_i >= 0 && to_j >= 0; to_i--, to_j--)
    {
        char destination = (*board)[to_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *bMove = malloc(sizeof(move_t));
            bMove->from_i = from_i;
            bMove->from_j = from_j;
            bMove->to_i = to_i;
            bMove->to_j = to_j;
            bMove->promotion = ' ';
            bMove->hostage = ' ';
            moves[numOfMoves++] = bMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *bMove = malloc(sizeof(move_t));
                bMove->from_i = from_i;
                bMove->from_j = from_j;
                bMove->to_i = to_i;
                bMove->to_j = to_j;
                bMove->promotion = ' ';
                bMove->hostage = ' ';
                moves[numOfMoves++] = bMove;
                break;
            }
        }
    }

    //Move up and right  (-1, +1)
    for (int to_i = from_i - 1, to_j = from_j + 1; to_i >= 0 && to_j < 8; to_i--, to_j++)
    {
        char destination = (*board)[to_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *bMove = malloc(sizeof(move_t));
            bMove->from_i = from_i;
            bMove->from_j = from_j;
            bMove->to_i = to_i;
            bMove->to_j = to_j;
            bMove->promotion = ' ';
            bMove->hostage = ' ';
            moves[numOfMoves++] = bMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *bMove = malloc(sizeof(move_t));
                bMove->from_i = from_i;
                bMove->from_j = from_j;
                bMove->to_i = to_i;
                bMove->to_j = to_j;
                bMove->promotion = ' ';
                bMove->hostage = ' ';
                moves[numOfMoves++] = bMove;
                break;
            }
        }
    }

    //Move down and left  (+1, -1)
    for (int to_i = from_i + 1, to_j = from_j - 1; to_i < 8 && to_j >= 0; to_i++, to_j--)
    {
        char destination = (*board)[to_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *bMove = malloc(sizeof(move_t));
            bMove->from_i = from_i;
            bMove->from_j = from_j;
            bMove->to_i = to_i;
            bMove->to_j = to_j;
            bMove->promotion = ' ';
            bMove->hostage = ' ';
            moves[numOfMoves++] = bMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *bMove = malloc(sizeof(move_t));
                bMove->from_i = from_i;
                bMove->from_j = from_j;
                bMove->to_i = to_i;
                bMove->to_j = to_j;
                bMove->promotion = ' ';
                bMove->hostage = ' ';
                moves[numOfMoves++] = bMove;
                break;
            }
        }
    }
    
    ////Move down and right  (+1, +1)
    for (int to_i = from_i + 1, to_j = from_j + 1; to_i < 8 && to_j < 8; to_i++, to_j++)
    {
        char destination = (*board)[to_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *bMove = malloc(sizeof(move_t));
            bMove->from_i = from_i;
            bMove->from_j = from_j;
            bMove->to_i = to_i;
            bMove->to_j = to_j;
            bMove->promotion = ' ';
            bMove->hostage = ' ';
            moves[numOfMoves++] = bMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *bMove = malloc(sizeof(move_t));
                bMove->from_i = from_i;
                bMove->from_j = from_j;
                bMove->to_i = to_i;
                bMove->to_j = to_j;
                bMove->promotion = ' ';
                bMove->hostage = ' ';
                moves[numOfMoves++] = bMove;
                break;
            }
        }
    }

    //Realloc to exact amount needed with NULL at the end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}

move_t **rookmoves(board_t *board, int from_i, int from_j, int colour)
{
    move_t **moves = malloc(16 * sizeof(move_t *)); // Allocate max possible moves + NULL pointer
    int numOfMoves = 0;

    //Move up
    for (int to_i = from_i - 1; to_i >= 0 ; to_i--)
    {
        char destination = (*board)[to_i][from_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *rMove = malloc(sizeof(move_t));
            rMove->from_i = from_i;
            rMove->from_j = from_j;
            rMove->to_i = to_i;
            rMove->to_j = from_j;
            rMove->promotion = ' ';
            rMove->hostage = ' ';
            moves[numOfMoves++] = rMove;

        }
        else //Run into a piece
        {

            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *rMove = malloc(sizeof(move_t));
                rMove->from_i = from_i;
                rMove->from_j = from_j;
                rMove->to_i = to_i;
                rMove->to_j = from_j;
                rMove->promotion = ' ';
                rMove->hostage = ' ';
                moves[numOfMoves++] = rMove;
                break;
            }
        }
    }

    //Move left
    for (int to_j = from_j - 1;to_j >= 0;to_j--)
    {
        char destination = (*board)[from_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *rMove = malloc(sizeof(move_t));
            rMove->from_i = from_i;
            rMove->from_j = from_j;
            rMove->to_i = from_i;
            rMove->to_j = to_j;
            rMove->promotion = ' ';
            rMove->hostage = ' ';
            moves[numOfMoves++] = rMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *rMove = malloc(sizeof(move_t));
                rMove->from_i = from_i;
                rMove->from_j = from_j;
                rMove->to_i = from_i;
                rMove->to_j = to_j;
                rMove->promotion = ' ';
                rMove->hostage = ' ';
                moves[numOfMoves++] = rMove;
                break;
            }
        }
    }

    //Move down
    for (int to_i = from_i + 1;to_i < 8; to_i++)
    {
        char destination = (*board)[to_i][from_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *rMove = malloc(sizeof(move_t));
            rMove->from_i = from_i;
            rMove->from_j = from_j;
            rMove->to_i = to_i;
            rMove->to_j = from_j;
            rMove->promotion = ' ';
            rMove->hostage = ' ';
            moves[numOfMoves++] = rMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *rMove = malloc(sizeof(move_t));
                rMove->from_i = from_i;
                rMove->from_j = from_j;
                rMove->to_i = to_i;
                rMove->to_j = from_j;
                rMove->promotion = ' ';
                rMove->hostage = ' ';
                moves[numOfMoves++] = rMove;
                break;
            }
        }
    }

    ////Move right 
    for (int to_j = from_j + 1;to_j < 8;to_j++)
    {
        char destination = (*board)[from_i][to_j];
        if (destination == ' ') //Move to empty space
        {
            move_t *rMove = malloc(sizeof(move_t));
            rMove->from_i = from_i;
            rMove->from_j = from_j;
            rMove->to_i = from_i;
            rMove->to_j = to_j;
            rMove->promotion = ' ';
            rMove->hostage = ' ';
            moves[numOfMoves++] = rMove;
        }
        else //Run into a piece
        {
            if ((colour == 0 && isupper(destination)) || (colour == 1 && islower(destination)))
            {
                break; //Ran into friendly piece
            }
            else
            {
                move_t *rMove = malloc(sizeof(move_t));
                rMove->from_i = from_i;
                rMove->from_j = from_j;
                rMove->to_i = from_i;
                rMove->to_j = to_j;
                rMove->promotion = ' ';
                rMove->hostage = ' ';
                moves[numOfMoves++] = rMove;
                break;
            }
        }
    }

    //Realloc to exact amount needed with NULL at the end
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
            move_t *kMove = malloc(sizeof(move_t));
            kMove->from_i = from_i;
            kMove->from_j = from_j;
            kMove->to_i = to_i;
            kMove->to_j = to_j;
            kMove->promotion = ' ';
            kMove->hostage = ' ';

            moves[numOfMoves] = kMove;
            numOfMoves++;
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
    if (board->board[from_i + path][from_j] == ' ') 
    {
        move_t *pMove = malloc(sizeof(move_t));
        pMove->from_i = from_i;
        pMove->from_j = from_j;
        pMove->to_i = from_i + path;
        pMove->to_j = from_j;
        pMove->promotion = ' ';
        pMove->hostage = ' ';
        moves[numOfMoves++] = pMove;

        // Check if pawn can move forward 2 squares (only if it is on starting rank)
        if ((colour == 0 && from_i == 1) || (colour == 1 && from_i == 6)) 
        {
            if (board->board[from_i + 2 * path][from_j] == ' ') 
            {
                move_t *pMove2 = malloc(sizeof(move_t));
                pMove2->from_i = from_i;
                pMove2->from_j = from_j;
                pMove2->to_i = from_i + 2 * path;
                pMove2->to_j = from_j;
                pMove2->promotion = ' ';
                pMove2->hostage = ' ';
                moves[numOfMoves++] = pMove2;
            }
        }
    }

    // Check if pawn can capture diagonally (left)
    if (from_j - 1 >= 0) 
    {
        char captureLeft = board->board[from_i + path][from_j - 1];
        if ((colour == 0 && islower(captureLeft)) || (colour == 1 && isupper(captureLeft))) 
        {
            move_t *pMove = malloc(sizeof(move_t));
            pMove->from_i = from_i;
            pMove->from_j = from_j;
            pMove->to_i = from_i + path;
            pMove->to_j = from_j - 1;
            pMove->promotion = ' ';
            pMove->hostage = ' ';
            moves[numOfMoves++] = pMove;
        }
    }

    // Check if pawn can capture diagonally (right)
    if (from_j + 1 < 8) 
    {
        char captureRight = board->board[from_i + path][from_j + 1];
        if ((colour == 0 && islower(captureRight)) || (colour == 1 && isupper(captureRight))) 
        {
            move_t *pMove = malloc(sizeof(move_t));
            pMove->from_i = from_i;
            pMove->from_j = from_j;
            pMove->to_i = from_i + path;
            pMove->to_j = from_j + 1;
            pMove->promotion = ' ';
            pMove->hostage = ' ';
            moves[numOfMoves++] = pMove;
        }
    }

    // Realloc to exact number of moves and add NULL at end
    moves = realloc(moves, (numOfMoves + 1) * sizeof(move_t *));
    moves[numOfMoves] = NULL;

    return moves;
}


