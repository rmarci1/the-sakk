#include "piece.h"
#include "board.h"
PieceType setType(int cell, int row){
    if (row == 0 || row == HEIGHT-1){
        switch(cell){
            case 0:
                return ROOK;
            case 1:
                return KNIGHT;
            case 2:
                return BISHOP;
            case 3:
                return QUEEN;
            case 4:
                return KING;
            case 5:
                return BISHOP;
            case 6:
                return KNIGHT;
            case 7:
                return ROOK;
        }
    }
    else if (row == 1 || row == HEIGHT-2){
        return PAWN;
    }
    return EMPTY;
}
char* getPiece(PieceColor color, PieceType type){
    switch(type){
        case KING:
            return color != WHITE ? "\u2654 " : "\u265A ";
        case QUEEN:
            return color != WHITE ? "\u2655 " : "\u265B ";
        case ROOK:
            return color != WHITE ? "\u2656 " : "\u265C ";
        case KNIGHT:
            return color != WHITE ? "\u2658 " : "\u265E ";
        case BISHOP:
            return color != WHITE ? "\u2657 " : "\u265D ";
        case PAWN:
            return color != WHITE ? "\u2659 " : "\u265F ";
        default:
            return "  ";
    }
}
void ChangingFromMovePosition(int row, int col){
    from_move.row = row;
    from_move.col = col;
}
void ChangingToMovePosition(int row, int col){
    to_move.row = row;
    to_move.col = col;
}
