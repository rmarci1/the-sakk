#ifndef UI_H
#define UI_H
    #define WHITE_TILE_BASE "100"
    #define BLACK_TILE_BASE "40"
    #define WHITE_TILE_CLASSIC "48;5;189"
    #define BLACK_TILE_CLASSIC  "48;5;69"
    #define WHITE_TILE_BLUE "48;5;248"
    #define BLACK_TILE_BLUE  "48;5;239"
    #define SPACE "\t\t\t\t"
    #define THE_CHESS_SPACE "\t\t\t\t"
    #define MENU_MIN_LENGTH 0
    #define MENU_MAX_LENGTH 2
    #define CTRL_C 3
    
    #include "imports.h"
    #include "board.h"
    #include "move.h"

    
    typedef struct {
        Move moves[16];
        int length;
        int curr_position;
    } PrintMove;
    extern PrintMove print_moves;

    extern char* black_tile;
    extern char* white_tile;

    void PrintTable(Piece[HEIGHT][WIDTH], PrintMove);
    void OldPrintTableForTest(Piece[HEIGHT][WIDTH], PrintMove);
    int main();
#endif