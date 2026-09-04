#ifndef PIECE_H
    #include <stdbool.h>

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
        PiecePlace* items;
        int size;
        int capacity;
    } PieceList;
    
    bool black_king_inCheck = false;
    bool white_king_inCheck = false;
    int white_bishop_count = 0;
    int black_bishop_count = 0;
    int white_rook_count = 0;
    int black_rook_count = 0;
    int white_queen_count = 0;
    int black_queen_count = 0;
    int* p_wbishopcount = NULL;
    int* p_bbishopcount = NULL;
    int* p_wrookcount = NULL;
    int* p_brookcount = NULL;
    int* p_wqueencount = NULL;
    int* p_bqueencount = NULL;
    
    char* getPiece(PieceColor, PieceType);
    PieceType setType(int, int);

#endif