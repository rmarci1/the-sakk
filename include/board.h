#ifndef BOARD.H
    #include "piece.h"

    #define HEIGHT 8
    #define WIDTH 8
    #define PIECE_MAX_COUNT 10

    typedef struct{
        int row;
        int col;
    } Location;
    typedef struct {
        int row;
        int column;
        PieceType piece;
    } PiecePlace;
    int white_bishops[PIECE_MAX_COUNT][2];
    int black_bishops[PIECE_MAX_COUNT][2];
    int white_rooks[PIECE_MAX_COUNT][2];
    int black_rooks[PIECE_MAX_COUNT][2];
    int white_queens[PIECE_MAX_COUNT-1][2];
    int black_queens[PIECE_MAX_COUNT-1][2];
    int white_king[2];
    int black_king[2];

    int (*p_white_bishops)[PIECE_MAX_COUNT][2] = &white_bishops;
    int (*p_black_bishops)[PIECE_MAX_COUNT][2] = &black_bishops;
    int (*p_white_rooks)[PIECE_MAX_COUNT][2] = &white_rooks;
    int (*p_black_rooks)[PIECE_MAX_COUNT][2] = &black_rooks;
    int (*p_white_queens)[PIECE_MAX_COUNT-1][2] = &white_queens;
    int (*p_black_queens)[PIECE_MAX_COUNT-1][2] = &black_queens; 

    PieceList check_depth_white[HEIGHT][WIDTH] = {0};
    PieceList check_depth_black[HEIGHT][WIDTH] = {0};
    PieceList black_pawn_moves[HEIGHT][WIDTH];
    PieceList white_pawn_moves[HEIGHT][WIDTH];
    PiecePlace checkingPiece;
    Piece empty;
    PiecePlace nothing;
    bool double_check = false;

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
    int CheckPlace(Piece, int, int, PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH], Bool);
    void RemovePieceFromDepth(int, int, PieceList[HEIGHT][WIDTH], Piece[HEIGHT][WIDTH]);
    bool CanBlock(Piece[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], int, int, PieceColor);
    Piece PieceinDirection(Piece[HEIGHT][WIDTH], int, int, int, int);
#endif