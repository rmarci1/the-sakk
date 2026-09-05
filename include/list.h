#ifndef LIST_H
#define LIST_H
    #include "piece.h"
    #include "config.h"
    #include "types.h"

    extern int* p_wbishopcount;
    extern int* p_bbishopcount;
    extern int* p_wrookcount;
    extern int* p_brookcount;
    extern int* p_wqueencount;
    extern int* p_bqueencount;
    extern int (*p_white_bishops)[PIECE_MAX_COUNT][2];
    extern int (*p_black_bishops)[PIECE_MAX_COUNT][2];
    extern int (*p_white_rooks)[PIECE_MAX_COUNT][2];
    extern int (*p_black_rooks)[PIECE_MAX_COUNT][2];

    void addPiece(PieceList*, PiecePlace);
    void removePiece(PieceList*,PiecePlace);
    void freePieceList(PieceList*);
    void piece_list_copy(PieceList[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH]);
    void freeAllPieceList(PieceList[HEIGHT][WIDTH]);
    int RemovePieceFromList(int[PIECE_MAX_COUNT][2], int*, int, int);
    int RemoveType(Piece[HEIGHT][WIDTH], int, int, PieceColor);
    void CleanDepthList(PieceList[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], bool, 
    bool, PiecePlace*, PiecePlace, Location, Location, bool);
#endif