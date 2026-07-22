#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEIGHT 8
#define WIDTH 8
#define MOVE_MAX_LENGTH 20
#define PIECE_MAX_COUNT 10
#define SPACE "\t\t\t\t"
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
typedef struct {
    int current_turn;
    char move[MOVE_MAX_LENGTH];
} Move;
typedef struct {
    Move moves[16];
    int length;
    int curr_position;
} PrintMove;
typedef enum{
    TRUE,
    FALSE
} Bool;





int white_bishops[PIECE_MAX_COUNT][2];
int black_bishops[PIECE_MAX_COUNT][2];
int white_rooks[PIECE_MAX_COUNT][2];
int black_rooks[PIECE_MAX_COUNT][2];
int white_queens[PIECE_MAX_COUNT-1][2];
int black_queens[PIECE_MAX_COUNT-1][2];
int white_king[2];
int black_king[2];

int (*p_white_bishops)[PIECE_MAX_COUNT][2] = &white_bishops;
int (*p_black_bishops)[PIECE_MAX_COUNT][2] = &black_bishops;
int (*p_white_rooks)[PIECE_MAX_COUNT][2] = &white_rooks;
int (*p_black_rooks)[PIECE_MAX_COUNT][2] = &black_rooks;
int (*p_white_queens)[PIECE_MAX_COUNT-1][2] = &white_queens;
int (*p_black_queens)[PIECE_MAX_COUNT-1][2] = &black_queens; 

int* p_wbishopcount = NULL;
int* p_bbishopcount = NULL;
int* p_wrookcount = NULL;
int* p_brookcount = NULL;
int* p_wqueencount = NULL;
int* p_bqueencount = NULL;

bool black_king_inCheck = false;
bool white_king_inCheck = false;
PiecePlace checkingPiece;
Piece empty;
PiecePlace nothing;
bool isPieceBetween(char, int, int, int, int, int, Piece[HEIGHT][WIDTH]);
int CheckPlace(Piece, int, int, PieceList[HEIGHT][WIDTH],Piece[HEIGHT][WIDTH], Bool);
int CheckWhenPieceMoves(int, int, int, int, Piece[HEIGHT][WIDTH], bool, bool, PieceColor, bool);
void addPiece(PieceList*, PiecePlace);
void removePiece(PieceList*,PiecePlace);
void freePieceList(PieceList*);
void piece_list_copy(PieceList dest[HEIGHT][WIDTH], PieceList src[HEIGHT][WIDTH]);
void freeAllPieceList(PieceList[HEIGHT][WIDTH]);
int CheckPawnMoves(PieceColor, PieceList[HEIGHT][WIDTH], int, int, bool, Piece[HEIGHT][WIDTH]);

PieceList check_depth_white[HEIGHT][WIDTH] = {0};
PieceList check_depth_black[HEIGHT][WIDTH] = {0};
PieceList black_pawn_moves[HEIGHT][WIDTH];
PieceList white_pawn_moves[HEIGHT][WIDTH];

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
PieceType setType(int cell, int row, PieceColor color){
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
void Initialize(Piece table[HEIGHT][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            Piece p;
            if(i == 0 || i== 1) p.color = BLACK;
            else if (i == HEIGHT-2 || i==HEIGHT-1) p.color = WHITE;
            else p.color = NOTHING;
            p.type = setType(y,i,p.color);
            if(p.color == WHITE && p.type == BISHOP){
                white_bishops[*p_wbishopcount][0] = i;
                white_bishops[*p_wbishopcount][1] = y;
                *p_wbishopcount = *p_wbishopcount + 1;
            }
            else if(p.color == BLACK && p.type == BISHOP){
                black_bishops[*p_bbishopcount][0] = i;
                black_bishops[*p_bbishopcount][1] = y;
                *p_bbishopcount = *p_bbishopcount + 1;
            }
            else if(p.color == WHITE && p.type == ROOK){
                white_rooks[*p_wrookcount][0] = i;
                white_rooks[*p_wrookcount][1] = y;
                *p_wrookcount = *p_wrookcount + 1;
            }
            else if(p.color == BLACK && p.type == ROOK){
                black_rooks[*p_brookcount][0] = i;
                black_rooks[*p_brookcount][1] = y;
                *p_brookcount = *p_brookcount + 1;
            }
            else if(p.color == WHITE && p.type == QUEEN){
                white_queens[*p_wqueencount][0] = i;
                white_queens[*p_wqueencount][1] = y;
                *p_wqueencount = *p_wqueencount + 1;
            }
            else if(p.color == BLACK && p.type == QUEEN){
                black_queens[*p_bqueencount][0] = i;
                black_queens[*p_bqueencount][1] = y;
                *p_bqueencount = *p_bqueencount + 1;
            }
            else if(p.color == WHITE && p.type == KING){
                white_king[0] = i;
                white_king[1] = y;
            }
            else if(p.color == BLACK && p.type == KING){
                black_king[0] = i;
                black_king[1] = y;
            }
            else if(p.type == PAWN){
                CheckPawnMoves(p.color, p.color == WHITE ? black_pawn_moves : white_pawn_moves, i, y, false, table);
            }
            table[i][y] = p;
        }
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int y = 0; y < WIDTH; y++){
            CheckPlace(table[i][y], i, y, table[i][y].color == WHITE ? check_depth_black : check_depth_white, table, FALSE);
        }
    }
}
void CleanDepthList(PieceList temp_white[HEIGHT][WIDTH], PieceList temp_black[HEIGHT][WIDTH], bool white_king_inCheck_temp, 
    bool black_king_inCheck_temp, PiecePlace* last_double_move, PiecePlace last_double_move_temp){
    piece_list_copy(check_depth_white,temp_white);
    piece_list_copy(check_depth_black,temp_black);
    white_king_inCheck = white_king_inCheck_temp;
    black_king_inCheck = black_king_inCheck_temp;
    *last_double_move = last_double_move_temp;
}
int IsCheckChanged(Piece table[HEIGHT][WIDTH], int sor, int oszlop, int hova_sor, int hova_oszlop, bool takes, int king[2]){
    Piece temp_from = table[hova_sor][hova_oszlop];
    Piece temp_to = table[sor][oszlop];
    table[sor][oszlop] = table[hova_sor][hova_oszlop];
    table[hova_sor][hova_oszlop] = empty;
    bool isChanged = true;
    Piece piece = table[sor][oszlop];
    if(takes && hova_sor == checkingPiece.row && hova_oszlop == checkingPiece.column){
    }
    else if(checkingPiece.piece == PAWN && (!takes || hova_sor != checkingPiece.row || hova_oszlop != checkingPiece.column )){
        printf("Ez a lépés nem állítja meg a sakkot!\n");
        isChanged = false;
    }
    else if(checkingPiece.piece == KNIGHT && (!takes || hova_sor != checkingPiece.row || hova_oszlop != checkingPiece.column )){
        printf("Ez a lépés nem állítja meg a sakkot!\n");
        isChanged = false;
    }
    else{
        if(takes && (hova_sor != checkingPiece.row || hova_oszlop != checkingPiece.column)){
            printf("Ez a lépés nem állítja meg a sakkot!\n");
            isChanged = false;
        }
        else{
            char type = checkingPiece.piece == QUEEN ? 'Q' : checkingPiece.piece == ROOK ? 'R' : 'B';
            int melyik_mezo = checkingPiece.row == white_king[0] ? 1 : checkingPiece.column == white_king[1] ? 0 : 2;
            if(isPieceBetween(type, melyik_mezo, checkingPiece.row, checkingPiece.column, king[0], king[1], table)){
                printf("Ez a lépés nem állítja meg a sakkot!\n");
                isChanged = false;
            }
        }
    }
    table[sor][oszlop] = temp_to;
    table[hova_sor][hova_oszlop] = temp_from;
    return isChanged ? 0 : 1;
}
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
        printf("%s%d ", SPACE, i+1);
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
                printf("\033[100m %s \033[0m",t);
                /*if(white_pawn_moves[i][y].size>0) printf("\033[41m %s \033[0m",t);
                else {
                    printf("\033[100m %s \033[0m",t);
                }
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
                }*/
            }
            else {
                printf("\033[40m %s \033[0m",t);
                /*if(white_pawn_moves[i][y].size>0) printf("\033[41m %s \033[0m",t);
                else {
                    printf("\033[40m %s \033[0m",t);
                }
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
                }*/
            }
        }
        printf(" %d ", i+1);
        if((i+1)*2 <= print_move.length){
            printf("\t\t %d. %s %s",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move,print_move.moves[(i+1)*2-1].move);   
        } else if((i+1)*2-1 <= print_move.length){
            printf("\t\t %d. %s",print_move.moves[(i+1)*2-2].current_turn, print_move.moves[(i+1)*2-2].move);
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
void RemovePieceFromDepth(int sor, int oszlop, PieceList depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH]){
    CheckPlace(table[sor][oszlop], sor, oszlop, depth, table, TRUE);
}
int RemovePieceFromList(int list[PIECE_MAX_COUNT][2], int* count, int sor, int oszlop){
    int j = -1;
    int i = -1;
    while(i<*count && (list[j][0] != sor || list[j][1] != oszlop))
    {
        i++;
        j++;
        while(list[j][0] == -1) j++;    
    }
    if(i==*count){
        printf("Hiba van a Remove résznél a programba!\n");
        return 1;
    }
    list[i][0] = -1;
    list[i][1] = -1;
    *count = *count-1;
    return 0;
}
int RemoveType(Piece table[HEIGHT][WIDTH], int sor, int oszlop, PieceColor turn){
    if(turn == WHITE){
        if(table[sor][oszlop].type == BISHOP){
            if(RemovePieceFromList(*p_black_bishops, p_bbishopcount, sor, oszlop) == 1){
                return 1;
            }
        }
        else if(table[sor][oszlop].type == ROOK){
            if(RemovePieceFromList(*p_black_rooks, p_brookcount, sor, oszlop) == 1){
                return 1;
            }
        }
    }
    else{
        if(table[sor][oszlop].type == BISHOP){
            if(RemovePieceFromList(*p_white_bishops, p_wbishopcount, sor, oszlop) == 1){
                return 1;
            }
        }
        else if(table[sor][oszlop].type == ROOK){
            if(RemovePieceFromList(*p_white_rooks, p_wrookcount, sor, oszlop) == 1){
                return 1;
            }
        }
    }
    return 0;
}
int CheckPawnMoves(PieceColor turn, PieceList pawn_moves[HEIGHT][WIDTH], int row, int column, bool remove, Piece table[HEIGHT][WIDTH]){ 
    PiecePlace pawn;
    pawn.row = row;
    pawn.column = column;
    pawn.piece = PAWN; 
    PieceColor opposite = turn == WHITE ? BLACK : WHITE;
    bool two_moves = false;
    if(turn == WHITE){
        if(row == 6){
            if(!remove){
                addPiece(&pawn_moves[row-1][column], pawn);
                addPiece(&pawn_moves[row-2][column], pawn);
            } else{
                removePiece(&pawn_moves[row-1][column], pawn);
                removePiece(&pawn_moves[row-2][column], pawn);
            }
            two_moves = true;
        }
    }
    else{
        if(row == 1){
            if(!remove){
                addPiece(&pawn_moves[row+1][column], pawn);
                addPiece(&pawn_moves[row+2][column], pawn);
            } else{
                removePiece(&pawn_moves[row+1][column], pawn);
                removePiece(&pawn_moves[row+2][column], pawn);
            }
            two_moves = true;
        }
    }
    int add = turn == WHITE ? -1 : 1;
    if(row+add >= 0 && row+add <= 7 && table[row+add][column].type == EMPTY && !two_moves){
        if(!remove){
            addPiece(&pawn_moves[row+add][column], pawn);
        } else{
            removePiece(&pawn_moves[row+add][column], pawn);
        }
    }
    if(row+add >= 0 && row+add <= 7 && column+1 >= 0 && column + 1 < 8 && table[row+add][column+1].type != EMPTY && table[row+add][column+1].color == opposite){
        if(remove){
            removePiece(&pawn_moves[row+add][column + 1], pawn);
        } else{
            addPiece(&pawn_moves[row+add][column + 1],pawn);
        }
    }
    if(row+add >= 0 && row+add <= 7 && column+1 >= 0 && column + 1 < 8 && table[row+add][column-1].type != EMPTY && table[row+add][column-1].color == opposite){
        if(remove){
            removePiece(&pawn_moves[row+add][column-1], pawn);
        } else {
            addPiece(&pawn_moves[row+add][column-1],pawn);
        }
    }
    return 0;
}
int CheckPawnTypes(PieceList pawn_moves[HEIGHT][WIDTH], int row, int column, int add, Piece table[HEIGHT][WIDTH], PiecePlace pawn, 
    PieceColor opposite, bool remove){
    // Megnézi hogy milyen gyalog lépés típusokra adjon vagy töröljön a pawn_moves listából/hoz; 
    if(table[row+(add*-1)][column+1].type == PAWN && table[row+(add*-1)][column+1].color == opposite){
        pawn.row = row+add*-1;
        pawn.column = column+1;
        //if(remove) removePiece(&pawn_moves[row][column], pawn);
        addPiece(&pawn_moves[row][column], pawn);
    }
    if(table[row+(add*-1)][column-1].type == PAWN && table[row+(add*-1)][column-1].color == opposite){
        pawn.row = row+add*-1;
        pawn.column = column-1;
        //if(remove) removePiece(&pawn_moves[row][column], pawn);
        addPiece(&pawn_moves[row][column], pawn);
    }
    return 0;
}

int CheckPawnRows(Piece table[HEIGHT][WIDTH], PiecePlace pawn, int from_row, int from_column, int to_row, int to_column, int add, PieceColor curr){
    if(table[from_row+add][from_column].color == BLACK && curr == WHITE){
        pawn.row = from_row+add;
        pawn.column = from_column;
        addPiece(&white_pawn_moves[from_row][from_column],pawn);
        if((pawn.row == 1 || pawn.row == 6) && table[from_row+add*-1][from_column].type == EMPTY && (to_row != from_row+add*-1 || to_column != from_column)){
            addPiece(&white_pawn_moves[from_row+add*-1][from_column], pawn); 
        }
    }
    if(table[from_row+add][from_column].color == WHITE && curr == BLACK){
        pawn.row = from_row+add;
        pawn.column = from_column;
        addPiece(&black_pawn_moves[from_row][from_column],pawn);
        if((pawn.row == 1 || pawn.row == 6) && table[from_row+add*-1][from_column].type == EMPTY && (to_row != from_row+add*-1 || to_column != from_column)){
            addPiece(&black_pawn_moves[from_row+add*-1][from_column],pawn); 
        }
    }
}
int CheckWhichPawnAffects(PieceList pawn_moves[HEIGHT][WIDTH], int from_row, int from_column, int to_row, int to_column, PieceColor curr,
    Piece table[HEIGHT][WIDTH], bool pawn_move, PiecePlace last_double_move, PieceColor turn, bool takes, bool is_pawn_taken, PieceType moveType){
    while(pawn_moves[from_row][from_column].size > 0){
        removePiece(&pawn_moves[from_row][from_column], pawn_moves[from_row][from_column].items[0]);
    }
    while(pawn_moves[to_row][to_column].size > 0){
        removePiece(&pawn_moves[to_row][to_column], pawn_moves[to_row][to_column].items[0]);
    }
    PieceColor opposite = turn == WHITE ? BLACK : WHITE;
    if(takes && is_pawn_taken && turn != curr){
        Piece temp;
        temp.color = turn;
        temp.type = moveType;
        table[from_row][from_column] = temp;
        CheckPawnMoves(opposite, pawn_moves, to_row, to_column, true, table);
        table[from_row][from_column] = empty;
    }
    int add = curr == WHITE ? -1 : 1;
    PiecePlace pawn;
    pawn.piece = PAWN;

    if(!(takes && is_pawn_taken && turn != curr) && !takes){
        if(to_row+add*-1 == 1 && curr == BLACK && !pawn_move && table[to_row+add*-1][to_column].type == PAWN && table[to_row+add*-1][to_column].color == WHITE){
            printf("inremove1\n");
            removePiece(&pawn_moves[to_row+1][to_column], pawn_moves[to_row+1][to_column].items[0]);
        }
        else if(to_row+add*-1 == 6 && curr == WHITE && !pawn_move && table[to_row+add*-1][to_column].type == PAWN && table[to_row+add*-1][to_column].color == BLACK){
            printf("inremove2\n");
            removePiece(&pawn_moves[to_row-1][to_column], pawn_moves[to_row-1][to_column].items[0]);
        }
    }
    

    //printf("row: %d, opposite: %s \n",from_row+add,opposite == WHITE ? "white" : "black");
    if(curr == WHITE){
        if (table[from_row+1][from_column].type == PAWN)
        {
           CheckPawnRows(table,pawn,from_row,from_column,to_row,to_column,1,BLACK);
        }    
        if (table[from_row-1][from_column].type == PAWN)
        {
           CheckPawnRows(table,pawn,from_row,from_column,to_row,to_column,-1,WHITE);
        }    
    }
    if(table[from_row+add*-2][from_column].type == PAWN && table[from_row+add*-1][from_column].type == EMPTY && (from_row+add*-2 == 1 || from_row+add*-2 == 6) && table[from_row+add*-2][from_column].color == opposite){
        pawn.row = from_row+add*-2;
        pawn.column = from_column;
        addPiece(&pawn_moves[from_row][from_column],pawn);
    }
    //CheckPawnTypes(pawn_moves, from_row, from_column, add, table, pawn, opposite, true);
    CheckPawnTypes(pawn_moves, to_row, to_column, add, table, pawn, opposite, false);
    if(last_double_move.piece != EMPTY && table[last_double_move.row][last_double_move.column+1].type == PAWN 
        && table[last_double_move.row][last_double_move.column+1].color == opposite){
        pawn.row = last_double_move.row;
        pawn.column = last_double_move.column+1;
        addPiece(&pawn_moves[last_double_move.row+add][last_double_move.column], pawn);
    }
    if(last_double_move.piece != EMPTY && table[last_double_move.row][last_double_move.column-1].type == PAWN 
        && table[last_double_move.row][last_double_move.column-1].color == opposite){
        pawn.row = last_double_move.row;
        pawn.column = last_double_move.column-1;
        addPiece(&pawn_moves[last_double_move.row+add][last_double_move.column], pawn);
    }
    return 0;
}
bool isKnightCheck(Piece table[HEIGHT][WIDTH], PieceColor opposite, int sor, int oszlop, int indulas[2]){
    for (int i = 1; i <= 8; ++i)
    {       
        //printf("megnézett hely[ sor: %d, oszlop: %d  ]", indulas[0], indulas[1]);
        //printf("bábu: %s \n", getPiece(table[indulas[0]][indulas[1]].color, table[indulas[0]][indulas[1]].type));
        //printf("sor: %d, oszlop: %d", indulas[0], indulas[1]);
        if (indulas[0]>=0 && indulas[0]<=7 && indulas[1]>=0 && indulas[1]<=7 && table[indulas[0]][indulas[1]].type == KING && table[indulas[0]][indulas[1]].color == opposite)
        {   
            return true;
        }
        if(i<4 && i%2 == 1){
            indulas[1] = indulas[1] + 2;
        }
        else if (i<4 && i%2 == 0){
            indulas[0] = sor - 2;
            indulas[1] = oszlop - 1;
        }
        else if (i==4){
            indulas[0] = sor - 1;
            indulas[1] = oszlop - 2;
        }
        else if(i>4 && i%2 == 1){
            indulas[0] = indulas[0] + 2;
        }
        else{
            indulas[0] = sor - 1;
            indulas[1] = oszlop + 2;
        }
    }
    return false;
}
int EnPassant(int from_column, int row, int column, PiecePlace last_double_move, PieceColor turn, Piece table[HEIGHT][WIDTH], bool check, bool takes){
    int add = turn == WHITE ? 1 : -1;
    if(last_double_move.piece != EMPTY && last_double_move.row == (row+add) && last_double_move.column == column)
    {   
        RemovePieceFromDepth(row+add, column, turn == WHITE ? check_depth_white : check_depth_black, table);
        if(CheckPlace(table[row+add][from_column], row+add, from_column, table[row+add][from_column].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
            return 1;
        }
        if (CheckWhenPieceMoves(row+add, from_column, row, column, table, check, true, turn, takes) == 1){
            return 1;
        }
        if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, row+add, from_column, true, table) == 1){
            return 1;
        }
        if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, row, column, false, table) == 1){
            return 1;
        }
        table[row][column] = table[row+add][from_column];
        table[row+add][from_column] = empty;
        table[row+add][column] = empty;
        if(CheckPlace(table[row][column], row, column, table[row][column].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
            return 1;
        }
        if (CheckWhenPieceMoves(row, column, -1, -1, table, check, true, turn, takes) == 1){
            return 1;
        }
        if(CheckWhichPawnAffects(white_pawn_moves, row+add, from_column, row, column, BLACK, table, true, last_double_move, turn, true, true, PAWN) || 
        CheckWhichPawnAffects(black_pawn_moves, row+add, from_column, row, column, WHITE, table, true, last_double_move, turn, true, true, PAWN) == 1) return 1;
        return 0;
        return 0;
    }
    return 1;
}
int Promote(int sor, int oszlop, int from_oszlop, char piece_type, Piece table[HEIGHT][WIDTH], bool check, PieceColor turn, bool takes){
    Piece p;
    p.color = turn;
    if(piece_type == 'B'){
        p.type = BISHOP;
        if(turn == WHITE){
            white_bishops[*p_wbishopcount][0] = sor;
            white_bishops[*p_wbishopcount][1] = oszlop;
            (*p_wbishopcount)++;
        }
        else{
            black_bishops[*p_bbishopcount][0] = sor;
            black_bishops[*p_bbishopcount][1] = oszlop;
            (*p_bbishopcount)++;
        }
    }
    else if(piece_type == 'N'){
        p.type = KNIGHT;
    }
    else if(piece_type == 'R'){
        p.type = ROOK;
        if(turn == WHITE){
            white_rooks[*p_wrookcount][0] = sor;
            white_rooks[*p_wrookcount][1] = oszlop;
            (*p_wrookcount)++;
        }
        else{
            black_rooks[*p_brookcount][0] = sor;
            black_rooks[*p_brookcount][1] = oszlop;
            (*p_brookcount)++;
        }
    }
    else if(piece_type == 'Q'){
        p.type = QUEEN;
        if(turn == WHITE){
            white_queens[*p_wqueencount][0] = sor;
            white_queens[*p_wqueencount][1] = oszlop;
            (*p_wqueencount)++;
        }
        else{
            black_queens[*p_bqueencount][0] = sor;
            black_queens[*p_bqueencount][1] = oszlop;
            (*p_bqueencount)++;
        }
    }  
    else{
        printf("Rossz típust adtál meg!\n");
        return 1;
    }
    int add = turn == WHITE ? 1 : -1;
    int where_sor = sor + add;
    if(CheckPlace(table[where_sor][from_oszlop], where_sor, from_oszlop, table[where_sor][from_oszlop].color == WHITE ? 
        check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(where_sor, from_oszlop, sor, oszlop, table, check, false, turn, takes) == 1) return 1;
    RemovePieceFromDepth(where_sor, from_oszlop, turn == WHITE ? check_depth_black : check_depth_white, table);
    table[sor][oszlop] = p;
    table[where_sor][from_oszlop] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? 
        check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, false, turn, takes) == 1) return 1;
    return 0;
}
int PawnTakes(char lepes[MOVE_MAX_LENGTH], Piece table[HEIGHT][WIDTH], PieceColor turn, bool check, PiecePlace last_double_move, bool promote, int king[2], bool* king_inCheck, bool takes)
{       
    int honnan_oszlop = lepes[0] - 'a';
    int oszlop = lepes[1] - 'a';
    int sor = lepes[2]-'0'-1;
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    if((sor == 7 || sor == 0 ) && !promote){
        printf("Nem adtad meg mivé promotoljon!\n");
        return 1;
    }
    if(turn == WHITE){
        if(table[sor + 1][honnan_oszlop].type == PAWN && table[sor + 1][honnan_oszlop].color == WHITE && table[sor][oszlop].color == BLACK && abs(oszlop-honnan_oszlop) == 1){
            if(white_king_inCheck || black_king_inCheck){
                if(IsCheckChanged(table,sor+1,honnan_oszlop,sor,oszlop,true,turn == WHITE ? white_king : black_king) == 1){
                    return 1;
                }
            }
            RemoveType(table,sor,oszlop,turn);
            RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);

            if(promote){
                if(strlen(lepes) < 4){
                    printf("Nem adtad meg mivé promotoljon!\n");
                    return 1;
                }
                if(Promote(sor, oszlop, honnan_oszlop, lepes[3], table, check, turn, takes) == 1){
                    return 1;
                }
                return 0;
            }
            bool isCheck = false;
            if(king[0]-sor == -1 && abs(king[1]-oszlop) == 1){
                isCheck = true;
            }
            if(!isCheck && check){
                printf("Ez nem sakk!\n");
                return 1;
            }
            else if(isCheck && !check){
                printf("Ez a lépés sakkot adna!\n");
                return 1;
            }
            if(CheckPlace(table[sor+1][honnan_oszlop], sor+1, honnan_oszlop, table[sor+1][honnan_oszlop].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
                return 1;
            }
            if(CheckWhenPieceMoves(sor+1, honnan_oszlop, sor, oszlop, table, check, isCheck, turn, takes) == 1){
                return 1;
            }
            if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, sor+1, honnan_oszlop, true, table) == 1){
                return 1;
            }
            if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, sor, oszlop, false, table) == 1){
                return 1;
            }
            table[sor][oszlop] = table[sor + 1][honnan_oszlop];
            table[sor + 1][honnan_oszlop] = empty;
            
            if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1){
                return 1;
            }
            if (CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1){
                return 1;
            }
            if(CheckWhichPawnAffects(white_pawn_moves, sor+1, honnan_oszlop, sor, oszlop, BLACK, table, true, last_double_move, turn, true, is_pawn_taken, PAWN) || 
            CheckWhichPawnAffects(black_pawn_moves, sor+1, honnan_oszlop, sor, oszlop, WHITE, table, true, last_double_move, turn, true, is_pawn_taken, PAWN) == 1) return 1;
            if(isCheck && check){
                *king_inCheck = true;
                checkingPiece.piece = table[sor][oszlop].type;
                checkingPiece.row = sor;
                checkingPiece.column = oszlop;
            }
            else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
                if(turn == WHITE){
                    white_king_inCheck = 0;
                }
                else{
                    black_king_inCheck = 0;
                }
            }
            return 0;
        }
        else if(EnPassant(honnan_oszlop, sor, oszlop, last_double_move, turn, table, check, takes) == 0){
            return 0;
        }
        else{
            printf("A lépés helytelen!\n");
            return 1;
        }
    }
    else{
        if(table[sor - 1][honnan_oszlop].type == PAWN && table[sor - 1][honnan_oszlop].color == BLACK && table[sor][oszlop].color == WHITE && abs(oszlop-honnan_oszlop) == 1){
            if(white_king_inCheck || black_king_inCheck){
                if(IsCheckChanged(table,sor-1,honnan_oszlop,sor,oszlop,true,turn == WHITE ? white_king : black_king) == 1){
                    return 1;
                }
            }
            RemoveType(table,sor,oszlop,turn);
            RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
            if(promote){
                if(strlen(lepes) < 4){
                    printf("Nem adtad meg mivé promotoljon!\n");
                    return 1;
                }
                if(Promote(sor, oszlop, honnan_oszlop, lepes[3], table, check, turn, takes) == 1){
                    return 1;
                }
                return 0;
            }
            bool isCheck = false;
            if(king[0]-sor == 1 && abs(king[1]-oszlop) == 1){
                isCheck = true;
            }
            if(!isCheck && check){
                printf("Ez nem sakk!\n");
                return 1;
            }
            else if(isCheck && !check){
                printf("Ez a lépés sakkot adna!\n");
                return 1;
            }
            if(CheckPlace(table[sor-1][honnan_oszlop], sor-1, honnan_oszlop, table[sor-1][honnan_oszlop].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
                return 1;
            }
            if(CheckWhenPieceMoves(sor-1, honnan_oszlop, sor, oszlop, table, check, false, turn, takes) == 1){
                return 1;
            }
            if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, sor-1, honnan_oszlop, true, table) == 1){
                return 1;
            }
            if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, sor, oszlop, false, table) == 1){
                return 1;
            }
            table[sor][oszlop] = table[sor - 1][honnan_oszlop];
            table[sor - 1][honnan_oszlop] = empty;
            if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1){
                return 1;
            }
            if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, false, turn, takes) == 1){
                return 1;
            }
            if(CheckWhichPawnAffects(white_pawn_moves, sor-1, honnan_oszlop, sor, oszlop, BLACK, table, true, last_double_move, turn, true, is_pawn_taken, PAWN) || 
            CheckWhichPawnAffects(black_pawn_moves, sor-1, honnan_oszlop, sor, oszlop, WHITE, table, true, last_double_move, turn, true, is_pawn_taken, PAWN) == 1) return 1;
            if(isCheck && check){
                *king_inCheck = true;
                checkingPiece.piece = table[sor][oszlop].type;
                checkingPiece.row = sor;
                checkingPiece.column = oszlop;
            }
            else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
                if(turn == WHITE){
                    white_king_inCheck = 0;
                }
                else{
                    black_king_inCheck = 0;
                }
            }
            return 0;
        }
        else if(EnPassant(honnan_oszlop, sor, oszlop, last_double_move, turn, table, check, takes) == 0){
            return 0;
        }
        else{
            printf("A lépés helytelen!\n");
            return 1;
        }
    }
}
int PawnMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, Piece table[HEIGHT][WIDTH], 
    PieceColor turn, bool takes, bool check, PiecePlace* last_double_move, bool* isMoveDouble){
    int promote = false;
    if(strchr(lepes, '=') != NULL){
        int j = 0;
        promote = true;
        for (int i = 0; lepes[i] != '\0'; i++) {
            if (lepes[i] != '=') {
                lepes[j++] = lepes[i];
            }
        }
        lepes[j] = '\0'; 
    }
    if(takes){
        return PawnTakes(lepes, table, turn, check, *last_double_move, promote, king, king_inCheck, takes);
    }
    if(promote){
        if(strlen(lepes) < 3){
            printf("Nem adtad meg mivé promotoljon!\n");
            return 1;
        }
        if(Promote(lepes[0] - 'a', lepes[1]-'0'-1, lepes[1]-'0'-1, lepes[2], table, check, turn, takes) == 1) return 1;
        return 0;
    }
    int length = strlen(lepes);
    if(length != 2 || lepes[0] < 'a' || lepes[0] > 'h' || lepes[1] < '1' || lepes[1] > '8'){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = lepes[0] - 'a';
    int sor = lepes[1]-'0'-1;
    if(sor == 7 || sor == 0){
        printf("Nem adtad meg mivé promotoljon!\n");
        return 1;
    }
    int add = turn == WHITE ? 1 : -1;
    int check_row = turn == WHITE ? 6 : 1;
    Piece curr = table[sor][oszlop];
    int hol_sor = -1;
    if(table[sor+add][oszlop].type == PAWN && table[sor+add][oszlop].color == turn && curr.type == EMPTY){
        hol_sor = sor+add;
    }
    else if(table[sor+2*add][oszlop].type == PAWN && table[sor+2*add][oszlop].color == turn && sor+2*add == check_row && curr.type == EMPTY && table[sor+add][oszlop].type == EMPTY){
        hol_sor = sor+2*add;
    }
    else{
        printf("A lépés helytelen!\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table,sor,oszlop,hol_sor,oszlop,false,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    bool isCheck = false;
    if(turn==WHITE && king[0]-sor == -1 && abs(king[1]-oszlop) == 1){
        isCheck = true;
    }
    else if(king[0]-sor == 1 && abs(king[1]-oszlop) == 1){
        isCheck = true;
    }
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    if(abs(hol_sor - sor) == 2){
        *isMoveDouble = true;
        (*last_double_move).piece = PAWN;
        (*last_double_move).row = sor;
        (*last_double_move).column = oszlop;
    }
    if(CheckPlace(table[hol_sor][oszlop], hol_sor, oszlop, table[hol_sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(hol_sor, oszlop, sor, oszlop, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, hol_sor, oszlop, true, table) == 1){
        return 1;
    }
    if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, sor, oszlop, false, table) == 1){
        return 1;
    }
    table[sor][oszlop] = table[hol_sor][oszlop];
    table[hol_sor][oszlop] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, hol_sor, oszlop, sor, oszlop, BLACK, table, true, *last_double_move, turn, takes, false, PAWN) || 
    CheckWhichPawnAffects(black_pawn_moves, hol_sor, oszlop, sor, oszlop, WHITE, table, true, *last_double_move, turn, takes, false, PAWN) == 1) return 1;

    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[sor][oszlop].type;
        checkingPiece.row = sor;
        checkingPiece.column = oszlop;
    }
    else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }
    return 0;
}
int KnightMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, bool check){
    int length = strlen(lepes);
    int oszlop = length == 3 ? lepes[1] - 'a' : length == 4 ? lepes[2] - 'a' : lepes[3] - 'a';
    int sor = length == 3 ? lepes[2]-'0'-1 : length == 4 ? lepes[3] - '0' - 1 : lepes[4] - '0' - 1;
    if(!takes && table[sor][oszlop].type != EMPTY){
        printf("Ott van már bábu!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].color == turn){
        printf("A saját bábudat nem tudod leütni!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].type == EMPTY){
        printf("Ott nincs bábu amit le lehet ütni!\n");
        return 1;
    }
    //printf("oszlop: %d\n",oszlop);
    //printf("sor: %d\n",sor);
    int lovak[8][2];
    int db = 0;
    int* p_db = &db;
    int indulas[2] = {sor+2, oszlop-1};
    for (int i = 1; i <= 8; ++i)
    {       
        //printf("megnézett hely[ sor: %d, oszlop: %d  ]", indulas[0], indulas[1]);
        //printf("bábu: %s \n", getPiece(table[indulas[0]][indulas[1]].color, table[indulas[0]][indulas[1]].type));
        //printf("sor: %d, oszlop: %d", indulas[0], indulas[1]);
        if (indulas[0]>=0 && indulas[0]<=7 && indulas[1]>=0 && indulas[1]<=7 && table[indulas[0]][indulas[1]].type == KNIGHT && table[indulas[0]][indulas[1]].color == turn)
        {   
            lovak[db][0] = indulas[0];
            lovak[db][1] = indulas[1];
            *p_db = *p_db+1;
        }
        if(i<4 && i%2 == 1){
            indulas[1] = indulas[1] + 2;
        }
        else if (i<4 && i%2 == 0){
            indulas[0] = sor - 2;
            indulas[1] = oszlop - 1;
        }
        else if (i==4){
            indulas[0] = sor - 1;
            indulas[1] = oszlop - 2;
        }
        else if(i>4 && i%2 == 1){
            indulas[0] = indulas[0] + 2;
        }
        else{
            indulas[0] = sor - 1;
            indulas[1] = oszlop + 2;
        }
    }
    if(*p_db == 0){
        printf("Hibás lépés!\n");
        return 1;
    }
    int hova_sor = -1;
    int hova_oszlop = -1;
    if(length==3){
        if(*p_db > 1)
        {
            printf("Hibás lépés! Több ló közül lehet választani!\n");
            return 1;
        }
        if(takes){
            RemoveType(table,sor,oszlop,turn);
        }
        hova_sor = lovak[0][0];
        hova_oszlop = lovak[0][1];
    }
    else if(length == 4){
        int van = 1;
        int hol[2];
        int honnan_checking_mezo = -1;
        int checking_mezo = -1;
        if(lepes[1] > '0' && lepes[1] <= '8'){
            checking_mezo = lepes[1]-'0'-1;
            honnan_checking_mezo=0;
        }
        else if(lepes[1] >= 'a' && lepes[1] <= 'h'){
            checking_mezo = lepes[1]-'a';
            honnan_checking_mezo=1;
        }
        else{
            printf("Nincs ilyen lépés!\n");
            return 1;
        }
        for (int i = 0; i < *p_db; i++)
        {
            if(van == 0 && lovak[i][honnan_checking_mezo] == checking_mezo)
            {    
                printf("Hibás lépés! Több ló közül lehet választani ebben a sorban!\n");
                return 1;
            }
            else if(van == 1 && lovak[i][honnan_checking_mezo] == checking_mezo){
                van = 0;
                hol[0] = lovak[i][0];
                hol[1] = lovak[i][1];
            }
        }  
        if(van == 1){
            printf("Nincs ebben a sorban ló!\n");
            return 1;
        }     
        if(takes){
            RemoveType(table,sor,oszlop,turn);
        }
        hova_sor = hol[0];
        hova_oszlop = hol[1];
    }
    else if(length == 5){
        if(lepes[1] < 'a' || lepes[1] > 'h' || lepes[2] < '1' || lepes[2] > '8'){
            printf("Nincs ilyen lépés\n");
            return 1;
        }
        int hol[2] = {lepes[2] - '0' - 1, lepes[1] - 'a'};
        if(table[hol[0]][hol[1]].type != KNIGHT){
            printf("Nincs ott ló\n");
            return 1;
        }
        hova_sor = hol[0];
        hova_oszlop = hol[1];
    }   
    else{
        printf("Hibás lépés\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table,sor,oszlop,hova_sor,hova_oszlop,takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    int second_indulas[2] = {sor+2, oszlop-1};
    bool isCheck = isKnightCheck(table, turn == WHITE ? BLACK : WHITE, sor, oszlop,second_indulas);
    if(hova_sor == king[0] && hova_oszlop == king[1]){
        isCheck = true;
    }
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    if(takes){
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    if(CheckPlace(table[hova_sor][hova_oszlop], hova_sor, hova_oszlop, table[hova_sor][hova_oszlop].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(hova_sor, hova_oszlop, sor, oszlop, table, check, isCheck, turn, takes) == 1) return 1;
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    table[sor][oszlop] = table[hova_sor][hova_oszlop];
    table[hova_sor][hova_oszlop] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, hova_sor, hova_oszlop, sor, oszlop, BLACK, table, false, nothing, turn, takes, is_pawn_taken, KNIGHT) || 
    CheckWhichPawnAffects(black_pawn_moves, hova_sor, hova_oszlop, sor, oszlop, WHITE, table, false, nothing, turn, takes, is_pawn_taken, KNIGHT) == 1) return 1;
    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[sor][oszlop].type;
        checkingPiece.row = sor;
        checkingPiece.column = oszlop;
    }
    else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }
    return 0;
}
int BishopMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int bishops[PIECE_MAX_COUNT][2], 
    int* bishops_count, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, bool check){
    int length = strlen(lepes);
    if(length > 5){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = length == 3 ? lepes[1] - 'a' : length == 4 ? lepes[2] - 'a' : lepes[3] - 'a';
    int sor = length == 3 ? lepes[2]-'0'-1 : length == 4 ? lepes[3] - '0' - 1 : lepes[4] - '0' - 1;
    if(!takes && table[sor][oszlop].type != EMPTY){
        printf("Ott van már bábu!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].color == turn){
        printf("A saját bábudat nem tudod leütni!\n");
        return 1;
    }    
    else if(takes && table[sor][oszlop].type == EMPTY){
        printf("Ott nincs bábu amit le lehet ütni!\n");
        return 1;
    }
    int bishops_avalaible[PIECE_MAX_COUNT][2];

    int db = 0;
    int* p_db = &db;

    if(length == 5){
        int curr_sor = lepes[2] - '0' - 1; 
        int curr_oszlop = lepes[1] - 'a';
        if(curr_sor < 0 || curr_oszlop < 0 || curr_oszlop > 8 || curr_sor > 8){
            printf("Nincs ilyen lépés!\n");
            return 1;
        }
        else if(table[curr_sor][curr_oszlop].type != BISHOP || table[curr_sor][curr_oszlop].color != turn){
            printf("Nincs ott futó vagy a másik oldalé!\n");
            return 1;
        }        
        else if(abs(sor-curr_sor) != abs(oszlop-curr_oszlop)){
            printf("A futó nem tud oda lépni!\n");
            return 1;
        }
        bishops_avalaible[0][0] = curr_sor;
        bishops_avalaible[0][1] = curr_oszlop;
        *p_db = *p_db + 1;
    }
    else{
        int j = -1;
        for (int i = 0; i < *bishops_count; i++)
        {      
            j+=1;
            while(bishops[j][0] == -1){
                j++;
            }
            if(abs(sor-bishops[j][0]) == abs(oszlop-bishops[j][1])){
                if(length == 4){
                    if(lepes[1] > '0' && lepes[1] <= '8'){
                        if(bishops[j][0] != lepes[1] - '0' - 1) continue;
                    }
                    else if(lepes[1] >= 'a' && lepes[1] <= 'h'){
                        if(bishops[j][1] != lepes[1] - 'a') continue;
                    }
                    else{
                        printf("Nincs ilyen lépés!\n");
                        return 1;
                    }
                }
                bishops_avalaible[db][0] = bishops[j][0];
                bishops_avalaible[db][1] = bishops[j][1];
                *p_db = *p_db + 1;
            }
        }      
    }
    int remaining_bishops[PIECE_MAX_COUNT][2];
    int remain_db = 0;
    int *p_remain_db = &remain_db;
    for (int i = 0; i < *p_db; i++)
    {   
        bool vane = isPieceBetween('B',2,bishops_avalaible[i][0],bishops_avalaible[i][1],sor,oszlop,table);
        if(vane){
            remaining_bishops[*p_remain_db][0] = bishops_avalaible[i][0];
            remaining_bishops[*p_remain_db][1] = bishops_avalaible[i][1];
            *p_remain_db = *p_remain_db + 1;
        }
    }
    if(*p_remain_db == 0){
        printf("Hiba nincs futó ami odatud lépni\n");
        return 1;
    }
    if(*p_remain_db > 1){
        printf("Több futó közül lehet választani!\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table,remaining_bishops[0][0],remaining_bishops[0][1],sor,oszlop,takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    int isCheck = false;
    if(abs(sor - king[0]) == abs(oszlop - king[1])){
        isCheck = isPieceBetween('B',2,sor,oszlop,king[0],king[1],table);
    }
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    int i = 0;
    int curr_count = 0;
    int curr_sor = remaining_bishops[0][0];
    int curr_oszlop = remaining_bishops[0][1];
    while (curr_count<*bishops_count && (bishops[i][0] != curr_sor || bishops[i][1] != curr_oszlop)){
        if(bishops[i][0] != -1) curr_count++;
        i++;
    }
    if(curr_count>=*bishops_count){
        printf("Nem találja a bishops listában az elemet! Hibás a kód\n");
        return 1;
    }
    bishops[i][0] = sor;
    bishops[i][1] = oszlop;
    if(takes){
        if(RemoveType(table,sor,oszlop,turn) == 1){
            return 1;
        };   
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    if(CheckPlace(table[remaining_bishops[0][0]][remaining_bishops[0][1]], remaining_bishops[0][0], remaining_bishops[0][1], table[remaining_bishops[0][0]][remaining_bishops[0][1]].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if (CheckWhenPieceMoves(remaining_bishops[0][0], remaining_bishops[0][1], sor, oszlop, table, check, isCheck, turn, takes) == 1) return 1;
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    table[sor][oszlop] = table[remaining_bishops[0][0]][remaining_bishops[0][1]];
    table[remaining_bishops[0][0]][remaining_bishops[0][1]] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, remaining_bishops[0][0], remaining_bishops[0][1], sor, oszlop, BLACK, table, false, nothing, turn, takes, is_pawn_taken, BISHOP) || 
    CheckWhichPawnAffects(black_pawn_moves, remaining_bishops[0][0], remaining_bishops[0][1], sor, oszlop, WHITE, table, false, nothing, turn, takes, is_pawn_taken, BISHOP) == 1) return 1;
    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[sor][oszlop].type;
        checkingPiece.row = sor;
        checkingPiece.column = oszlop;
    }
    else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }
    return 0;
}
int RookMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int rooks[PIECE_MAX_COUNT][2], int* rooks_count, 
    Piece table[HEIGHT][WIDTH], PieceColor turn, bool* left_rook_moved, bool* right_rook_moved, bool takes, bool check){
    int length = strlen(lepes);
    if(length > 5){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = length == 3 ? lepes[1] - 'a' : length == 4 ? lepes[2] - 'a' : lepes[3] - 'a';
    int sor = length == 3 ? lepes[2]-'0'-1 : length == 4 ? lepes[3] - '0' - 1 : lepes[4] - '0' - 1;

    if(!takes && table[sor][oszlop].type != EMPTY){
        printf("Ott van már bábu!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].color == turn){
        printf("A saját bábudat nem tudod leütni!\n");
        return 1;
    }    
    else if(takes && table[sor][oszlop].type == EMPTY){
        printf("Ott nincs bábu amit le lehet ütni!\n");
        return 1;
    }
    int rooks_avalaible[PIECE_MAX_COUNT][2];

    int db = 0;
    int* p_db = &db;

    if(length == 5){
        int curr_sor = lepes[2] - '0' - 1; 
        int curr_oszlop = lepes[1] - 'a';
        if(curr_sor < 0 || curr_oszlop < 0 || curr_oszlop > 8 || curr_sor > 8){
            printf("Nincs ilyen lépés!\n");
            return 1;
        }
        else if(table[curr_sor][curr_oszlop].type != ROOK || table[curr_sor][curr_oszlop].color != turn){
            printf("Nincs ott bástya vagy a másik oldalé!\n");
            return 1;
        }        
        else if(curr_oszlop != oszlop || curr_sor != sor){
            printf("A bástya nem tud oda lépni!\n");
            return 1;
        }
        rooks_avalaible[0][0] = curr_sor;
        rooks_avalaible[0][1] = curr_oszlop;
        *p_db = *p_db + 1;
    }
    else{
        int j = -1;
        for (int i = 0; i < *rooks_count; i++)
        {      
            j+=1;
            while(rooks[j][0] == -1){
                j++;
            }
            if(rooks[j][0] == sor || rooks[j][1] == oszlop){
                if(length == 4){
                    if(lepes[1] > '0' && lepes[1] <= '8'){
                        if(rooks[j][0] != lepes[1] - '0' - 1) continue;
                    }
                    else if(lepes[1] >= 'a' && lepes[1] <= 'h'){
                        if(rooks[j][1] != lepes[1] - 'a') continue;
                    }
                    else{
                        printf("Nincs ilyen lépés!\n");
                        return 1;
                    }
                }
                rooks_avalaible[db][0] = rooks[j][0];
                rooks_avalaible[db][1] = rooks[j][1];
                *p_db = *p_db + 1;
            }
        }      
    }
    int remaining_rooks[PIECE_MAX_COUNT][2];
    int remain_db = 0;
    int *p_remain_db = &remain_db;
    for (int i = 0; i < *p_db; i++)
    {   
        int melyik_mezo = sor == rooks_avalaible[i][0] ? 1 : 0;
        bool vane = isPieceBetween('R',melyik_mezo,rooks_avalaible[i][0],rooks_avalaible[i][1],sor,oszlop,table);
        if(vane){
            remaining_rooks[*p_remain_db][0] = rooks_avalaible[i][0];
            remaining_rooks[*p_remain_db][1] = rooks_avalaible[i][1];
            *p_remain_db = *p_remain_db + 1;
        }
    }
    if(*p_remain_db == 0){
        printf("Hiba nincs bástya ami odatud lépni\n");
        return 1;
    }
    if(*p_remain_db > 1){
        printf("Több bástya közül lehet választani!\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table,remaining_rooks[0][0],remaining_rooks[0][1],sor,oszlop,takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    bool isCheck = false;
    if(sor == king[0] || oszlop == king[1]){
        int melyik_mezo = sor == king[0] ? 1 : 0;
        isCheck = isPieceBetween('R',melyik_mezo,sor,oszlop,king[0],king[1],table);
    }
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    int i = 0;
    int curr_count = 0;
    int curr_sor = remaining_rooks[0][0];
    int curr_oszlop = remaining_rooks[0][1];
    while (curr_count<*rooks_count && (rooks[i][0] != curr_sor || rooks[i][1] != curr_oszlop)){
        if(rooks[i][0] != -1) curr_count++;
        i++;

    }
    if(curr_count>=*rooks_count){
        printf("Nem találja a bástya listában az elemet! Hibás a kód\n");
        return 1;
    }
    rooks[i][0] = sor;
    rooks[i][1] = oszlop;
    if(takes){
        if(RemoveType(table,sor,oszlop,turn) == 1){
            return 1;
        };
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    
    if(CheckPlace(table[remaining_rooks[0][0]][remaining_rooks[0][1]], remaining_rooks[0][0], remaining_rooks[0][1], table[remaining_rooks[0][0]][remaining_rooks[0][1]].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(remaining_rooks[0][0], remaining_rooks[0][1], sor, oszlop, table, check, isCheck, turn, takes) == 1) return 1;
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    table[sor][oszlop] = table[remaining_rooks[0][0]][remaining_rooks[0][1]];
    table[remaining_rooks[0][0]][remaining_rooks[0][1]] = empty;
    
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, remaining_rooks[0][0], remaining_rooks[0][1], sor, oszlop, BLACK, table, false, nothing, turn, takes, is_pawn_taken, ROOK) || 
    CheckWhichPawnAffects(black_pawn_moves, remaining_rooks[0][0], remaining_rooks[0][1], sor, oszlop, WHITE, table, false, nothing, turn, takes, is_pawn_taken, ROOK) == 1) return 1;
    if(rooks[i][0] == 7 && rooks[i][1] == 0){
        *left_rook_moved = true;
    }
    else if(rooks[i][0] == 7 && rooks[i][1] == 7){
        *right_rook_moved = true;
    }
    else if(rooks[i][0] == 0 && rooks[i][1] == 0){
        *right_rook_moved = true;
    }
    else if(rooks[i][0] == 0 && rooks[i][1] == 7){
        *left_rook_moved = true;
    }
    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[sor][oszlop].type;
        checkingPiece.row = sor;
        checkingPiece.column = oszlop;
    }
    else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }

    return 0;
}
int QueenMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int queens[PIECE_MAX_COUNT-1][2], 
    int* queens_count, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, bool check){
    int length = strlen(lepes);
    if(length > 5){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = length == 3 ? lepes[1] - 'a' : length == 4 ? lepes[2] - 'a' : lepes[3] - 'a';
    int sor = length == 3 ? lepes[2]-'0'-1 : length == 4 ? lepes[3] - '0' - 1 : lepes[4] - '0' - 1;

    if(!takes && table[sor][oszlop].type != EMPTY){
        printf("Ott van már bábu!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].color == turn){
        printf("A saját bábudat nem tudod leütni!\n");
        return 1;
    }    
    else if(takes && table[sor][oszlop].type == EMPTY){
        printf("Ott nincs bábu amit le lehet ütni!\n");
        return 1;
    }
    int queens_avalaible[PIECE_MAX_COUNT][2];

    int db = 0;
    int* p_db = &db;
    if(length == 5){
        int curr_sor = lepes[2] - '0' - 1; 
        int curr_oszlop = lepes[1] - 'a';
        if(curr_sor < 0 || curr_oszlop < 0 || curr_oszlop > 8 || curr_sor > 8){
            printf("Nincs ilyen lépés!\n");
            return 1;
        }
        else if(table[curr_sor][curr_oszlop].type != ROOK || table[curr_sor][curr_oszlop].color != turn){
            printf("Nincs ott királynő vagy a másik oldalé!\n");
            return 1;
        }        
        else if(curr_oszlop != oszlop || curr_sor != sor || abs(sor-curr_sor) != abs(oszlop-curr_oszlop)){
            printf("A kiránynő nem tud oda lépni!\n");
            return 1;
        }
        queens_avalaible[0][0] = curr_sor;
        queens_avalaible[0][1] = curr_oszlop;
        *p_db = *p_db + 1;
    }
    else{
        int j = -1;
        for (int i = 0; i < *queens_count; i++)
        {      
            j+=1;
            while(queens[j][0] == -1){
                j++;
            }
            if(queens[j][0] == sor || queens[j][1] == oszlop || abs(sor-queens[j][0]) == abs(oszlop-queens[j][1])){
                if(length == 4){
                    if(lepes[1] > '0' && lepes[1] <= '8'){
                        if(queens[j][0] != lepes[1] - '0' - 1) continue;
                    }
                    else if(lepes[1] >= 'a' && lepes[1] <= 'h'){
                        if(queens[j][1] != lepes[1] - 'a') continue;
                    }
                    else{
                        printf("Nincs ilyen lépés!\n");
                        return 1;
                    }
                }
                queens_avalaible[db][0] = queens[j][0];
                queens_avalaible[db][1] = queens[j][1];
                *p_db = *p_db + 1;
            }
        }      
    }
    int remaining_queens[PIECE_MAX_COUNT][2];
    int remain_db = 0;
    int *p_remain_db = &remain_db;
    for (int i = 0; i < *p_db; i++)
    {   
        int curr_sor = queens_avalaible[i][0];
        int curr_oszlop = queens_avalaible[i][1];
        // 2 == átló 1 == oszlop, 0 == sor
        int melyik_mezo = curr_sor == sor ? 1 : curr_oszlop == oszlop ? 0 : 2;
        bool vane = isPieceBetween('Q',melyik_mezo,curr_sor,curr_oszlop,sor,oszlop,table);
        if(vane){
            remaining_queens[*p_remain_db][0] = queens_avalaible[i][0];
            remaining_queens[*p_remain_db][1] = queens_avalaible[i][1];
            *p_remain_db = *p_remain_db + 1;
        }
    }
    if(*p_remain_db == 0){
        printf("Hiba nincs királynő ami odatud lépni\n");
        return 1;
    }
    if(*p_remain_db > 1){
        printf("Több királynő közül lehet választani!\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table,sor,oszlop,remaining_queens[0][0],remaining_queens[0][1],takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    bool isCheck = false;

    if(sor == king[0] || oszlop == king[1] || abs(sor-king[0]) == abs(oszlop-king[1])){
        int melyik_mezo = king[0] == sor ? 1 : king[1] == oszlop ? 0 : 2;
        isCheck = isPieceBetween('Q',melyik_mezo,sor,oszlop,king[0],king[1],table);
    }
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    int i = 0;
    int curr_count = 0;
    int curr_sor = remaining_queens[0][0];
    int curr_oszlop = remaining_queens[0][1];
    while (curr_count<*queens_count && (queens[i][0] != curr_sor || queens[i][1] != curr_oszlop)){
        if(queens[i][0] != -1) curr_count++;
        i++;
    }
    if(curr_count>=*queens_count){
        printf("Nem találja a királynő listában az elemet! Hibás a kód\n");
        return 1;
    }
    queens[i][0] = sor;
    queens[i][1] = oszlop;
    if(takes){
        if(RemoveType(table,sor,oszlop,turn) == 1){
            return 1;
        };
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    if(CheckPlace(table[remaining_queens[0][0]][remaining_queens[0][1]], remaining_queens[0][0], remaining_queens[0][1], table[remaining_queens[0][0]][remaining_queens[0][1]].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
        return 1;
    }
    if(CheckWhenPieceMoves(remaining_queens[0][0], remaining_queens[0][1], sor, oszlop, table, check, isCheck, turn, takes) == 1){
        return 1;
    }
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    table[sor][oszlop] = table[remaining_queens[0][0]][remaining_queens[0][1]];
    table[remaining_queens[0][0]][remaining_queens[0][1]] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1){
        return 1;
    }
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1){
        return 1;
    }
    if(CheckWhichPawnAffects(white_pawn_moves, remaining_queens[0][0], remaining_queens[0][1], sor, oszlop, BLACK, table, false, nothing, turn, takes, is_pawn_taken, QUEEN) || 
    CheckWhichPawnAffects(black_pawn_moves, remaining_queens[0][0], remaining_queens[0][1], sor, oszlop, WHITE, table, false, nothing, turn, takes, is_pawn_taken, QUEEN) == 1) return 1;
    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[sor][oszlop].type;
        checkingPiece.row = sor;
        checkingPiece.column = oszlop;
    }
    else if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }
    return 0;
}
bool KingCheck(int sor, int oszlop){
    return (((checkingPiece.piece == ROOK || checkingPiece.piece == QUEEN) && (sor == checkingPiece.row || oszlop == checkingPiece.column)) || 
           ((checkingPiece.piece == BISHOP || checkingPiece.piece == QUEEN) && (abs(sor-checkingPiece.row) == abs(oszlop-checkingPiece.column))));
}
int KingMove(char lepes[MOVE_MAX_LENGTH], int king[2], Piece table[HEIGHT][WIDTH], PieceColor turn, bool* king_moved, bool takes, bool check, PieceList check_depth[HEIGHT][WIDTH]){
    int length = strlen(lepes);
    if(length > 3){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = lepes[1] - 'a';
    int sor = lepes[2]-'0'-1;
    if(oszlop < 0 || sor < 0 || oszlop > 8 || sor > 8){
        printf("Nincs ilyen mező a pályán!\n");
        return 1;
    }
    if(!takes && table[sor][oszlop].type != EMPTY){
        printf("Ott van már bábu!\n");
        return 1;
    }
    else if(takes && table[sor][oszlop].color == turn){
        printf("A saját bábudat nem tudod leütni!\n");
        return 1;
    }    
    else if(takes && table[sor][oszlop].type == EMPTY){
        printf("Ott nincs bábu amit le lehet ütni!\n");
        return 1;
    }
    if(abs(sor-king[0]) > 1 || abs(oszlop-king[1]) > 1){
        printf("Oda nem tud lépni a király!\n");
        return 1;
    }
    if(check_depth[sor][oszlop].size > 0){
        printf("Ott sakban lennél!\n");
        return 1;
    }
    else if(KingCheck(sor,oszlop)){
        printf("Ott sakban lennél!\n");
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(turn == BLACK && check_depth_black[sor][oszlop].size > 0){
            printf("Ott is sakkban vagy!\n");
            return 1;
        }
        else if(turn == WHITE && check_depth_white[sor][oszlop].size > 0){
            printf("Ott is sakkban vagy!\n");
            return 1;
        }
        white_king_inCheck = false;
        black_king_inCheck = false;
    }

    if(takes){
        if(RemoveType(table,sor,oszlop,turn) == 1){
            return 1;
        };
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    if(CheckPlace(table[king[0]][king[1]], king[0], king[1], table[king[0]][king[1]].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(king[0], king[1], sor, oszlop, table, check, false, turn, takes) == 1) return 1;
    bool is_pawn_taken = table[sor][oszlop].type == PAWN;
    table[sor][oszlop] = table[king[0]][king[1]];
    table[king[0]][king[1]] = empty;
    king[0] = sor;
    king[1] = oszlop;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, false, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, king[0], king[1], sor, oszlop, BLACK, table, false, nothing, turn, takes, is_pawn_taken, KING) || 
    CheckWhichPawnAffects(black_pawn_moves, king[0], king[1], sor, oszlop, WHITE, table, false, nothing, turn, takes, is_pawn_taken, KING) == 1) return 1;
    if(!(*king_moved)){
        *king_moved = true;
    }
    return 0;
}
int ChangeRookPosition(int row, int column, int to_row, int to_column, int rooks_count, int rooks[PIECE_MAX_COUNT][2]){
    for (int i = 0; i < rooks_count; i++)
    {
        if(rooks[i][0] == row && rooks[i][1] == column){
            rooks[i][0] = to_row;
            rooks[i][1] = to_column;
            return 0;
        }
    }  
    return 1;
}
int Castle(int king[2], Piece table[HEIGHT][WIDTH], PieceColor turn, bool* king_moved, bool short_castle,
    bool left_rook_moved, bool right_rook_moved, int rook_moved_sor, int rook_moved_oszlop, Piece empty, 
    int rooks[PIECE_MAX_COUNT][2], int rooks_count, bool check, PieceList depth[HEIGHT][WIDTH], bool takes){
    // A Sáncolás megoldására szól ez a függvény
    if(black_king_inCheck || white_king_inCheck){
        printf("Sakkban vagy, nem tudsz sáncolni!\n");
        return 1;
    }
    if(*king_moved){
        printf("Már nem tudsz sáncolni mert léptél a királlyal!\n");
        return 1;
    }
    PieceColor color = short_castle ? WHITE : BLACK; // Melyik oldalt nézzük ahhozképest alakítjuk ki a többi részét
    if(short_castle && right_rook_moved){
        printf("Nem tudsz sáncolni mert léptél azzal a bástyával!\n");
        return 1;
    }
    else if(!short_castle && left_rook_moved){
        printf("Nem tudsz sáncolni mert léptél azzal a bástyával!\n");
        return 1;
    }
    for (int i = short_castle ? king[1]+1 : king[1]-1; short_castle ? i < 7 : i>0; short_castle ? i++ : i--)
    {   
        if(table[king[0]][i].type != EMPTY){
            printf("Van közte bábú!\n");
            return 1;
        }
        if(depth[king[0]][i].size > 0){
            printf("Nem tudsz sáncolni sakkon keresztül!\n");
            return 1;
        }
    }
    //int adjust_by_color = turn == WHITE ? 1 : -1;
    //int adjust = short_castle ? 0:1;
    int add_to_king = short_castle ? 2 : -2;
    int add_to_rook = short_castle ? -2 : 3;
    bool isCheck = false; 
    int rook_column = rook_moved_oszlop+add_to_rook;
    PieceColor opposite_color = turn == WHITE ? BLACK : WHITE;
    printf("col: %d\n",rook_column);
    for (int i = rook_moved_oszlop + (short_castle ? -1 : 1); short_castle ? i >= 0 : i <= 7; short_castle ? i-- : i++)
    {   
        if(table[rook_moved_sor][i].type != EMPTY){
            if(table[rook_moved_sor][i].type == KING && table[rook_moved_sor][i].color == opposite_color){
                isCheck = true;
            }
            break;
        }
    }
    if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    else if(!isCheck && check){
        printf("Ez a lépés nem ad sakkot!\n");
        return 1;
    }
    Piece table_piece;
    table_piece.type = KING;
    table_piece.color = turn;
    if(CheckPlace(table[king[0]][king[1]], king[0], king[1], table[king[0]][king[1]].color == WHITE ?
        check_depth_black : check_depth_white, table, TRUE) == 1){
        return 1;
    }
    if(CheckPlace(table[rook_moved_sor][rook_moved_oszlop], rook_moved_sor , rook_moved_oszlop, 
        table[rook_moved_sor][rook_moved_oszlop].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1)
    {
        return 1;
    }
    table[king[0]][king[1]+add_to_king] = table_piece;
    table[king[0]][king[1]] = empty;
    if(CheckWhenPieceMoves(king[0], king[1], king[0], king[1], table, false, false, turn, takes) == 1){
        return 1;
    }
    if(CheckWhenPieceMoves(rook_moved_sor, rook_moved_oszlop, rook_moved_sor, rook_moved_oszlop, table, false, false, turn, takes) == 1){
        return 1;
    }
    table[rook_moved_sor][rook_column] = table[rook_moved_sor][rook_moved_oszlop];
    table[rook_moved_sor][rook_moved_oszlop] = empty;
    king[1] = king[1]+add_to_king;
    if(CheckPlace(table[king[0]][king[1]], king[0], king[1], table[king[0]][king[1]].color == WHITE 
    ? check_depth_black : check_depth_white, table, FALSE) == 1)
    {
        return 1;
    }
    if(CheckPlace(table[rook_moved_sor][rook_column], rook_moved_sor, rook_column, 
        table[rook_moved_sor][rook_column].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1)
    {
        return 1;
    }
    /*if(CheckWhenPieceMoves(king[0], king[1], -1, -1, table, false, false, takes, takes) == 1){
        return 1;
    }
    if(CheckWhenPieceMoves(rook_moved_sor, rook_column, -1, -1, table, false, false, takes, takes) == 1){
        return 1;
    }*/
    ChangeRookPosition(rook_moved_sor, rook_moved_oszlop, rook_moved_sor, rook_column, rooks_count, rooks);
    *king_moved = true;
    if(turn == WHITE && isCheck){
        black_king_inCheck = true;
    } 
    else if (turn == BLACK && isCheck){
        white_king_inCheck = true;
    }
    return 0;
}
bool isPieceBetween(char type, int melyik_mezo, int sor, int oszlop, int target_sor, int target_oszlop, Piece table[HEIGHT][WIDTH]){
    int curr_sor = sor;
    int curr_oszlop = oszlop;
    int increment_sor = 0;
    int increment_oszlop = 0;
    int increment_mezo = -1;
    switch (type){
        case 'B':
            increment_sor = curr_sor > target_sor ? -1 : 1;
            increment_oszlop = curr_oszlop > target_oszlop ? -1 : 1;
            break;
        case 'R':
            increment_mezo = melyik_mezo == 0 ? (curr_sor > target_sor ? -1 : 1) : (curr_oszlop > target_oszlop ? -1 : 1);
            break;
        case 'Q':
            increment_mezo = melyik_mezo == 0 ? (curr_sor > target_sor ? -1 : 1) : (curr_oszlop > target_oszlop ? -1 : 1);
            increment_sor = curr_sor > target_sor ? -1 : 1;
            break;
    }
    do{
        if(melyik_mezo == 0){
            curr_sor+=increment_mezo;
        }
        else if (melyik_mezo == 1){
            curr_oszlop+=increment_mezo;
        }
        else{
            curr_sor+=increment_sor;
            curr_oszlop+=(type=='B'?increment_oszlop:increment_mezo);
        }
    } while(table[curr_sor][curr_oszlop].type == EMPTY && (curr_sor != target_sor || curr_oszlop != target_oszlop));
    return curr_sor == target_sor && curr_oszlop == target_oszlop;
}
int CheckInstanceBishop(int curr_row, int curr_column, int where_row, int where_column, int to_row, int i, PieceList* list, 
    PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], PieceColor opposite_color, int check, bool* isDiscoveredCheck){
    int add_row = curr_row > list->items[i].row ? 1 : -1;
    int add_column = curr_column > list->items[i].column ? 1 : -1;
    do{
        curr_row += add_row;
        curr_column += add_column;                  
    } while(table[curr_row][curr_column].type == EMPTY && curr_row < 7 && curr_column < 7 && curr_row > 0 && curr_column > 0);
    if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color && check == 1){
        printf("Ez a lépés sakkot adna!");
        return 1;
    }
    else if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color){
        *isDiscoveredCheck = true;
    }
    curr_row = where_row;
    curr_column = where_column;
    do{ 
        curr_row += add_row;
        curr_column += add_column;
        if(curr_row > 7 || curr_column > 7 || curr_row < 0 || curr_column < 0) break;
        if(to_row != -1)addPiece(&check_depth[curr_row][curr_column], list->items[i]);
        else removePiece(&check_depth[curr_row][curr_column], list->items[i]);                    
    } while(table[curr_row][curr_column].type == EMPTY && curr_row < 7 && curr_column < 7 && curr_row > 0 && curr_column > 0);
    return 0;
}
int CheckInstanceRook(int curr_row, int curr_column, int where_row, int where_column, int to_column, int i, PieceList* list, 
    PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], PieceColor opposite_color, int check, bool* isDiscoveredCheck){
    int melyik_mezo = list->items[i].row == curr_row ? 1 : 0;
    int add_row = curr_row > list->items[i].row ? 1 : -1;
    int add_column = curr_column > list->items[i].column ? 1 : -1;
    //printf("CheckRook\n");
    //PrintTable(table);
    do{
        if(melyik_mezo == 1){
            curr_column += add_column;
        }
        else{
            curr_row += add_row;                 
        }
        if(curr_row > 7 || curr_column > 7 || curr_row < 0 || curr_column < 0) break;
    } while(curr_row <= 7 && curr_column <= 7 && curr_row >= 0 && curr_column >= 0 && table[curr_row][curr_column].type == EMPTY);
    if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color && check == 1){
        printf("Ez a lépés sakkot adna!");
        return 1;
    }
    else if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color){
        *isDiscoveredCheck = true;
    }
    curr_row = where_row;
    curr_column = where_column;
    do{
        if(melyik_mezo == 1){
            curr_column += add_column;                
        }
        else{
            curr_row += add_row;
        }
        if(curr_row > 7 || curr_column > 7 || curr_row < 0 || curr_column < 0) break;
        //printf("row: %d column: %d\n",curr_row,curr_column);
        //printf("%s",to_column != -1 ? "add" :"remove");
        if(to_column != -1)addPiece(&check_depth[curr_row][curr_column], list->items[i]);
        else removePiece(&check_depth[curr_row][curr_column], list->items[i]);
    } while(curr_row <= 7 && curr_column <= 7 && curr_row >= 0 && curr_column >=0 && table[curr_row][curr_column].type == EMPTY);
    //PrintTable(table);
    return 0;
}
int CheckInstances(int where_row, int where_column, int to_row, int to_column, PieceList check_depth[HEIGHT][WIDTH], 
    Piece table[HEIGHT][WIDTH], PieceColor opposite_color, int check, bool* isDiscoveredCheck){
    // Ez a segédfüggvény csak azért van hogy a fehér, fekete check_depth listát is megnézzük
    PieceList* list = &check_depth[where_row][where_column];
    for (int i = 0; i < list->size; i++)
    {   
        if(list->items[i].piece == BISHOP || list->items[i].piece == ROOK || list->items[i].piece == QUEEN){
            int curr_row = where_row;
            int curr_column = where_column;
            switch(list->items[i].piece){
                case BISHOP:
                    if (CheckInstanceBishop(curr_row, curr_column, where_row, where_column, to_row, i, list, check_depth, table, opposite_color, check, isDiscoveredCheck) == 1) return 1;            
                    break;
                case ROOK:
                    if (CheckInstanceRook(curr_row, curr_column, where_row, where_column, to_column, i, list, check_depth, table, opposite_color, check, isDiscoveredCheck) == 1) return 1;            
                    break;
                case QUEEN:
                    //printf("Queen: %d:%d\n",list->items[i].row,list->items[i].column);
                    if(abs(list->items[i].row - curr_row) == abs(list->items[i].column - curr_column)){
                        if(CheckInstanceBishop(curr_row, curr_column, where_row, where_column, to_row, i, list, check_depth, table, opposite_color, check, isDiscoveredCheck) == 1) return 1;  
                    }
                    else{
                        if(CheckInstanceRook(curr_row, curr_column, where_row, where_column, to_column, i, list, check_depth, table, opposite_color, check, isDiscoveredCheck) == 1) return 1;  
                    }
                    break;               
                default:
                    break;
            }   
        }
    }
    return 0;
}
int CheckWhenPieceMoves(int where_row, int where_column, int to_row, int to_column, Piece table[HEIGHT][WIDTH], bool check, bool isCheck, PieceColor turn, bool takes){
    // Első meghívásnál:
    // A honnan részével foglalkozunk a bábuval hogy milyen hatással lesz azokra a bábukra amik látják ezt a mezőt ha elmozdul
    // Második meghívásnál:
    // A hova részével foglalkozunk a bábuval hogy milyen hatással lesz azokra a bábukra amik látják ezt a mezőt ha elmozdul
    bool isDiscoveredCheck = false;
    bool checking_black = false;
    bool checking_white = false;
    //printf("row %d,col: %d\n",where_row,where_column);
    //printf("white\n");
    if(to_row == -1 && takes){
        return 0;
    }
    if(CheckInstances(where_row, where_column, to_row, to_column, check_depth_white, table, WHITE, check, &isDiscoveredCheck) == 1 ){
        return 1;
    }
    //printf("black\n");
    if(CheckInstances(where_row, where_column, to_row, to_column, check_depth_black, table, BLACK, check, &isDiscoveredCheck) == 1 ){
        return 1;
    }


    if(!isDiscoveredCheck && !isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    if(turn == WHITE && checking_white){
        printf("Ez a lépés sakkot adna neked!\n");
        return 1;
    }
    else if(turn == BLACK && checking_black){
        printf("Ez a lépés sakkot adna neked!\n");
        return 1;
    }
    if(checking_black){
        black_king_inCheck = true;
    }
    else if(checking_white){
        white_king_inCheck = true;
    }
    return 0;
}
int CheckPlace(Piece piece_type, int sor, int oszlop, PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], Bool remove){  
    // megnézi melyik területetek látja a bábu és a remove paraméter szerint törli vagy egészíti ki a látóterét
    switch (piece_type.type)
    {
        case KNIGHT:
            int indulas[2] = {sor+2, oszlop-1};
            for (int i = 1; i <= 8; ++i)
            {       
                if (indulas[0]>=0 && indulas[0]<=7 && indulas[1]>=0 && indulas[1]<=7)
                {   
                    PiecePlace add;
                    add.row = sor;
                    add.column = oszlop;
                    add.piece = KNIGHT;
                    if(remove == FALSE) addPiece(&check_depth[indulas[0]][indulas[1]], add);
                    else removePiece(&check_depth[indulas[0]][indulas[1]],add);  
                }
                if(i<4 && i%2 == 1){
                    indulas[1] = indulas[1] + 2;
                }
                else if (i<4 && i%2 == 0){
                    indulas[0] = sor - 2;
                    indulas[1] = oszlop - 1;
                }
                else if (i==4){
                    indulas[0] = sor - 1;
                    indulas[1] = oszlop - 2;
                }
                else if(i>4 && i%2 == 1){
                    indulas[0] = indulas[0] + 2;
                }
                else{
                    indulas[0] = sor - 1;
                    indulas[1] = oszlop + 2;
                }
            } 
            return 0;
        case KING:
            int base_sor = sor-1;
            int base_oszlop = oszlop-1;
            for (int i = 0; i < 3; i++)
            {   
                int curr_sor = base_sor+i;
                for (int y = 0; y < 3; y++)
                {
                    int curr_oszlop = base_oszlop+y;
                    if(curr_sor>=0 && curr_sor<8 && curr_oszlop>=0 && curr_oszlop<8 && (curr_sor != sor || curr_oszlop != oszlop)){
                        PiecePlace add;
                        add.row = sor;
                        add.column = oszlop;
                        add.piece = piece_type.type;
                        if(remove == FALSE) addPiece(&check_depth[curr_sor][curr_oszlop],add);
                        else removePiece(&check_depth[curr_sor][curr_oszlop],add);                   
                    }
                }             
            }
            return 0;
        case PAWN:
            PiecePlace add;
            int curr_row = piece_type.color == WHITE ? sor - 1 : sor + 1;
            add.row = curr_row;
            add.piece = PAWN;
            if(oszlop-1>=0){
                add.column = oszlop-1;
                if(remove == FALSE) addPiece(&check_depth[curr_row][oszlop-1], add);
                else removePiece(&check_depth[curr_row][oszlop-1],add); 
            }
            if(oszlop+1<=7){
                add.column = oszlop+1;
                if(remove == FALSE) addPiece(&check_depth[curr_row][oszlop+1], add);
                else removePiece(&check_depth[curr_row][oszlop+1],add); 
            }
            return 0;
        case BISHOP:
            break;
        case ROOK:
            break;
        case QUEEN:
            break;
        default:
            return 1;
    }
    int meddig = piece_type.type == QUEEN ? 8 : 4;
    for (int i = 0; i < meddig; i++)
    {         
        int curr_sor = sor;
        int curr_oszlop = oszlop;
        int increment_sor = (i%4)<=1 ? -1 : 1;
        int increment_oszlop = (i%4) % 2 == 0 ? -1 : 1;
        int increment_mezo = (i%4)<=1 ? -1 : 1;
        int which_row = piece_type.type == QUEEN ? i / 4 > 0 ? 0 : 1 : -1;
        while(table[curr_sor][curr_oszlop].type == EMPTY || (curr_sor == sor && curr_oszlop == oszlop)){
            if(piece_type.type == BISHOP || which_row == 0){
                curr_sor += increment_sor;
                curr_oszlop += increment_oszlop;
            }
            else if(piece_type.type == ROOK || which_row == 1){
                if(i%2 == 0){
                    curr_oszlop+=increment_mezo;
                }
                else{
                    curr_sor+=increment_mezo;
                }
            }
            if(curr_sor > 7 || curr_sor < 0 || curr_oszlop > 7 || curr_oszlop < 0 || table[curr_sor][curr_oszlop].type != EMPTY) break;
            PiecePlace add;
            add.row = sor;
            add.column = oszlop;
            add.piece = piece_type.type;
            if(remove == FALSE) addPiece(&check_depth[curr_sor][curr_oszlop],add);
            else removePiece(&check_depth[curr_sor][curr_oszlop],add);
        }
        if(curr_sor <= 7 && curr_sor >= 0 && curr_oszlop <= 7 && curr_oszlop >= 0){
            PiecePlace add;
            add.row = sor;
            add.column = oszlop;
            add.piece = piece_type.type;
            if(remove == FALSE) addPiece(&check_depth[curr_sor][curr_oszlop],add);
            else removePiece(&check_depth[curr_sor][curr_oszlop],add);
        }
    }
    return 0;
}
bool IsDepthNotPawnOrKing(PiecePlace* check_depth, int size){
    if(size <= 0) return false;
    for (int i = 0; i < size; i++)
    {
        if(check_depth[i].piece != PAWN && check_depth[i].piece != KING){
            //printf("Piece: %s\n",getPiece(WHITE,check_depth[i].piece));
            return true;
        }
    }
    return false;
}
bool isMate(Piece table[HEIGHT][WIDTH], PieceList check_depth[HEIGHT][WIDTH], PieceList opposite_check_depth[HEIGHT][WIDTH], int king[2], PieceList pawn_moves[HEIGHT][WIDTH]){
    if(opposite_check_depth[checkingPiece.row][checkingPiece.column].size > 0 && check_depth[checkingPiece.row][checkingPiece.column].size <= 0) return false;
    int curr_row = king[0]-1;
    int curr_column = king[1]-1;
    //printf("row: %d, col: %d\n",curr_row,curr_column);
    for (int i = 0; i < 3; i++)
    {   
        if (curr_row >= 0 && curr_row <= 7){
            for (int j = 0; j < 3; j++)
            {   
                if(curr_column <= 7 && curr_column >= 0 && !KingCheck(curr_row,curr_column) && (table[curr_row][curr_column].type == EMPTY && check_depth[curr_row][curr_column].size <= 0)){
                    printf("Can move to:%d %d\n ",curr_row,curr_column);
                    return false;
                }   
                curr_column += 1;
            }
        }
        curr_column = king[1]-1;
        curr_row += 1;
    }
    if(checkingPiece.piece == BISHOP || (checkingPiece.piece == QUEEN && abs(checkingPiece.row - king[0]) == abs(checkingPiece.column - king[1]))){
        int row_inc = king[0]-checkingPiece.row > 0 ? 1 : -1;
        int col_inc = king[1]-checkingPiece.column > 0 ? 1 : -1;
        int starting_row = checkingPiece.row;
        int starting_col = checkingPiece.column;
        //printf("Starting: %d:%d\n",starting_row,starting_col);
        starting_row += row_inc;
        starting_col += col_inc;
        while (table[starting_row][starting_col].type != KING){    
            //printf("Curr: %d:%d\n",starting_row,starting_col);
            if(IsDepthNotPawnOrKing(opposite_check_depth[starting_row][starting_col].items, opposite_check_depth[starting_row][starting_col].size) || 
            pawn_moves[starting_row][starting_col].size > 0){
                printf("Can block on: %d/%d size: %d\n",starting_row,starting_col,pawn_moves[starting_row][starting_col].size);
                return false;
            }
            starting_row += row_inc;
            starting_col += col_inc;
        }
       
    }
    else if(checkingPiece.piece == ROOK || checkingPiece.piece == QUEEN){
        bool row_or_col = king[0] == checkingPiece.row;
        int row_inc = king[0]-checkingPiece.row > 0 ? 1 : -1;
        int col_inc = king[1]-checkingPiece.column > 0 ? 1 : -1;
        int starting_row = checkingPiece.row;
        int starting_col = checkingPiece.column;
        if (row_or_col) starting_col += col_inc;
        else starting_row += row_inc;
        while (table[starting_row][starting_col].type != KING) { 
            if(row_or_col) starting_col += col_inc;
            else starting_row += row_inc;
            if(IsDepthNotPawnOrKing(opposite_check_depth[starting_row][starting_col].items, opposite_check_depth[starting_row][starting_col].size) || 
            pawn_moves[starting_row][starting_col].size > 0){
                printf("Can block on: %d/%d\n",starting_row,starting_col);
                return false;
            }
        } 
    }
    return true;
}
void addPiece(PieceList* list, PiecePlace piece) {
    //printf("Added before, size: %d\n",list->size);
    if(list->capacity <= 0 && list->size <= 0){
        list->capacity = 2;
        list->items = malloc(sizeof(PiecePlace) * list->capacity);
        if (!list->items) {
            perror("malloc failed");
            exit(1);
        }
        list->size = 0;
    }
    else if (list->size >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 2 : list->capacity * 2;
        list->items = realloc(list->items, list->capacity * sizeof(PiecePlace));
        if (!list->items) {
            perror("realloc failed");
            exit(1);
        }
    }
    list->items[list->size++] = piece;
    //printf("Added, size: %d\n",list->size);
}
void removePiece(PieceList* list,PiecePlace piece){ 
    Bool found = FALSE;
    //printf("---\n");
    //printf("new: %d \n",list->size);
    //printf("removing: %d:%d\n",piece.row,piece.column);
    for (int i = 0; i < list->size; i++)
    {       
        //printf("%d\n",i);
        if(found == FALSE && list->items[i].column==piece.column && list->items[i].row==piece.row && list->items[i].piece==piece.piece){
            found=TRUE;
        }
        if(found == TRUE){
            list->items[i]=list->items[i+1];
        }
    }
    list->size--;
    if(list->size==0){
        freePieceList(list);
    }
    else{
        list->items = realloc(list->items, list-> capacity * sizeof(PiecePlace));
        if (!list->items) {
            perror("realloc failed");
            exit(1);
        }
    }
}
void freePieceList(PieceList* list) {
    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}
void piece_list_copy(PieceList dest[HEIGHT][WIDTH], PieceList src[HEIGHT][WIDTH])
{   
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dest[i][j].size = src[i][j].size;
            dest[i][j].capacity = src[i][j].capacity;
            if (src[i][j].capacity > 0) {
                dest[i][j].items = malloc(src[i][j].capacity * sizeof(PiecePlace));
                if (!dest[i][j].items) {
                    printf("malloc hiba\n");
                    exit(1);
                }
                
                for (int y = 0; y < src[i][j].size; y++) {
                    dest[i][j].items[y] = src[i][j].items[y];
                }
            } else {
                dest[i][j].items = NULL;
            }
        }
    }
}
void freeAllPieceList(PieceList dest[HEIGHT][WIDTH])
{   
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            free(dest[i][j].items);
        }
    }
}
void clearLastDoubleMove(PiecePlace* last_double_move){
    (*last_double_move).piece = EMPTY;
    (*last_double_move).row = -1;
    (*last_double_move).column = -1;
}
int main(){
    //Terminálba: chcp 65001
    printf("\033[8;30;120t");
    Piece table[HEIGHT][WIDTH];
    empty.color = NOTHING;
    empty.type = EMPTY;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            table[i][j] = empty;
        }     
    }
    char moves[50][MOVE_MAX_LENGTH];
    int moves_count = 0;

    int white_bishop_count = 0;
    int black_bishop_count = 0;
    int white_rook_count = 0;
    int black_rook_count = 0;
    int white_queen_count = 0;
    int black_queen_count = 0;

    p_wbishopcount = &white_bishop_count;
    p_bbishopcount = &black_bishop_count;
    p_wrookcount = &white_rook_count;
    p_brookcount = &black_rook_count;
    p_wqueencount = &white_queen_count;
    p_bqueencount = &black_queen_count;

    bool black_king_moved = false;
    bool white_king_moved = false;
    bool left_black_rook_moved = false;
    bool right_black_rook_moved = false;

    bool left_white_rook_moved = false;
    bool right_white_rook_moved = false;
    nothing.piece = EMPTY;
    nothing.row = -1;
    nothing.column = -1;
    PiecePlace last_double_move;
    last_double_move.piece = EMPTY;
    Initialize(table);
    int vege = 0;
    int lepesek_szama = 1;
    int* p_lepesek = &lepesek_szama;
    PrintMove print_moves;
    print_moves.length = 0;
    print_moves.curr_position = 0;
    PieceColor turn = WHITE;
    printf("\033[2J");
    PrintTable(table,print_moves);
    while (vege == 0)
    {   
        printf("\033[2K");
        printf("%s%s %d. lépése: ",SPACE,turn == WHITE ? "Fehér" : "Fekete", lepesek_szama);
        char lepes[MOVE_MAX_LENGTH];
        scanf("%20s", lepes);
        printf("\033[2K");
        printf("%s",SPACE);
        char actual_lepes[MOVE_MAX_LENGTH];
        strcpy(actual_lepes,lepes);
        bool takes = false;
        bool check = false;
        if(strchr(lepes, 'x') != NULL){
            int j = 0;
            takes = true;
            for (int i = 0; lepes[i] != '\0'; i++) {
                if (lepes[i] != 'x') {
                    lepes[j++] = lepes[i];
                }
            }
            lepes[j] = '\0'; 
        }
        if(strchr(lepes, '+') != NULL){
            int j = 0;
            check = true;
            for (int i = 0; lepes[i] != '\0'; i++) {
                if (lepes[i] != '+') {
                    lepes[j++] = lepes[i];
                }
            }
            lepes[j] = '\0'; 
        }
        PieceList temp_white[HEIGHT][WIDTH];
        PieceList temp_black[HEIGHT][WIDTH];
        PiecePlace temp_last_double_move = last_double_move;
        bool white_king_inCheck_temp = white_king_inCheck;
        bool black_king_inCheck_temp = black_king_inCheck;
        piece_list_copy(temp_white, check_depth_white);
        piece_list_copy(temp_black, check_depth_black);
        
        bool isMoveDouble = false;
        bool isCorrect = false;
        bool wasMoveDouble = false;
        if(lepes[0] >= 'a' && lepes[0] <= 'h'){
            if(!takes && last_double_move.piece != EMPTY){
                last_double_move.piece = EMPTY;
                last_double_move.row = -1;
                last_double_move.column = -1;
                wasMoveDouble = true;
            }
            if(PawnMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck, table, turn, takes, check, &last_double_move, &isMoveDouble) == 0)
            {
                isCorrect = true;
            }
        }
        else if(lepes[0] == 'N'){
            if(KnightMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck, table, turn, takes, check) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if(lepes[0] == 'B'){
            if(BishopMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck, turn == WHITE? white_bishops : black_bishops, turn == WHITE? p_wbishopcount : p_bbishopcount,table,turn, takes, check) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if(lepes[0] == 'R'){
            if(RookMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck,
                turn == WHITE? white_rooks : black_rooks, turn == WHITE? p_wrookcount : p_brookcount, table, turn, 
                turn == WHITE ? &left_white_rook_moved : &left_black_rook_moved, turn == WHITE ? &right_white_rook_moved : &right_black_rook_moved, takes, check) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if(lepes[0] == 'Q'){
            if(QueenMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? 
                &black_king_inCheck : &white_king_inCheck, turn == WHITE? white_queens : black_queens,
                 turn == WHITE? p_wqueencount : p_bqueencount, table, turn, takes, check) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if(lepes[0] == 'K'){
            if(KingMove(lepes, turn == WHITE? white_king : black_king, table, turn, turn == WHITE ? 
                &white_king_moved : &black_king_moved, takes, check, turn == WHITE ? check_depth_white : check_depth_black) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if(strcmp(lepes,"O-O") == 0){
            if(Castle(turn == WHITE ? white_king : black_king, table, turn, turn == WHITE ? &white_king_moved : &black_king_moved, true, turn == WHITE?
            left_white_rook_moved : left_black_rook_moved, turn == WHITE ? right_black_rook_moved : right_white_rook_moved, turn == WHITE ? 7 : 0, 7, empty, 
            turn == WHITE ? white_rooks : black_rooks, turn == WHITE ? white_rook_count : black_rook_count, 
            check, turn == WHITE ? check_depth_white : check_depth_black, takes) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }   
        else if (strcmp(lepes,"O-O-O") == 0){
            if(Castle(turn == WHITE ? white_king : black_king, table, turn, turn == WHITE ? &white_king_moved : &black_king_moved, false, turn == WHITE?
            left_white_rook_moved : left_black_rook_moved, turn == WHITE ? right_black_rook_moved : right_white_rook_moved, turn == WHITE ? 7 : 0, 0, 
            empty, turn == WHITE ? white_rooks : black_rooks, turn == WHITE ? white_rook_count : black_rook_count, 
            check, turn == WHITE ? check_depth_white : check_depth_black, takes) == 0){
                isCorrect = true;
            }
            clearLastDoubleMove(&last_double_move);
        }
        else if (strcmp(lepes, "moves") == 0){
            FILE *file = fopen("moves.txt", "w");
            if (file == NULL) {
                printf("Nem sikerült megnyitni a fájlt.\n");
                return 1;
            }
            for (int i = 0; i < moves_count; i++)
            {
                fprintf(file,moves[i]);
                if(i!= moves_count-1) fprintf(file,"\n");
            }
            fclose(file);
            continue;
        }
        else{
            printf("Nincs ilyen lépés!\n");
        }
        if(isCorrect){
            if(wasMoveDouble){
            if(turn == WHITE){
                while(black_pawn_moves[temp_last_double_move.row-1][temp_last_double_move.column].size > 0){
                    removePiece(&black_pawn_moves[temp_last_double_move.row-1][temp_last_double_move.column],
                        black_pawn_moves[temp_last_double_move.row-1][temp_last_double_move.column].items[0]);
                    }
                }
                else{
                    while(white_pawn_moves[temp_last_double_move.row+1][temp_last_double_move.column].size > 0){
                        removePiece(&black_pawn_moves[temp_last_double_move.row+1][temp_last_double_move.column],
                            white_pawn_moves[temp_last_double_move.row+1][temp_last_double_move.column].items[0]);
                        }
                    }
                    last_double_move.piece = EMPTY;
                    last_double_move.row = -1;
                    last_double_move.column = -1;
                    
            }
            if(check && isMate(table, turn == WHITE ? check_depth_black : check_depth_white, turn == WHITE ? check_depth_white : check_depth_black,
                turn == WHITE ? black_king : white_king, turn == WHITE ? white_pawn_moves : black_pawn_moves
            )){
                printf("Sakk Matt\nA %s nyert! gg\n",turn == WHITE ? "Fehér" : "Fekete");
                break;
            }
            strcpy(print_moves.moves[print_moves.curr_position].move,actual_lepes);
            print_moves.moves[print_moves.curr_position++].current_turn = *p_lepesek;
            if(print_moves.length != 16){
                print_moves.length++;
            } else {
                print_moves.length = 1;
            }
            if(print_moves.curr_position == 16){
                print_moves.curr_position = 0;
            } 
            turn = turn == WHITE ? BLACK : WHITE;
            if(turn == WHITE) *p_lepesek = *p_lepesek + 1; 
            strcpy(moves[moves_count++], actual_lepes);
        }
        else{
            CleanDepthList(temp_white, temp_black, white_king_inCheck_temp, black_king_inCheck_temp, &last_double_move, temp_last_double_move);
        }
        freeAllPieceList(temp_white);
        freeAllPieceList(temp_black);
        PrintTable(table,print_moves);
    }
    return 0;
}