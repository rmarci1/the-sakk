#ifndef MOVE_H
#define MOVE_H
    
    #include "imports.h"
    #include "piece.h"
    #include "config.h"
    #include "board.h"
    #include "types.h"
    typedef struct {
        int current_turn;
        char move[MOVE_MAX_LENGTH];
    } Move;

    int PawnMove(char[MOVE_MAX_LENGTH], int[2], bool*, Piece[HEIGHT][WIDTH], 
        PieceColor, bool, bool, PiecePlace*, bool*);
    int PawnTakes(char[MOVE_MAX_LENGTH], Piece[HEIGHT][WIDTH], PieceColor, 
        bool, PiecePlace, bool, int[2], bool*, bool);
    int EnPassant(int, int, int, PiecePlace, PieceColor, Piece[HEIGHT][WIDTH], bool, bool);
    int Promote(int, int, int, char, Piece[HEIGHT][WIDTH], bool, PieceColor, bool, int[2], bool*);
    int KnightMove(char[MOVE_MAX_LENGTH], int[2], bool*, Piece[HEIGHT][WIDTH], PieceColor, bool, 
        bool, int, int, int);
    int BishopMove(char[MOVE_MAX_LENGTH], int[2], bool*, int[PIECE_MAX_COUNT][2], int*, 
        Piece[HEIGHT][WIDTH], PieceColor, bool, bool, int, int, int);
    int RookMove(char[MOVE_MAX_LENGTH], int[2], bool*, int[PIECE_MAX_COUNT][2], int*, 
        Piece[HEIGHT][WIDTH], PieceColor, bool*, bool*, bool, bool, int, int, int);
    int QueenMove(char[MOVE_MAX_LENGTH], int[2], bool*, int[PIECE_MAX_COUNT-1][2], 
        int*, Piece[HEIGHT][WIDTH], PieceColor, bool, bool, int, int, int);
    int KingMove(char[MOVE_MAX_LENGTH], int[2], Piece[HEIGHT][WIDTH], PieceColor, bool*, 
        bool, bool, PieceList[HEIGHT][WIDTH]);
    int Castle(int[2], int[2], Piece[HEIGHT][WIDTH], PieceColor, bool*, bool,
        bool, bool, int, int, Piece, 
        int[PIECE_MAX_COUNT][2], int, bool, PieceList[HEIGHT][WIDTH], bool, bool*);
    int IsMoveWrong(Piece[HEIGHT][WIDTH], bool, int, int, PieceColor, int);
    int ChangeRookPosition(int, int, int, int, int, int[PIECE_MAX_COUNT][2]);
    int isPieceFoundCorrect(Piece[HEIGHT][WIDTH], int*, int, int, int, int, bool, 
    PieceColor, char*, char*);
#endif