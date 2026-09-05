#include "ui.h"
#include "board.h"
#include "piece.h"
#include "ui.h"
void PrintTable(Piece table[HEIGHT][WIDTH], PrintMove print_move){
    printf("\033[9;1H");
    printf("%s  ",SPACE);
    for (int i = 0; i < WIDTH; i++)
    {
        printf(" %c  ",'a'+i);
    }  
    printf("\n");
    for (int i = 0; i < HEIGHT; ++i)
    {   
        if(print_move.curr_position == 1) printf("\033[2K");
        else if(i == print_move.curr_position/2-1)  printf("\033[2K");
        printf("%s%d ", SPACE, 8-i);  
        for (int y = 0; y < WIDTH; y++)
        {   
            char* t = getPiece(table[i][y].color,table[i][y].type);
            if(table[i][y].type == KING && table[i][y].color == BLACK && black_king_inCheck == 1){
                printf("\033[31m %s \033[0m",t);
            }
            else if(table[i][y].type == KING && table[i][y].color == WHITE && white_king_inCheck == 1){
                printf("\033[31m %s \033[0m",t);
            }
            else if((i == from_move.row && y == from_move.col) || (i == to_move.row && y == to_move.col)){
                printf("\033[46m %s \033[0m",t);
            }
            else if( (i + y) % 2 == 0 ){
                printf("\033[%sm %s \033[0m",white_tile,t);
            }
            else {
                printf("\033[%sm %s \033[0m",black_tile,t);
            }
        }
        printf(" %d ", i+1);
        if((i+1)*2 <= print_move.length){
            //printf("%d:d",print_move.curr_position);
            if(print_move.moves[(i+1)*2-1].current_turn-((print_move.moves[(i+1)*2-1].current_turn-1)/8*8) == (print_move.curr_position+1)/2){
                printf("\033[100m\t\t %d. %s %s\033[0m",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move,print_move.moves[(i+1)*2-1].move); 
            } else {
                printf("\t\t %d. %s %s",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move,print_move.moves[(i+1)*2-1].move);   
            }
        } else if((i+1)*2-1 <= print_move.length){
            if((print_move.moves[(i+1)*2-2].current_turn-1)-((print_move.moves[(i+1)*2-2].current_turn-1)/8*8) == print_move.curr_position/2){
                printf("\033[100m\t\t %d. %s\033[0m",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move); 
            } else {
                printf("\t\t %d. %s",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move); 
            }
        }
        printf("\n");
    }
    printf("%s  ",SPACE);
    for (int i = 0; i < WIDTH; i++)
    {
        printf(" %c  ",'a'+i);
    }  
    printf("\n");
}   
void OldPrintTableForTest(Piece table[HEIGHT][WIDTH], PrintMove print_move){
    printf("  ");
    for (int i = 0; i < WIDTH; i++)
    {
        printf(" %c  ",'a'+i);
    }  
    printf("\n");
    for (int i = 0; i < HEIGHT; ++i)
    {   
        printf("%d ", i+1);
        for (int y = 0; y < WIDTH; y++)
        {   
            char* t = getPiece(table[i][y].color,table[i][y].type);
            if(table[i][y].type == KING && table[i][y].color == BLACK && black_king_inCheck == 1){
                printf("\033[31m %s \033[0m",t);
            }
            else if(table[i][y].type == KING && table[i][y].color == WHITE && white_king_inCheck == 1){
                printf("\033[31m %s \033[0m",t);
            }
            else if( (i + y) % 2 == 0 ){
                //printf("\033[100m %s \033[0m",t);
                /*if(black_pawn_moves[i][y].size>0) printf("\033[41m %s \033[0m",t);
                else {
                    printf("\033[100m %s \033[0m",t);
                }*/
                switch ( check_depth_black[i][y].size)
                {
                case 1:
                    printf("\033[41m %s \033[0m",t);
                    break;
                case 2:
                    printf("\033[42m %s \033[0m",t);
                    break;
                case 3:
                    printf("\033[44m %s \033[0m",t);
                    break;
                case 4:
                    printf("\033[45m %s \033[0m",t);
                    break;
                case 5:
                    printf("\033[46m %s \033[0m",t);
                    break;
                default:
                        printf("\033[100m %s \033[0m",t);
                }
            }
            else {
                //printf("\033[40m %s \033[0m",t);
                /*if(black_pawn_moves[i][y].size>0) printf("\033[41m %s \033[0m",t);
                else {
                    printf("\033[40m %s \033[0m",t);
                }*/
                switch ( check_depth_black[i][y].size)
                {
                case 1:
                    printf("\033[41m %s \033[0m",t);
                    break;
                case 2:
                    printf("\033[42m %s \033[0m",t);
                    break;
                case 3:
                    printf("\033[44m %s \033[0m",t);
                    break;
                case 4:
                    printf("\033[45m %s \033[0m",t);
                    break;
                default:
                    printf("\033[40m %s \033[0m",t);
                }
            }
        }
        printf(" %d ", i+1);
        printf("\n");
    }
    printf("  ");
    for (int i = 0; i < WIDTH; i++)
    {
        printf(" %c  ",'a'+i);
    }  
    printf("\n");
}