#ifndef GAME_H
#define GAME_H
    #ifdef _WIN32
        #include <conio.h>
        #define ENTER 13
    #else
        #include <termios.h>
        #include <unistd.h>
        #include <stdio.h>
        #define ENTER 10
    #endif
    #include "imports.h"
    #include "board.h"
    #include "ui.h"
    #include "move.h"

    extern int white_bishops[PIECE_MAX_COUNT][2];
    extern int black_bishops[PIECE_MAX_COUNT][2];

    extern int white_rooks[PIECE_MAX_COUNT][2];
    extern int black_rooks[PIECE_MAX_COUNT][2];

    extern int white_queens[PIECE_MAX_COUNT-1][2];
    extern int black_queens[PIECE_MAX_COUNT-1][2];

    extern int white_king[2];
    extern int black_king[2];

    extern int (*p_white_bishops)[PIECE_MAX_COUNT][2];
    extern int (*p_black_bishops)[PIECE_MAX_COUNT][2];

    extern int (*p_white_rooks)[PIECE_MAX_COUNT][2];
    extern int (*p_black_rooks)[PIECE_MAX_COUNT][2];

    extern int (*p_white_queens)[PIECE_MAX_COUNT-1][2];
    extern int (*p_black_queens)[PIECE_MAX_COUNT-1][2];

    extern PrintMove print_moves;

    extern char* black_tile;
    extern char* white_tile;

    extern PieceList check_depth_white[HEIGHT][WIDTH];
    extern PieceList check_depth_black[HEIGHT][WIDTH];

    extern PieceList black_pawn_moves[HEIGHT][WIDTH];
    extern PieceList white_pawn_moves[HEIGHT][WIDTH];

    extern PiecePlace checkingPiece;
    extern Piece empty;
    extern PiecePlace nothing;

    extern bool double_check;

    extern int white_bishop_count;
    extern int black_bishop_count;
    extern int white_rook_count;
    extern int black_rook_count;
    extern int white_queen_count;
    extern int black_queen_count;

    extern Location from_move;
    extern Location to_move;

    extern bool black_king_inCheck;
    extern bool white_king_inCheck;
    extern bool enPassant;

    int game(Piece[HEIGHT][WIDTH]);
    void Initialize(Piece[HEIGHT][WIDTH]);
    bool IsDepthNotPawnOrKing(PiecePlace*, int, Piece[HEIGHT][WIDTH], PieceColor, bool, PieceList[HEIGHT][WIDTH]);
    bool isMate(Piece[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], 
    int[2], PieceList[HEIGHT][WIDTH], bool, PieceColor);
    void clearLastDoubleMove(PiecePlace*);
    char getch_new(void);
#endif