#include "game.h"
void Initialize(Piece table[HEIGHT][WIDTH])
{
    p_wbishopcount = &white_bishop_count;
    p_bbishopcount = &black_bishop_count;
    p_wrookcount = &white_rook_count;
    p_brookcount = &black_rook_count;
    p_wqueencount = &white_queen_count;
    p_bqueencount = &black_queen_count;
    nothing.piece = EMPTY;
    nothing.row = -1;
    nothing.column = -1;
    print_moves.length = 0;
    print_moves.curr_position = 0;
    from_move.row = -1;
    from_move.col = -1;
    to_move.row = -1;
    to_move.col = -1;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            Piece p;
            if(i == 0 || i== 1) p.color = BLACK;
            else if (i == HEIGHT-2 || i==HEIGHT-1) p.color = WHITE;
            else p.color = NOTHING;
            p.type = setType(y,i);
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
bool IsDepthNotPawnOrKing(PiecePlace* check_depth_row, int size, Piece table[HEIGHT][WIDTH], PieceColor king_color, bool is_pawn_depth, PieceList check_depth[HEIGHT][WIDTH]){
    if(size <= 0) return false;
    for (int i = 0; i < size; i++)
    {   
        printf("\nsize:%d\n",i);
        if((check_depth_row[i].piece != PAWN && check_depth_row[i].piece != KING) || is_pawn_depth){
            //printf("\nPiece: %s\n",getPiece(WHITE,check_depth[i].piece));
            int row = check_depth_row[i].row;
            int col = check_depth_row[i].column;
            bool canBlock = CanBlock(table,check_depth,row,col,king_color);
            if (canBlock) return canBlock;
        }
    }
    return false;
}
bool isMate(Piece table[HEIGHT][WIDTH], PieceList check_depth[HEIGHT][WIDTH], PieceList opposite_check_depth[HEIGHT][WIDTH], 
    int king[2], PieceList pawn_moves[HEIGHT][WIDTH], bool check, PieceColor opposite){
    if(opposite_check_depth[checkingPiece.row][checkingPiece.column].size > 0 && check_depth[checkingPiece.row][checkingPiece.column].size <= 0) return false;
    int curr_row = king[0]-1;
    int curr_column = king[1]-1;
    //printf("row: %d, col: %d\n",curr_row,curr_column);
    for (int i = 0; i < 3; i++)
    {   
        if (curr_row >= 0 && curr_row <= 7){
            for (int j = 0; j < 3; j++)
            {   
                if(curr_column <= 7 && curr_column >= 0 && !KingCheck(curr_row,curr_column,check) && (table[curr_row][curr_column].type == EMPTY && check_depth[curr_row][curr_column].size <= 0)){
                    //printf("\nCan move to:%d %d\n ",curr_row,curr_column);
                    return false;
                }   
                curr_column += 1;
            }
        }
        curr_column = king[1]-1;
        curr_row += 1;
    }
    if(double_check) return true;
    if(checkingPiece.piece == BISHOP || (checkingPiece.piece == QUEEN && abs(checkingPiece.row - king[0]) == abs(checkingPiece.column - king[1]))){
        int row_inc = king[0]-checkingPiece.row > 0 ? 1 : -1;
        int col_inc = king[1]-checkingPiece.column > 0 ? 1 : -1;
        int starting_row = checkingPiece.row;
        int starting_col = checkingPiece.column;
        //printf("Starting: %d:%d\n",starting_row,starting_col);
        starting_row += row_inc;
        starting_col += col_inc;
        while (table[starting_row][starting_col].type != KING){    
            bool canBlock = IsDepthNotPawnOrKing(opposite_check_depth[starting_row][starting_col].items, opposite_check_depth[starting_row][starting_col].size, table, opposite, false, check_depth);
            bool canBlockWithPawn = pawn_moves[starting_row][starting_col].size > 0 && IsDepthNotPawnOrKing(pawn_moves[starting_row][starting_col].items, pawn_moves[starting_row][starting_col].size, table, opposite, true, check_depth);
            if(canBlock || canBlockWithPawn){
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
            bool canBlock = IsDepthNotPawnOrKing(opposite_check_depth[starting_row][starting_col].items, opposite_check_depth[starting_row][starting_col].size, table, opposite, false, check_depth);
            bool canBlockWithPawn = pawn_moves[starting_row][starting_col].size > 0 && IsDepthNotPawnOrKing(pawn_moves[starting_row][starting_col].items, pawn_moves[starting_row][starting_col].size, table, opposite, true, check_depth);
            if(canBlock || canBlockWithPawn){
                return false;
            }
            starting_row += row_inc;
            starting_col += col_inc;
            if(row_or_col) starting_col += col_inc;
            else starting_row += row_inc;
        } 
    }
    return true;
}
void clearLastDoubleMove(PiecePlace* last_double_move){
    (*last_double_move).piece = EMPTY;
    (*last_double_move).row = -1;
    (*last_double_move).column = -1;
}

int game(Piece table[HEIGHT][WIDTH]){

    char(*moves)[MOVE_MAX_LENGTH] = malloc(sizeof(*moves) * 20);
    int moves_count = 0;

    bool black_king_moved = false;
    bool white_king_moved = false;
    bool left_black_rook_moved = false;
    bool right_black_rook_moved = false;

    bool left_white_rook_moved = false;
    bool right_white_rook_moved = false;
    PiecePlace last_double_move;
    last_double_move.piece = EMPTY;
    

    int vege = 0;
    int lepesek_szama = 1;
    int* p_lepesek = &lepesek_szama;
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
        Location temp_from_move = from_move;
        Location temp_to_move = to_move;
        bool white_king_inCheck_temp = white_king_inCheck;
        bool black_king_inCheck_temp = black_king_inCheck;
        piece_list_copy(temp_white, check_depth_white);
        piece_list_copy(temp_black, check_depth_black);
        
        bool isMoveDouble = false;
        bool isCorrect = false;
        bool wasMoveDouble = false;
        bool temp_double_check = double_check;
        if(double_check && lepes[0] == 'K') double_check = false;
        if(double_check && lepes[0] != 'K'){
            printf("Ez nem állítja meg a sakkot! (Kettős sakk)");
        } 
        else if(lepes[0] >= 'a' && lepes[0] <= 'h'){
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
        else{
            int length = strlen(lepes);
            int sor = 8 - (length == 3 ? (lepes[2]-'0') : length == 4 ? (lepes[3] - '0') : (lepes[4] - '0'));
            printf("\nsor:%d\n",sor);
            int oszlop = length == 3 ? lepes[1] - 'a' : length == 4 ? lepes[2] - 'a' : lepes[3] - 'a';
            if(lepes[0] == 'N'){
                if(KnightMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck, table, turn, takes, check, sor, oszlop, length) == 0){
                    isCorrect = true;
                }
            }
            else if(lepes[0] == 'B'){
                if(BishopMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck, turn == WHITE? white_bishops : black_bishops, turn == WHITE? p_wbishopcount : p_bbishopcount,table,turn, takes, check, sor, oszlop, length) == 0){
                    isCorrect = true;
                }
            }
            else if(lepes[0] == 'R'){
                if(RookMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? &black_king_inCheck : &white_king_inCheck,
                    turn == WHITE? white_rooks : black_rooks, turn == WHITE? p_wrookcount : p_brookcount, table, turn, 
                    turn == WHITE ? &left_white_rook_moved : &left_black_rook_moved, turn == WHITE ? &right_white_rook_moved : &right_black_rook_moved, takes, check, sor, oszlop, length) == 0){
                    isCorrect = true;
                }
            }
            else if(lepes[0] == 'Q'){
                if(QueenMove(lepes, turn == WHITE ? black_king : white_king, turn == WHITE ? 
                    &black_king_inCheck : &white_king_inCheck, turn == WHITE? white_queens : black_queens,
                    turn == WHITE? p_wqueencount : p_bqueencount, table, turn, takes, check, sor, oszlop, length) == 0){
                    isCorrect = true;
                }
            }
            else if(lepes[0] == 'K'){
                if(KingMove(lepes, turn == WHITE? white_king : black_king, table, turn, turn == WHITE ? 
                    &white_king_moved : &black_king_moved, takes, check, turn == WHITE ? check_depth_white : check_depth_black) == 0){
                    isCorrect = true;
                }
            }
            else if(strcmp(lepes,"O-O") == 0){
                if(Castle(turn == WHITE ? white_king : black_king, turn == WHITE ? black_king : white_king, table, turn, turn == WHITE ? &white_king_moved : &black_king_moved, true, turn == WHITE?
                left_white_rook_moved : left_black_rook_moved, turn == WHITE ? right_black_rook_moved : right_white_rook_moved, turn == WHITE ? 7 : 0, 7, empty, 
                turn == WHITE ? white_rooks : black_rooks, turn == WHITE ? white_rook_count : black_rook_count, 
                check, turn == WHITE ? check_depth_white : check_depth_black, takes, turn == WHITE ? &black_king_inCheck : &white_king_inCheck) == 0){
                    isCorrect = true;
                }
            }   
            else if (strcmp(lepes,"O-O-O") == 0){
                if(Castle(turn == WHITE ? white_king : black_king, turn == WHITE ? black_king : white_king, table, turn, turn == WHITE ? &white_king_moved : &black_king_moved, false, turn == WHITE?
                left_white_rook_moved : left_black_rook_moved, turn == WHITE ? right_black_rook_moved : right_white_rook_moved, turn == WHITE ? 7 : 0, 0, 
                empty, turn == WHITE ? white_rooks : black_rooks, turn == WHITE ? white_rook_count : black_rook_count, 
                check, turn == WHITE ? check_depth_white : check_depth_black, takes, turn == WHITE ? &black_king_inCheck : &white_king_inCheck) == 0){
                    isCorrect = true;
                }
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
            clearLastDoubleMove(&last_double_move);

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
                turn == WHITE ? black_king : white_king, turn == WHITE ? white_pawn_moves : black_pawn_moves, check, turn == WHITE ? BLACK : WHITE
            )){
                PrintTable(table,print_moves);
                printf("%sSakk Matt\n%sA %s nyert! gg\n",SPACE,SPACE,turn == WHITE ? "Fehér" : "Fekete");
                break;
            }
            if(print_moves.curr_position == 16){
                print_moves.curr_position = 0;
            } 
            strcpy(print_moves.moves[print_moves.curr_position].move,actual_lepes);
            print_moves.moves[print_moves.curr_position++].current_turn = *p_lepesek;
            if(print_moves.length != 16){
                print_moves.length++;
            } else {
                print_moves.length = 1;
            }
            turn = turn == WHITE ? BLACK : WHITE;
            if(turn == WHITE) *p_lepesek = *p_lepesek + 1; 
            strcpy(moves[moves_count++], actual_lepes);
            if(moves_count%20 == 0){
                moves = realloc(moves, sizeof(*moves) * (moves_count+20));
            }
        }
        else{
            CleanDepthList(temp_white, temp_black, white_king_inCheck_temp, black_king_inCheck_temp, &last_double_move, temp_last_double_move,temp_from_move,temp_to_move, temp_double_check);
        }
        freeAllPieceList(temp_white);
        freeAllPieceList(temp_black);
        PrintTable(table,print_moves);
    }
}