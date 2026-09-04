#ifndef LIST.H
    #include "board.h"
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