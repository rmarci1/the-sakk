#include "list.h"
void addPiece(PieceList* list, PiecePlace piece){
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
void freePieceList(PieceList* list){
    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}
void piece_list_copy(PieceList dest[HEIGHT][WIDTH], PieceList src[HEIGHT][WIDTH]){   
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
void freeAllPieceList(PieceList dest[HEIGHT][WIDTH]){   
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            free(dest[i][j].items);
        }
    }
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
void CleanDepthList(PieceList temp_white[HEIGHT][WIDTH], PieceList temp_black[HEIGHT][WIDTH], bool white_king_inCheck_temp, 
    bool black_king_inCheck_temp, PiecePlace* last_double_move, PiecePlace last_double_move_temp, Location temp_from_move, Location temp_to_move, bool temp_double_check){
    piece_list_copy(check_depth_white,temp_white);
    piece_list_copy(check_depth_black,temp_black);
    white_king_inCheck = white_king_inCheck_temp;
    black_king_inCheck = black_king_inCheck_temp;
    *last_double_move = last_double_move_temp;
    from_move = temp_from_move;
    to_move = temp_to_move;
    double_check = temp_double_check;
}