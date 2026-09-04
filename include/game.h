#ifndef GAME.H
    #ifdef _WIN32
        #include <conio.h>
        char getch_new(void)
        {
            return _getch();
        }
        #define ENTER 13
    #else

        #include <termios.h>
        #include <unistd.h>
        char getch_new(void)
        {
            struct termios oldt, newt;
            char ch;

            tcgetattr(STDIN_FILENO, &oldt);

            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);

            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            ch = getchar();

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

            return ch;
        }
        #define ENTER 10

    #endif
    #include "piece.h"
    #include "move.h"
    #include "ui.h"
    int game(Piece[HEIGHT][WIDTH]);
    void Initialize(Piece[HEIGHT][WIDTH]);
    bool IsDepthNotPawnOrKing(PiecePlace*, int, Piece[HEIGHT][WIDTH], PieceColor, bool, PieceList[HEIGHT][WIDTH]);
    bool isMate(Piece[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], PieceList[HEIGHT][WIDTH], 
    int[2], PieceList[HEIGHT][WIDTH], bool, PieceColor);
    void clearLastDoubleMove(PiecePlace*);
#endif