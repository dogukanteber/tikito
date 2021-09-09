#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// COLOR CODES FOR COLORFUL TERMINAL OUTPUT
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define BOARD_SIZE 9

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

static const char HUMAN = 'X';
static const char COMPUTER = 'O';
static const char BLANK = ' ';

void init_board(char* board) {
    for ( size_t i=0; i<BOARD_SIZE; ++i ) {
        board[i] = BLANK;
    }
}

void welcome_message(void) {
    printf(GRN "\n\n%s\n\n\n" RESET, "Welcome to tikito, a Tic-Tac-Toe game against AI.");
    printf(GRN "%s\n" RESET, "=> The same rules in normal Tic-Tac-Toe game applies.");
    printf(GRN "%s\n" RESET, "=> The board will print after each move whether your move or AI's move.");
    printf(GRN "%s\n\n" RESET, "=> Try to beat him and most importantly have fun! Let's begin...");
}

void players_info(void) {
    printf(RED "You are %c%s" RESET, HUMAN, "\n");
    printf(MAG "AI is %c%s" RESET, COMPUTER, "\n");
}

void print_colored_pipe(void) {
    printf(CYN "|" RESET);
}

void print_colored_char(char c) {
    if ( c == COMPUTER )
        printf(MAG " %c " RESET, c);
    else 
        printf(RED " %c " RESET, c);
}

void print_board(const char* board) {
    printf("\n");

    for ( size_t i=0; i<BOARD_SIZE; ++i ) {
        if ( board[i] == BLANK ) {
            print_colored_pipe();
            printf(YEL" %ld " RESET, i+1);
        }
        else {
            print_colored_pipe();
            char c = board[i];
            print_colored_char(c);
        }
        if ( i % 3 == 2 ) {
            print_colored_pipe();
            printf(" \n");
        }
    }
    printf("\n");
}

bool is_board_filled(const char* board) {
    for ( size_t i=0; i<BOARD_SIZE; ++i ) {
        if ( board[i] == BLANK )
            return false;
    }
    return true;
}

bool is_row_win(const char* board, char player) {
    for ( size_t i=0; i<BOARD_SIZE; i+= 3 ) {
        if ( board[i] == board[i+1] && board[i+1] == board[i+2] ) {
            if ( board[i] == player ) {
                return true;
            }
        }
    }
    return false;
}

bool is_col_win(const char* board, char player) {
    for ( size_t i=0; i<BOARD_SIZE / 3; ++i ) {
        if ( board[i] == board[i+3] && board[i+3] == board[i+6] ) {
            if ( board[i] == player ) {
                return true;
            }
        }
    }
    return false;
}

bool is_left_diag_win(const char* board, char player) {
    if ( board[2] == board[4] && board[4] == board[6] ) {
        if ( board[2] == player ) {
            return true;
        }
    }
    return false;
}

bool is_right_diag_win(const char* board, char player) {
    if ( board[0] == board[4] && board[4] == board[8] ) {
        if ( board[0] == player ) {
            return true;
        }
    }
    return false;
}

bool is_win(const char* board, char player) {
    if ( is_row_win(board, player) || is_col_win(board, player) ||
        is_right_diag_win(board, player) || is_left_diag_win(board, player) ) {
            return true;
        }
    else
        return false;
}

// Checks whether the game is ended or not
bool is_terminal_state(const char* board) {
    if ( is_win(board, COMPUTER) || is_win(board, HUMAN) || is_board_filled(board) )
        return true;
    else
        return false;
}

bool is_empty(const char* board, int i) {
    return board[i] == BLANK;
}

void who_won(const char* board) {
    printf("%s", "Game is over. ");

    if ( is_win(board, HUMAN) )
        printf("%s","You won!\n");
    else if ( is_win(board, COMPUTER) )
        printf("%s", "AI won!\n");
    else
        printf("%s", "Draw!\n");
}

/*
    Minimax evaluation function, also known as heuristic function.
    if AI wins, returns +10 as score;
    if the player wins, returns -10 as score
    if no one wins, it returns 0.
*/
int evaluate(const char* board) {
    if ( is_win(board, HUMAN) ) {
            return -10;
        }
    else if ( is_win(board, COMPUTER) ) {
            return 10;
        }
    else {
        return 0;
    }
}

int minimax(char* board, int depth, bool is_max) {
    // get the score of the board
    int score = evaluate(board);

    // if AI wins
    if ( score == 10 )
        // subtract depth to choose the fastest win
        return score - depth;

    // if the player wins
    if ( score == -10 )
        // add depth to postpone the end state of the game
        return score + depth;

    if ( is_board_filled(board) )
        return 0;
    
    // if the current player is AI
    if ( is_max ) {
        int best_score = INT_MIN;
        // visit every possible spot
        for ( int i=0; i<BOARD_SIZE; ++i ) {
            if ( is_empty(board, i) ) {
                board[i] = COMPUTER;
                // call minimax function as minimizer
                int score = minimax(board, depth+1, !is_max);
                board[i] = BLANK;
                best_score = max(score, best_score);
            }
        }
        return best_score;
    }
    // if the current player is human
    else {
        int best_score = INT_MAX;
        // visit every possible spot
        for ( int i=0; i<BOARD_SIZE; ++i ) {
            if ( is_empty(board, i) ) {
                board[i] = HUMAN;
                // call minimax function as maximizer
                int score = minimax(board, depth+1, !is_max);
                board[i] = BLANK;
                best_score = min(score, best_score);
            }
        }
        return best_score;
    }
}

int find_optimal_move(char* board) {
    int best_val = INT_MIN;
    int move;

    for ( size_t i=0; i<BOARD_SIZE; ++i ) {
        if ( is_empty(board, i) ) {
            board[i] = COMPUTER;
            // triggers minimax algorithm for every possible spot
            int move_value = minimax(board, 0, false);
            board[i] = BLANK;
            if ( move_value > best_val ) {
                best_val = move_value;
                move = i;
            }
        }
    }
    return move;
}

void computer_move(char* board) {
    int move = find_optimal_move(board);

    board[move] = COMPUTER;
}

bool computer_plays(char* board) {
    printf("%s", "--> AI's turn\n");
    computer_move(board);

    printf("%s", "Board after AI's turn \n");
    print_board(board);

    if ( is_board_filled(board) )
        return true;
    else if ( is_win(board, COMPUTER) )
        return true;
    return false;
}

void human_move(char* board) {
    int move;
    for ( ;; ) {
        printf("%s", "--> Your turn, make your move: ");
        scanf("%d", &move);

        if ( move >= 1 && move <= 9 )
            break;

        printf("%s", "You cannot make that move. Try again.\n");
    }

    // we input the moves starting from 1, therefore we subtract one
    // when accessing the board
    board[move-1] = HUMAN;
}

bool human_plays(char* board) {
    human_move(board);

    printf("%s", "Board after your turn:\n");
    print_board(board);

    if ( is_board_filled(board) )
        return true;
    else if ( is_win(board, HUMAN) )
        return true;
    
    return false;
}

void game(char* board) {
    init_board(board);
    welcome_message();
    players_info();
    print_board(board);

    // game loop
    // run until someone wins or the board is full
    while ( !is_terminal_state(board) ) {
        // human_plays returns a boolean value to determine
        // whether the game is ended or not
        // if the game is ended, get out of while loop
        if ( human_plays(board) )
            break;
        // same as human_plays function
        // above explanation applies here too
        if ( computer_plays(board) )
            break;
    }
    who_won(board);
    print_board(board);
}

int main(int argc, char** argv) {
    char board[BOARD_SIZE];

    game(board);

    return EXIT_SUCCESS;
}
