#include "piece.h"
#include "move.h"
#include <stdlib.h>
#include <string.h>
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
        if(Promote(lepes[1]-'0'-1, lepes[0] - 'a', lepes[0] - 'a', lepes[2], table, check, turn, takes, king, king_inCheck) == 1) return 1;
        return 0;
    }
    int length = strlen(lepes);
    if(length != 2 || lepes[0] < 'a' || lepes[0] > 'h' || lepes[1] < '1' || lepes[1] > '8'){
        printf("Nincs ilyen lépés!\n");
        return 1;
    }
    int oszlop = lepes[0] - 'a';
    int sor = 8 - (lepes[1]-'0');
    printf("\nsor: %d\n",sor);
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
        if(IsCheckChanged(table,hol_sor,oszlop,sor,oszlop,false,turn == WHITE ? white_king : black_king) == 1){
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
    if(abs(hol_sor - sor) == 2){
        *isMoveDouble = true;
        (*last_double_move).piece = PAWN;
        (*last_double_move).row = sor;
        (*last_double_move).column = oszlop;
    }
    if(ChangingTablePositions(table,hol_sor,oszlop,sor,oszlop,check,isCheck,turn,takes,false,*last_double_move) == 1) return 1;
    if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
    return 0;
}
int PawnTakes(char lepes[MOVE_MAX_LENGTH], Piece table[HEIGHT][WIDTH], PieceColor turn, 
    bool check, PiecePlace last_double_move, bool promote, int king[2], bool* king_inCheck, bool takes)
{       
    int honnan_oszlop = lepes[0] - 'a';
    int oszlop = lepes[1] - 'a';
    int sor = lepes[2]-'0'-1;
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
                if(Promote(sor, oszlop, honnan_oszlop, lepes[3], table, check, turn, takes, king, king_inCheck) == 1){
                    return 1;
                }
                return 0;
            }
            bool isCheck = false;
            if(king[0]-sor == -1 && abs(king[1]-oszlop) == 1){
                isCheck = true;
            }
            if(ChangingTablePositions(table, sor+1, honnan_oszlop, sor, oszlop, check, isCheck, turn, takes, table[sor][oszlop].type == PAWN, last_double_move) == 1) return 1;
            if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
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
                if(Promote(sor, oszlop, honnan_oszlop, lepes[3], table, check, turn, takes, king, king_inCheck) == 1){
                    return 1;
                }
                return 0;
            }
            bool isCheck = false;
            if(king[0]-sor == 1 && abs(king[1]-oszlop) == 1){
                isCheck = true;
            }
            if(ChangingTablePositions(table, sor-1, honnan_oszlop, sor, oszlop, check, isCheck, turn, takes, table[sor][oszlop].type == PAWN, last_double_move) == 1) return 1;
            if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
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
int EnPassant(int from_column, int row, int column, PiecePlace last_double_move, PieceColor turn, Piece table[HEIGHT][WIDTH], bool check, bool takes){
    int add = turn == WHITE ? 1 : -1;
    enPassant = true;
    if(last_double_move.piece != EMPTY && last_double_move.row == (row+add) && last_double_move.column == column)
    {   
        RemovePieceFromDepth(row+add, column, turn == WHITE ? check_depth_white : check_depth_black, table);
        if(CheckPlace(table[row+add][from_column], row+add, from_column, table[row+add][from_column].color == WHITE ? check_depth_black : check_depth_white, table, TRUE) == 1){
            return 1;
        }
        if(CheckWhenPieceMoves(row+add, from_column, row, column, table, check, true, turn, takes) == 1){
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
        if(CheckPlace(table[row][column], row, column, table[row][column].color == WHITE ? check_depth_black : check_depth_white, table, FALSE) == 1){
            return 1;
        }
        if (CheckWhenPieceMoves(row, column, -1, -1, table, check, true, turn, takes) == 1){
            return 1;
        }
        if(CheckWhichPawnAffects(white_pawn_moves, row+add, from_column, row, column, BLACK, table, true, last_double_move, turn, true, true, PAWN) || 
        CheckWhichPawnAffects(black_pawn_moves, row+add, from_column, row, column, WHITE, table, true, last_double_move, turn, true, true, PAWN) == 1) return 1;
        enPassant = false;
        return 0;
    }
    enPassant = false;
    return 1;
}
int Promote(int sor, int oszlop, int from_col, char piece_type, Piece table[HEIGHT][WIDTH], bool check, PieceColor turn, bool takes, int king[2], bool* king_inCheck){
    Piece p;
    p.color = turn;
    bool isCheck = false;
    int add = turn == WHITE ? 1 : -1;
    int from_row = sor + add;
    Piece temp_from = table[from_row][from_col];
    Piece temp_to = table[sor][oszlop];
    table[sor][oszlop] = table[from_row][from_col];
    table[from_row][from_col] = empty;
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

        isCheck = IsCheck('B',sor,oszlop,king,table);
    }
    else if(piece_type == 'N'){
        p.type = KNIGHT;
        isCheck = IsKnightCheck(table, turn == WHITE ? BLACK : WHITE, sor, oszlop);
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
        isCheck = IsCheck('R',sor,oszlop,king,table);
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
        isCheck = IsCheck('Q',sor,oszlop,king,table);
    }  
    else{
        printf("Rossz típust adtál meg!\n");
        return 1;
    }
    table[sor][oszlop] = temp_to;
    table[from_row][from_col] = temp_from;
    if(CheckPlace(table[from_row][from_col], from_row, from_col, table[from_row][from_col].color == WHITE ? 
        check_depth_black : check_depth_white, table, TRUE) == 1) return 1;
    if(CheckWhenPieceMoves(from_row, from_col, sor, oszlop, table, check, isCheck, turn, takes) == 1) return 1;
    RemovePieceFromDepth(from_row, from_col, turn == WHITE ? check_depth_black : check_depth_white, table);
    table[sor][oszlop] = p;
    table[from_row][from_col] = empty;
    if(CheckPlace(table[sor][oszlop], sor, oszlop, table[sor][oszlop].color == WHITE ? 
        check_depth_black : check_depth_white, table, FALSE) == 1) return 1;
    if(CheckWhenPieceMoves(sor, oszlop, -1, -1, table, check, isCheck, turn, takes) == 1) return 1;
    if(CheckPawnMoves(turn, turn == WHITE ? black_pawn_moves : white_pawn_moves, from_row, from_col, true, table) == 1) return 1;
    ChangingFromMovePosition(from_row, from_col);
    ChangingToMovePosition(sor, oszlop);
    if(ChangingKingInCheck(table, isCheck, check, king_inCheck, sor, oszlop, turn) == 1) return 1;
    return 0;
}
int KnightMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, 
    bool check, int sor, int oszlop, int length){
    if(IsMoveWrong(table, takes, sor, oszlop, turn, length) == 1) return 1;
    int lovak[8][2];
    int db = 0;
    int* p_db = &db;
    int indulas[2] = {sor+2, oszlop-1};
    for (int i = 1; i <= 8; ++i)
    {       
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
        if(IsCheckChanged(table,hova_sor,hova_oszlop,sor,oszlop,takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    bool isCheck = IsKnightCheck(table, turn == WHITE ? BLACK : WHITE, sor, oszlop);
    if(hova_sor == king[0] && hova_oszlop == king[1]){
        isCheck = true;
    }
    if(takes){
        RemovePieceFromDepth(sor, oszlop, turn == WHITE ? check_depth_white : check_depth_black, table);
    }
    if(ChangingTablePositions(table, hova_sor, hova_oszlop, sor, oszlop, check, isCheck, turn, takes, table[sor][oszlop].type == PAWN,nothing) == 1) return 1;
    if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
    return 0;
}
int BishopMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int bishops[PIECE_MAX_COUNT][2], 
    int* bishops_count, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, bool check, int sor, int oszlop, int length){
    if(IsMoveWrong(table, takes, sor, oszlop, turn, length) == 1) return 1;
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

    if(isPieceFoundCorrect(table,p_remain_db,remaining_bishops[0][0],remaining_bishops[0][1],sor,oszlop,takes,turn,
        "Hiba nincs futó ami odatud lépni","Több futó közül lehet választani!") == 1) return 1;
    int isCheck = IsCheck('B',sor,oszlop,king,table);
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
    if(ChangingTablePositions(table,remaining_bishops[0][0],remaining_bishops[0][1],sor,oszlop,check,isCheck,turn,takes,table[sor][oszlop].type == PAWN,nothing) == 1) return 1;
    if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
    return 0;
}
int RookMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int rooks[PIECE_MAX_COUNT][2], int* rooks_count, 
    Piece table[HEIGHT][WIDTH], PieceColor turn, bool* left_rook_moved, bool* right_rook_moved, bool takes, bool check, int sor, int oszlop, int length){
    if(IsMoveWrong(table, takes, sor, oszlop, turn, length) == 1) return 1;
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
    if(isPieceFoundCorrect(table,p_remain_db,remaining_rooks[0][0],remaining_rooks[0][1],sor,oszlop,takes,turn,
        "Hiba nincs bástya ami odatud lépni","Több bástya közül lehet választani!") == 1) return 1;
    bool isCheck = IsCheck('R',sor,oszlop,king,table);
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
    if(ChangingTablePositions(table,remaining_rooks[0][0],remaining_rooks[0][1],sor,oszlop,check,isCheck,turn,takes,table[sor][oszlop].type == PAWN,nothing) == 1) return 1;
    
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
    if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
    return 0;
}
int QueenMove(char lepes[MOVE_MAX_LENGTH], int king[2], bool* king_inCheck, int queens[PIECE_MAX_COUNT-1][2], 
    int* queens_count, Piece table[HEIGHT][WIDTH], PieceColor turn, bool takes, bool check, int sor, int oszlop, int length){
    if(IsMoveWrong(table, takes, sor, oszlop, turn, length) == 1) return 1;
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
    if(isPieceFoundCorrect(table,p_remain_db,remaining_queens[0][0],remaining_queens[0][1],sor,oszlop,takes,turn,
        "Hiba nincs királynő ami odatud lépni","Több királynő közül lehet választani!") == 1) return 1;

    bool isCheck = IsCheck('Q',sor,oszlop,king,table);
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
    if(ChangingTablePositions(table,remaining_queens[0][0],remaining_queens[0][1],sor,oszlop,check,isCheck,turn,takes,table[sor][oszlop].type == PAWN,nothing) == 1) return 1;
    if(ChangingKingInCheck(table,isCheck,check,king_inCheck,sor,oszlop,turn) == 1) return 1;
    return 0;
}
int KingMove(char lepes[MOVE_MAX_LENGTH], int king[2], Piece table[HEIGHT][WIDTH], PieceColor turn, bool* king_moved, 
    bool takes, bool check, PieceList check_depth[HEIGHT][WIDTH]){
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
    if(IsMoveWrong(table, takes, sor, oszlop, turn, length) == 1) return 1;
    if(abs(sor-king[0]) > 1 || abs(oszlop-king[1]) > 1){
        printf("Oda nem tud lépni a király!\n");
        return 1;
    }
    if(check_depth[sor][oszlop].size > 0){
        printf("Ott sakban lennél!\n");
        printf("\nHere?\n");
        return 1;
    }
    else if(KingCheck(sor,oszlop,takes)){
        printf("Ott sakban lennél!\n");
        printf("\nCheckingPiece: %d:%d\nPiece: %d:%d\nTakes: %s\n",checkingPiece.row,checkingPiece.column,sor,oszlop,takes ? "Yeah" : "Nay");
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
    if(ChangingTablePositions(table,king[0],king[1],sor,oszlop,check,false,turn,takes,table[sor][oszlop].type == PAWN,nothing) == 1) return 1;

    king[0] = sor;
    king[1] = oszlop;
    if(!(*king_moved)){
        *king_moved = true;
    }
    return 0;
}
int Castle(int king[2], int oppositeKing[2], Piece table[HEIGHT][WIDTH], PieceColor turn, bool* king_moved, bool short_castle,
    bool left_rook_moved, bool right_rook_moved, int rook_moved_sor, int rook_moved_oszlop, Piece empty, 
    int rooks[PIECE_MAX_COUNT][2], int rooks_count, bool check, PieceList depth[HEIGHT][WIDTH], bool takes, bool* king_inCheck){
    // A Sáncolás megoldására szól ez a függvény
    if(black_king_inCheck || white_king_inCheck){
        printf("Sakkban vagy, nem tudsz sáncolni!\n");
        return 1;
    }
    if(*king_moved){
        printf("Már nem tudsz sáncolni mert léptél a királlyal!\n");
        return 1;
    }
    //PieceColor color = short_castle ? WHITE : BLACK; // Melyik oldalt nézzük ahhozképest alakítjuk ki a többi részét
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
        if(depth[king[0]][i].size > 0 && (short_castle || (!short_castle && i > 1))){
            printf("Nem tudsz sáncolni sakkon keresztül!\n");
            return 1;
        }
    }
    //int adjust_by_color = turn == WHITE ? 1 : -1;
    //int adjust = short_castle ? 0:1;
    int add_to_king = short_castle ? 2 : -2;
    int add_to_rook = short_castle ? -2 : 3;
    int rook_column = rook_moved_oszlop+add_to_rook;
    Piece table_piece;
    table_piece.type = KING;
    table_piece.color = turn;
    ChangingFromMovePosition(king[0], king[1]);
    ChangingToMovePosition(king[0], king[1]+add_to_king);
    bool isCheckOppositeSide = IsCheck('R',rook_moved_sor,rook_column,oppositeKing,table);
    //bool isCheckOppositeSide = !isPieceBetween('R',1,rook_moved_sor,rook_column,oppositeKing[0],oppositeKing[1],table);
    if(IsCheckEqualsToMove(isCheckOppositeSide,check) == 1) return 1;
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
    table[rook_moved_sor][rook_column] = table[rook_moved_sor][rook_moved_oszlop];
    table[rook_moved_sor][rook_moved_oszlop] = empty;
    if(CheckWhenPieceMoves(king[0], king[1], king[0], king[1], table, false, false, turn, takes) == 1){
        return 1;
    }
    if(CheckWhenPieceMoves(rook_moved_sor, rook_moved_oszlop, rook_moved_sor, rook_moved_oszlop, table, false, false, turn, takes) == 1){
        return 1;
    }
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
    ChangingKingInCheck(table,isCheckOppositeSide,check,king_inCheck,rook_moved_sor,rook_column,turn);
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
void ChangingFromMovePosition(int row, int col){
    from_move.row = row;
    from_move.col = col;
}
void ChangingToMovePosition(int row, int col){
    to_move.row = row;
    to_move.col = col;
}
int IsMoveWrong(Piece table[HEIGHT][WIDTH], bool takes, int sor, int oszlop, PieceColor turn, int length){
    if(length > 5){
        printf("Nincs ilyen lépés!\n");
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
int isPieceFoundCorrect(Piece table[HEIGHT][WIDTH], int* p_remain_db, int from_row, int from_col, int row, int col, bool takes, 
    PieceColor turn, char* not_found, char* more_than_one_found){
     if(*p_remain_db == 0){
        printf("%s\n",not_found);
        return 1;
    }
    if(*p_remain_db > 1){
        printf("%s\n", more_than_one_found);
        return 1;
    }
    if(white_king_inCheck || black_king_inCheck){
        if(IsCheckChanged(table, from_row, from_col, row, col, takes,turn == WHITE ? white_king : black_king) == 1){
            return 1;
        }
    }
    return 0;
}