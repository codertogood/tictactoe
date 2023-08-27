#include <stdio.h>
#include <unistd.h>

int decode_move(char *buf, int *move_to);
int display(int played, int type);
int move(int board, int *type);
int has_won(int board, int *type);

int win_states[8] = {
    0b111000000,
    0b000111000,
    0b000000111,
    0b100100100,
    0b010010010,
    0b001001001,
    0b100010001,
    0b001010100
};
int main(){
    int board = 0;
    int type = 0;
    for(int i = 0; i < 9; i += 1){
        display(board, type);
        board = move( board, &type); 
        int won = has_won(board, &type);
        if(won == 1){
            display(board, type);
            printf(" X is the winner!\n");
            return 0;
        }else if(won == 0){
            display(board, type);
            printf(" O is the winner!\n");
            return 0;
        }
        board = board ^ (1 << 31);
    }
    display(board, type);
}
int decode_move(char *buf, int *move_to){
    switch (buf[0]){
        case 'a':
            move_to[0] = 0;
            break;
        case 'b':
            move_to[0] = 1;
            break;
        case 'c':
            move_to[0] = 2;
            break;
        default:
            return -1;

    }
    switch (buf[1]){
        case '1':
            move_to[1] = 0;
            break;
        case '2':
            move_to[1] = 1;
            break;
        case '3':
            move_to[1] = 2;
            break;
        default:
            return -1;
    }
}
int display(int played, int type){
    printf("   1   2   3");
    for (int i = 0; i < 9; i += 1){
        if (i == 0){
            printf("\n a ");
        }
        if (i % 3 == 0 && i > 0){
            printf("\n  ---+---+---\n %c ", (i/3) + 97 );
        }
        if(played & (1 << i)){
            if(type & (1 << i)){
                printf("x");
            }else {
                printf("o");
            }
        }else{
            printf(" ");
        }
        if(i % 3 != 2){
            printf(" | ");
        }
    }
    printf("\n\n");
    // printf("  %i | %i | %i \n", board & 1, (board >> 1) & 1, (board >> 2) & 1);
    // printf("----+---+----\n");
    // printf("  %i | %i | %i \n", (board >> 3)& 1, (board >> 4) & 1, (board >> 5) & 1);   
    // printf("----+---+----\n");
    // printf("  %i | %i | %i \n\n", (board >> 6) & 1, (board >> 7) & 1, (board >> 8) & 1);
}
int move( int board, int *type){
    char buf[64];
    int move_to[2];
    if(board & (1 << 31)){
            printf(" Its X's tern: ");
        }else{
            printf(" Its O's tern: ");
        }
    fflush(stdout);
    read(1, buf, 64);
    if(decode_move(buf, move_to)== -1){
        return move(board, type); 
    };
    int offset = move_to[0]*3 + move_to[1];
    if(board & (1 << offset)){
        return move(board, type);
    }
    // 0b000000010
    // 0b010000000
    // 0b101010101
    // 0b111010101
    if(board & (1 << 31)){
        *type = *type | (1 << offset);    
    }
    int new_board = board | (1 << offset);
    return new_board;
}
int has_won(int board, int *type){
    int O_won = *type ^ board;
    for (int i = 0; i < 8; i++){
        if ((O_won & win_states[i]) == win_states[i]){
            return 0;
        }
    }
    for (int i = 0; i < 8; i++){
        if ((*type & win_states[i]) == win_states[i]){
            return 1;
        }
    }
    return -1;
}