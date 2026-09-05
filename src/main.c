#include "main.h"
void SwitchSkins(int num){
    switch (num)
    {   
        case 1:
            black_tile = BLACK_TILE_CLASSIC;
            white_tile = WHITE_TILE_CLASSIC;
            break;     
        case 2:
            black_tile = BLACK_TILE_BLUE;
            white_tile = WHITE_TILE_BLUE;
            break;          
        default:
            black_tile = BLACK_TILE_BASE;
            white_tile = WHITE_TILE_BASE;
            break;
    }
}
int main(){
    //Terminálba: chcp 65001
    printf("\033[8;30;120t");
    int curr_row = 0;
    bool menu_end = false;
    printf("\033[2J");
    printf("\033[?25l");
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
    Initialize(table);
    black_tile = BLACK_TILE_BASE;
    white_tile = WHITE_TILE_BASE;
    int table_skins = 0;
    while(!menu_end){
        printf("\033[9;1H");
        printf(
            "%s████████╗██╗  ██╗███████╗    ██████╗██╗  ██╗███████╗███████╗███████╗\n"
            "%s╚══██╔══╝██║  ██║██╔════╝   ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝\n"
            "%s   ██║   ███████║█████╗     ██║     ███████║█████╗  ███████╗███████╗\n"
            "%s   ██║   ██╔══██║██╔══╝     ██║     ██╔══██║██╔══╝  ╚════██║╚════██║\n"
            "%s   ██║   ██║  ██║███████╗   ╚██████╗██║  ██║███████╗███████║███████║\n"
            "%s   ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n",
            
            THE_CHESS_SPACE,
            THE_CHESS_SPACE,
            THE_CHESS_SPACE,
            THE_CHESS_SPACE,
            THE_CHESS_SPACE,
            THE_CHESS_SPACE
        );
        printf("\033[2K");
        if(curr_row == 0) printf("\033[100m\t>> Start game\033[0m\n");
        else printf("\t> Start game\n");
        printf("\033[2K");
        if(curr_row == 1) printf("\033[100m\t>> Switch Board Color \033[0m\n");
        else printf("\t> Switch Board Color\n");
        printf("\033[2K");
        if(curr_row == 2) printf("\033[100m\t>> Language \033[0m\n");
        else printf("\t> Language\n");
        char menu = getch_new();
        switch (menu)
        {
        case 's':
            curr_row = curr_row+1 <= MENU_MAX_LENGTH ? ++curr_row : curr_row;
            break;
        
        case 'w':
            curr_row = curr_row-1 >= MENU_MIN_LENGTH ? --curr_row : curr_row;
            break;
        case CTRL_C:
            menu_end = true;
            break;
        case ENTER:
            if(curr_row == 1){
                bool next_menu = false;
                
                while(!next_menu){
                    printf("\033[7;1H");
                    printf("\033[0J");
                    switch(table_skins){
                        case 1: 
                            printf("\t\t\t\t\t << Blue Themed >>\n");
                            break;
                        case 2:
                            printf("\t\t\t\t\t   << Classic >>\n");
                            break;
                        default:
                            printf("\t\t\t\t\t     << Base >>\n");
                            break;
                    }
                    PrintTable(table,print_moves);
                    char menu2 = getch_new();
                    switch (menu2)
                    {
                        case CTRL_C:
                            next_menu = true;
                            break;
                        case ENTER:
                            next_menu = true;
                            break;
                        case 'd':
                            table_skins = table_skins == 2 ? 0 : ++table_skins; 
                            SwitchSkins(table_skins);
                            break;
                        case 'a':
                            table_skins = table_skins == 0 ? 2 : --table_skins; 
                            SwitchSkins(table_skins);
                            break;
                        default:
                            break;
                    }
                }
                printf("\033[7;1H");
                printf("\033[0J");
                break;
            }
            menu_end = true;
            break;
        default:
            break;
        }
    }
    printf("\033[?25h");
    game(table);
    return 0;
}