#include "board.h"
int CheckInstanceBishop(int curr_row, int curr_column, int where_row, int where_column, int to_row, int i, PieceList* list, 
    PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], PieceColor opposite_color, bool check, bool* isDiscoveredCheck){
    int add_row = curr_row > list->items[i].row ? 1 : -1;
    int add_column = curr_column > list->items[i].column ? 1 : -1;
    do{
        curr_row += add_row;
        curr_column += add_column;                  
    } while(table[curr_row][curr_column].type == EMPTY && curr_row < 7 && curr_column < 7 && curr_row > 0 && curr_column > 0);
    if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color && !check){
        printf("Ez a lépés sakkot adna!\n");
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
    PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], PieceColor opposite_color, bool check, bool* isDiscoveredCheck){
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
    if(table[curr_row][curr_column].type == KING && table[curr_row][curr_column].color == opposite_color && !check){
        printf("Ez a lépés sakkot adna!\n");
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
    Piece table[HEIGHT][WIDTH], PieceColor opposite_color, bool check, bool* isDiscoveredCheck){
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
    if(to_row != -1 && check && isCheck && isDiscoveredCheck && table[where_row][where_column].type != KING){
        double_check = true;
    }
    if(to_row != -1 && IsCheckEqualsToMove(isCheck,check) == 1) return 1;
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
bool IsCheck(char type, int sor, int oszlop, int king[2], Piece table[HEIGHT][WIDTH]){
    if(abs(sor - king[0]) == abs(oszlop - king[1]) && (type == 'B' || type == 'Q')){
        return isPieceBetween(type,2,sor,oszlop,king[0],king[1],table);
    } else if((sor == king[0] || oszlop == king[1]) && (type == 'R' || type == 'Q')){
        int melyik_mezo = sor == king[0] ? 1 : 0;
        return isPieceBetween(type,melyik_mezo,sor,oszlop,king[0],king[1],table);
    }
    return false;
}
bool IsKnightCheck(Piece table[HEIGHT][WIDTH], PieceColor opposite, int sor, int oszlop){
    int indulas[2] = {sor+2, oszlop-1};
    for (int i = 1; i <= 8; ++i)
    {       
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
bool KingCheck(int sor, int oszlop, bool takes){
    return (!(takes && checkingPiece.row == sor && checkingPiece.column == oszlop) && (
           ((checkingPiece.piece == ROOK || checkingPiece.piece == QUEEN) && (sor == checkingPiece.row || oszlop == checkingPiece.column)) || 
           ((checkingPiece.piece == BISHOP || checkingPiece.piece == QUEEN) && (abs(sor-checkingPiece.row) == abs(oszlop-checkingPiece.column)))));
}
int IsCheckChanged(Piece table[HEIGHT][WIDTH], int sor, int oszlop, int hova_sor, int hova_oszlop, bool takes, int king[2]){
    Piece temp_from = table[hova_sor][hova_oszlop];
    Piece temp_to = table[sor][oszlop];
    table[hova_sor][hova_oszlop] = table[sor][oszlop];
    table[sor][oszlop] = empty;
    bool isChanged = true;
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
            int melyik_mezo = checkingPiece.row == king[0] ? 1 : checkingPiece.column == king[1] ? 0 : 2;
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
int IsCheckEqualsToMove(bool isCheck, bool check){
    if(!isCheck && check){
        printf("Ez nem sakk!\n");
        return 1;
    }
    else if(isCheck && !check){
        printf("Ez a lépés sakkot adna!\n");
        return 1;
    }
    return 0;
}
int ChangingTablePositions(Piece table[HEIGHT][WIDTH], int from_row, int from_col, int row, int col, bool check, bool isCheck, 
    PieceColor turn, bool takes, bool is_pawn_taken, PiecePlace last_double_move){
    int canBlock = CanBlock(table, table[from_row][from_col].color == WHITE ? check_depth_white : check_depth_black, from_row, from_col, turn);
    if(!canBlock){
        printf("Sakkban lennél!");
        return 1;
    }
    if(CheckPlace(table[from_row][from_col], from_row, from_col, table[from_row][from_col].color == WHITE ? check_depth_black : check_depth_white, table, true) == 1) return 1;
    if(CheckWhenPieceMoves(from_row, from_col, row, col, table, check, isCheck, turn, takes) == 1) return 1;
    if(table[from_row][from_col].type == PAWN){
        if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, from_row, from_col, true, table) == 1){
            return 1;
        }
        if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, row, col, false, table) == 1){
            return 1;
        }
    }
    table[row][col] = table[from_row][from_col];
    table[from_row][from_col] = empty;
    if(CheckPlace(table[row][col], row, col, table[row][col].color == WHITE ? check_depth_black : check_depth_white, table, false) == 1) return 1;
    if(CheckWhenPieceMoves(row, col, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckWhichPawnAffects(white_pawn_moves, from_row, from_col, row, col, BLACK, table, true, last_double_move, turn, takes, is_pawn_taken, PAWN) || 
    CheckWhichPawnAffects(black_pawn_moves, from_row, from_col, row, col, WHITE, table, true, last_double_move, turn, takes, is_pawn_taken, PAWN) == 1) return 1;
    ChangingFromMovePosition(from_row, from_col);
    ChangingToMovePosition(row, col);
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
int ChangingKingInCheck(Piece table[HEIGHT][WIDTH], bool isCheck, int check, bool* king_inCheck, int row, int col, PieceColor turn){
    if(isCheck && check){
        *king_inCheck = true;
        checkingPiece.piece = table[row][col].type;
        checkingPiece.row = row;
        checkingPiece.column = col;
    }
    if((turn == WHITE && white_king_inCheck == 1) || (turn == BLACK && black_king_inCheck == 1)){
        if(turn == WHITE){
            white_king_inCheck = 0;
        }
        else{
            black_king_inCheck = 0;
        }
    }
    return 0;
}
int CheckPlace(Piece piece_type, int sor, int oszlop, PieceList check_depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH], bool remove){  
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
                    if(!remove) addPiece(&check_depth[indulas[0]][indulas[1]], add);
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
                        if(!remove) addPiece(&check_depth[curr_sor][curr_oszlop],add);
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
                if(!remove) addPiece(&check_depth[curr_row][oszlop-1], add);
                else removePiece(&check_depth[curr_row][oszlop-1],add); 
            }
            if(oszlop+1<=7){
                add.column = oszlop+1;
                if(!remove) addPiece(&check_depth[curr_row][oszlop+1], add);
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
            if(!remove) addPiece(&check_depth[curr_sor][curr_oszlop],add);
            else removePiece(&check_depth[curr_sor][curr_oszlop],add);
        }
        if(curr_sor <= 7 && curr_sor >= 0 && curr_oszlop <= 7 && curr_oszlop >= 0){
            PiecePlace add;
            add.row = sor;
            add.column = oszlop;
            add.piece = piece_type.type;
            if(!remove) addPiece(&check_depth[curr_sor][curr_oszlop],add);
            else removePiece(&check_depth[curr_sor][curr_oszlop],add);
        }
    }
    return 0;
}
void RemovePieceFromDepth(int sor, int oszlop, PieceList depth[HEIGHT][WIDTH], Piece table[HEIGHT][WIDTH]){
    CheckPlace(table[sor][oszlop], sor, oszlop, depth, table, true);
}
Piece PieceinDirection(Piece table[HEIGHT][WIDTH], int from_row, int from_col, int row, int col){
    // 0:row 1:col 2:diagonal
    int inc_row = row == from_row ? 0 : row > from_row ? 1 : -1;
    int inc_col = col == from_col ? 0 : col > from_col ? 1 : -1;
    int curr_row = row;
    int curr_col = col;
    //printf("\nfrom: %d:%d to: %d:%d P: %s\n",from_col,from_col,row,col,table[curr_row][curr_col].type != EMPTY ? "YEAH" : "NAY");
    while(table[curr_row][curr_col].type == EMPTY && curr_row < 7 && curr_row > 0 && curr_col < 7 && curr_col > 0){
        curr_row += inc_row;
        curr_col += inc_col;
        //printf("CurrInc:%d:%d\n",curr_row,curr_col);
    }
    //printf("Done\n");
    return table[curr_row][curr_col];
     
}
bool CanBlock(Piece table[HEIGHT][WIDTH], PieceList check_depth[HEIGHT][WIDTH], int row, int col, PieceColor king_color){
    Piece temp = table[row][col];
    table[row][col] = empty;
    for (int i = 0; i < check_depth[row][col].size; i++)
    {   
        PieceType p = check_depth[row][col].items[i].piece;
        if(p == PAWN || p == KING || p == KNIGHT) continue;
        Piece piece = PieceinDirection(table,check_depth[row][col].items[i].row,check_depth[row][col].items[i].column,row,col);
        if(piece.color == king_color && piece.type == KING){
            table[row][col] = temp;
            return false;
        }
    }
    table[row][col] = temp;
    return true;
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
    PieceColor opposite){
    // Megnézi hogy milyen gyalog lépés típusokra adjon a pawn_moves listához; 
    if(table[row+(add*-1)][column+1].type == PAWN && table[row+(add*-1)][column+1].color == opposite){
        pawn.row = row+add*-1;
        pawn.column = column+1;
        addPiece(&pawn_moves[row][column], pawn);
    }
    if(table[row+(add*-1)][column-1].type == PAWN && table[row+(add*-1)][column-1].color == opposite){
        pawn.row = row+add*-1;
        pawn.column = column-1;
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
    return 0;
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
        int addTo = turn == WHITE ? 1 : -1; 
        int actual_to_row = enPassant ? to_row + addTo : to_row;
        table[from_row][from_column] = temp;
        CheckPawnMoves(opposite, pawn_moves, actual_to_row, to_column, true, table);
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
    CheckPawnTypes(pawn_moves, to_row, to_column, add, table, pawn, opposite);
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