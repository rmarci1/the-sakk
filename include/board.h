#ifndef BOARD_H
#define BOARD_H

    #include "imports.h"
    #include "piece.h"
    #include "list.h"
    #include "config.h"
    #include "types.h"

    extern PieceList check_depth_white[HEIGHT][WIDTH];
    extern PieceList check_depth_black[HEIGHT][WIDTH];
    extern PieceList black_pawn_moves[HEIGHT][WIDTH];
    extern PieceList white_pawn_moves[HEIGHT][WIDTH];
    extern PiecePlace checkingPiece;
    extern Piece empty;
    extern PiecePlace nothing;
    extern bool double_check;
    extern int white_queens[PIECE_MAX_COUNT-1][2];
    extern int black_queens[PIECE_MAX_COUNT-1][2];
    extern int white_king[2];
    extern int black_king[2];
    extern int (*p_white_queens)[PIECE_MAX_COUNT-1][2];
    extern int (*p_black_queens)[PIECE_MAX_COUNT-1][2]; 
    extern int white_bishops[PIECE_MAX_COUNT][2];
    extern int black_bishops[PIECE_MAX_COUNT][2];
    extern int white_rooks[PIECE_MAX_COUNT][2];
    extern int black_rooks[PIECE_MAX_COUNT][2];
    extern int white_bishop_count;
    extern int black_bishop_count;
    extern int white_rook_count;
    extern int black_rook_count;
    extern int white_queen_count;
    extern int black_queen_count;

    int CheckInstanceBishop(int, int, int, int, int, int, PieceList*, 
    PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH], PieceColor, bool, bool*);
    int CheckInstanceRook(int, int, int, int, int, int, PieceList*, 
    PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH], PieceColor, bool, bool*);
    int CheckInstances(int, int, int, int, PieceList[HEIGHT][WIDTH], 
    Piece[HEIGHT][WIDTH], PieceColor, bool, bool*);
    int CheckWhenPieceMoves(int, int, int, int, Piece[HEIGHT][WIDTH], 
        bool, bool, PieceColor, bool);
    bool IsCheck(char, int, int, int[2], Piece[HEIGHT][WIDTH]);
    bool IsKnightCheck(Piece[HEIGHT][WIDTH], PieceColor, int, int);
    bool KingCheck(int, int, bool);
    int IsCheckChanged(Piece[HEIGHT][WIDTH], int, int, int, int, bool, int[2]);
    int IsCheckEqualsToMove(bool, bool);
    int ChangingTablePositions(Piece[HEIGHT][WIDTH], int, int, int, int, bool, bool, 
    PieceColor, bool, bool, PiecePlace);
    bool isPieceBetween(char, int, int, int, int, int, Piece[HEIGHT][WIDTH]);
    int ChangingKingInCheck(Piece[HEIGHT][WIDTH], bool, int, bool*, int, int, PieceColor);
    int CheckPlace(Piece, int, int, PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH], bool);
    void RemovePieceFromDepth(int, int, PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH]);
    bool CanBlock(Piece[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], int, int, PieceColor);
    Piece PieceinDirection(Piece[HEIGHT][WIDTH], int, int, int, int);
    int CheckPawnMoves(PieceColor, PieceList[HEIGHT][WIDTH], int, int, bool, Piece[HEIGHT][WIDTH]);
    int CheckPawnTypes(PieceList[HEIGHT][WIDTH], int, int, int, Piece[HEIGHT][WIDTH], PiecePlace, 
    PieceColor);
    int CheckPawnRows(Piece[HEIGHT][WIDTH], PiecePlace, int, int, int, int, int, PieceColor);
    int CheckWhichPawnAffects(PieceList[HEIGHT][WIDTH], int, int, int, int, PieceColor,
    Piece[HEIGHT][WIDTH], bool, PiecePlace, PieceColor, bool, bool, PieceType);
#endif