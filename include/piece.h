#ifndef PIECE_H
#define PIECE_H
    #include "imports.h"
    #include "types.h"
    typedef enum{
        KING,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN,
        EMPTY
    } PieceType;
    typedef enum{
        WHITE, 
        BLACK,
        NOTHING
    } PieceColor;
    typedef struct{
        PieceType type;
        PieceColor color;
    } Piece;
     typedef struct {
        int row;
        int column;
        PieceType piece;
    } PiecePlace;
    typedef struct {
        PiecePlace* items;
        int size;
        int capacity;
    } PieceList;
    extern Location from_move;
    extern Location to_move;
    extern bool black_king_inCheck;
    extern bool white_king_inCheck;
    extern bool enPassant;
    char* getPiece(PieceColor, PieceType);
    PieceType setType(int, int);
    void ChangingFromMovePosition(int, int);
    void ChangingToMovePosition(int, int);

#endif