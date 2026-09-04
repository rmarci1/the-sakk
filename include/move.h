#ifndef MOVE.H
    
    #include "board.h"
    #define MOVE_MAX_LENGTH 20
    
    typedef struct {
        int current_turn;
        char move[MOVE_MAX_LENGTH];
    } Move;
    
    Location from_move;
    Location to_move;
    bool enPassant = false;

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
    int CheckPawnMoves(PieceColor, PieceList[HEIGHT][WIDTH], int, int, bool, Piece[HEIGHT][WIDTH]);
    int CheckPawnTypes(PieceList[HEIGHT][WIDTH], int, int, int, Piece[HEIGHT][WIDTH], PiecePlace, 
    PieceColor);
    int CheckPawnRows(Piece[HEIGHT][WIDTH], PiecePlace, int, int, int, int, int, PieceColor);
    int CheckWhichPawnAffects(PieceList[HEIGHT][WIDTH], int, int, int, int, PieceColor,
    Piece[HEIGHT][WIDTH], bool, PiecePlace, PieceColor, bool, bool, PieceType);
    void ChangingFromMovePosition(int, int);
    void ChangingToMovePosition(int, int);
    int IsMoveWrong(Piece[HEIGHT][WIDTH], bool, int, int, PieceColor, int);
    int ChangeRookPosition(int, int, int, int, int, int[PIECE_MAX_COUNT][2]);
    int isPieceFoundCorrect(Piece[HEIGHT][WIDTH], int*, int, int, int, int, bool, 
    PieceColor, char*, char*);
#endif