#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define AC_BLACK "\x1b[30m"
#define AC_RED "\x1b[31m"
#define AC_GREEN "\x1b[32m"
#define AC_YELLOW "\x1b[33m"
#define AC_BLUE "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN "\x1b[36m"
#define AC_WHITE "\x1b[37m"
#define AC_NORMAL "\x1b[m"

/*

TO-DO

fix order of board... it's upside down rn

upgrade CPU move function
    maybe place token around player
    scans for 2 or 3 in a row and blocks it

create function to check points/scan board
    add diagonal checks

    lol nocreate recursive function to compare group A to B. input int "in a row" score
        base case: 1 element, obviously belongs to both groups. A = B
        return score + 1 if equivalent and belongs to player
        return 0 if not equivalent
        return score - 1 if equivalent and belongs to computer

        if

recursive comparison function
int compare (int rows, int columns, int board[rows][columns], int score)



create function to run game
*/

typedef struct
{
    int row;
    int column;
} Grid;

//  general, useful functions
void clear_input();
void blank_space(int spaces);
int random_int(int min, int max);
void delay(int seconds);
void load(int seconds);
int sign(int input);

//  game-related functions
Grid user_setup();
void initialize_board(Grid size, int board[size.row][size.column]);
void print_board(Grid size, int board[size.row][size.column]);
int scan_board(Grid size, int board[size.row][size.column]);

int restart_game(int size);

//  gameplay functions
int get_user_input(int columns);
void make_user_move(Grid size, int board[size.row][size.column]);
void make_cpu_move(Grid size, int board[size.row][size.column]);

int main()
{
    const Grid MAX_GRID = {7, 10};

    srand(time(0));

    int board[MAX_GRID.row][MAX_GRID.column]; //    initializes a 2D array with 7 rows, and 10 columns -> 10x7
    Grid size;

    //  set zeroes for whole board
    initialize_board(MAX_GRID, board);

    size = user_setup();

    //for (int i = 0; i < 20; i++)
    while (1)
    {
        print_board(size, board);
        make_user_move(size, board);
        print_board(size, board);
        int result = scan_board(size, board);
        if (result == 1)
        {
            printf("You won!\n");
            initialize_board(MAX_GRID, board);
            clear_input();
            size = user_setup();
            continue;;
        }
        else if (result == 0)
        {
            printf("It was a draw\n");
            initialize_board(MAX_GRID, board);
            size = user_setup();
            continue;;
        }
        else if (result == -1)
        {
            printf("You lost :(\n");
            initialize_board(MAX_GRID, board);
            size = user_setup();
            continue;;
        }
        

        load(1);

        make_cpu_move(size, board);
        print_board(size, board);
        scan_board(size, board);

        // printf("%f", board[7][0]);

        // kms = scan_board(size, board, (Grid) {0, 0});

        /*
        print_board(size, board);
        load(1);
        make_cpu_move(size, board);
        */
    }

    /*
    print_board(size, board);
    make_user_move(size, board);
    print_board(size, board);
    load(1);
    make_cpu_move(size, board);
    print_board(size, board);

    make_user_move(size, board);
    print_board(size, board);
    make_cpu_move(size, board);
    print_board(size, board);
    */

    return 0;
}

void clear_input()
{
    while (getchar() != '\n')
        ;
}

void blank_space(int spaces)
{
    for (int i = 0; i < spaces; i++)
    {
        printf("\n");
    }
}

int random_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void delay(int seconds)
{
    time_t start_time, current_time;

    //  get the current time
    time(&start_time);

    //  calculate the target end time
    time_t end_time = start_time + seconds;

    //  loop until the current time reaches the end time
    do
    {
        time(&current_time);
    } while (current_time < end_time);
}

void load(int seconds)
{
    if (seconds <= 2)
    {
        printf("\n...");
        fflush(stdout); //  flush the output buffer
        delay(seconds);
        blank_space(1);
        return;
    }

    for (int i = 0; i < seconds; i++)
    {
        if (i > 0 && i % 3 == 0)
        {
            blank_space(1);
        }
        printf(".");
        fflush(stdout); //  flush the output buffer so that each "." is printed separately
        delay(1);
    }

    blank_space(1);
}

int sign(int input)
{
    if (input > 0)
    {
        return 1;
    }
    else if (input < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*
int user_setup()
{
    int start;
    int size;

    printf("\nWould you like to start a new game of Connect Four? (Y/N)\n");

    start = getchar();

    while (toupper(start) != 'Y' && toupper(start) != 'N')
    {
        //  process and clear line break
        clear_input();

        printf("\nInvalid response. Please enter either Y or N.\n");
        start = getchar();
    }

    if (toupper(start) == 'N')
    {
        printf("Goodbye!\n");
        exit(0);
    }

    //  process and clear line break
    clear_input();

    printf("\nPlease select a grid size:\n");
    printf("1: Small - 5x4\n");
    printf("2: Standard - 7x6\n");
    printf("3: Large - 10x7\n\n");
    size = getchar();

    //  while invalid input
    while (size < '1' || size > '3')
    {
        clear_input();

        printf("\nInvalid grid size. Please choose a size from 1 to 3.\n");
        size = getchar();
    }

    blank_space(1);

    switch (size)
    {
    case '1':
        printf("Size selected: Small - 5x4\n");
        break;

    case '2':
        printf("Size selected: Standard - 7x6\n");
        break;

    case '3':
        printf("Size selected: Large - 10x7\n");
        break;
    }

    blank_space(1);

    return size - '0';
}
*/

Grid user_setup()
{
    int start;
    int size;

    //clear_input();
    printf("\nWould you like to start a new game of Connect Four? (%sY%s/%sN%s)\n", AC_GREEN, AC_NORMAL, AC_RED, AC_NORMAL);
    start = getchar();

    while (toupper(start) != 'Y' && toupper(start) != 'N')
    {
        //  process and clear line break
        clear_input();

        printf("\nInvalid response. Please enter either Y or N.\n");
        start = getchar();
    }

    if (toupper(start) == 'N')
    {
        printf("Goodbye!\n");
        exit(0);
    }

    //  process and clear line break
    clear_input();

    printf("\nPlease select a grid size:\n");
    printf("1: Small - 5x4\n");
    printf("2: Standard - 7x6\n");
    printf("3: Large - 10x7\n\n");
    size = getchar();

    //  while invalid input
    while (size < '1' || size > '3')
    {
        clear_input();

        printf("\nInvalid grid size. Please choose a size from 1 to 3.\n");
        size = getchar();
    }

    blank_space(1);

    switch (size)
    {
    case '1':
        printf("Size selected: Small - 5x4\n\n");
        return (Grid){4, 5};
        break;

    case '2':
        printf("Size selected: Standard - 7x6\n\n");
        return (Grid){6, 7};
        break;

    case '3':
        printf("Size selected: Large - 10x7\n\n");
        return (Grid){7, 10};
        break;

    default:
        printf("\nERROR: INVALID BOARD SIZE\n\n");
        return (Grid){0, 0};
        break;
    }

    //  blank_space(1);
    /*
        switch (size)
        {
            case 1 + '0':
                return (struct Pos){4, 5};
                break;

            case 2 + '0':
                return (struct Pos){6, 7};
                break;

            case 3 + '0':
                return (struct Pos){7, 10};
                break;

            default:
                printf("\nERROR: INVALID BOARD SIZE\n");
                return (struct Pos){0, 0};
                break;
        }
    */
}

void initialize_board(Grid size, int board[size.row][size.column])
{
    for (int i = 0; i < size.row; i++)
    {
        for (int j = 0; j < size.column; j++)
        {
            board[i][j] = 0;
        }
    }
}

void print_board(Grid size, int board[size.row][size.column])
{
    blank_space(1);
    for (int i = size.row - 1; i >= 0; i--)
    {
        printf("|");

        for (int j = 0; j < size.column; j++)
        {
            switch (board[i][j])
            {
            case 0:
                printf("   ");
                break;

            case 1:
                printf(" %sx%s ", AC_BLUE, AC_NORMAL);
                break;

            case -1:
                printf(" %so%s ", AC_RED, AC_NORMAL);
                break;

            default:
                printf("ERROR: INVALID BOARD VALUE");
                exit(1);
                break;
            }
        }

        printf("|");

        //  new line for each row
        blank_space(1);
    }

    /*
    for (int i = 0; i < size.row; i++)
    {
        printf("|");

        for (int j = 0; j < size.column; j++)
        {
            switch (board[i][j])
            {
            case 0:
                printf("   ");
                break;

            case 1:
                printf(" %sx%s ", AC_BLUE, AC_NORMAL);
                break;

            case -1:
                printf(" %so%s ", AC_RED, AC_NORMAL);
                break;

            default:
                printf("ERROR: INVALID BOARD VALUE");
                exit(1);
                break;
            }
        }

        printf("|");

        //  new line for each row
        blank_space(1);
    }
    */

    //  print bottom border
    for (int j = 0; j < size.column; j++)
    {
        if (j == 0)
        {
            printf("+");
        }

        printf("---");

        if (j == size.column - 1)
        {
            printf("+\n");
        }
    }

    for (int j = 0; j < size.column; j++)
    {
        if (j == 0)
        {
            printf(" ");
        }

        printf(" %d ", j);

        if (j == size.column - 1)
        {
            printf(" \n");
        }
    }

    blank_space(1);
}

int get_user_input(int columns)
{
    int target = -1;
    int input;

    //  process and clear line break
    clear_input();

    printf("\n(You are %sx%s.)\nSelect a column from 0 to %d for your next move:\n", AC_BLUE, AC_NORMAL, columns - 1);
    input = getchar();

    //  verify input is in valid range
    while (input < '0' || input > columns - 1 + '0')
    {
        //  process and clear line break
        clear_input();

        printf("\nInvalid size entered. Please select a column from 0 to %d:\n", columns - 1);
        input = getchar();
    }

    target = input - '0';

    // printf("Position entered: %d\n", move);
    return target;
}

void make_user_move(Grid size, int board[size.row][size.column])
{
    int move = get_user_input(size.column);
    int spaces;

    do
    {
        spaces = 0;

        for (int i = 0; i < size.row; i++)
        {
            if (board[i][move] == 0)
            {
                spaces++;
            }
        }

        if (spaces <= 0)
        {
            printf("\nColumn full.\n");

            load(1);

            print_board(size, board);
            move = get_user_input(size.column);
        }
    } while (spaces <= 0);

    board[size.row - spaces][move] = 1;

    /*
    do
    {
        spaces = 0;

        for (int i = 0; i < size.row; i++)
        {
            if (board[i][move] == 0)
            {
                spaces++;
            }
        }

        if (spaces <= 0)
        {
            printf("Column full.\n");
            move = get_user_input(size.column);
        }
    } while (spaces <= 0);

    board[spaces - 1][move] = 1;
    */

    //  printf("Row: %d\nColumn: %d\n", spaces - 1, move);
}

void make_cpu_move(Grid size, int board[size.row][size.column])
{
    int move = random_int(0, size.column - 1);
    int spaces;

    do
    {
        spaces = 0;

        for (int i = 0; i < size.row; i++)
        {
            if (board[i][move] == 0)
            {
                spaces++;
            }
        }

        if (spaces <= 0)
        {
            move = random_int(0, size.column - 1);
        }
    } while (spaces <= 0);

    board[size.row - spaces][move] = -1;

    //  printf("Row: %d\nColumn: %d\n", spaces - 1, move);
}

int scan_board(Grid size, int board[size.row][size.column])
{
    /*
    --CONCEPT--

    check each row
    check each column

    check:
        if consecutive elements are equal
        if there are x in a row, record the "origin" (bottom and/or left)
        create a struct to record the data of each origin i.e. whose points, how many in a row, which direction
    */

    int count;
    int spaces = 0;

    for (int i = 0; i < size.row; i++)
    {
        for (int j = 0; j < size.column; j++)
        {
            if (board[i][j] == 0)
            {
                spaces++;
            }
        }
    }

    if (spaces == 0)
    {
        return 0;
        // printf("It's a draw.\n");
        // exit(0);
    }

    // scan rows

    for (int i = 0; i < size.row - 1; i++)
    {
        count = board[i][0];

        for (int j = 0; j < size.column - 1; j++)
        {
            if (board[i][j] == board[i][j + 1])
            {
                count += board[i][j + 1];
            }
            else
            {
                count = board[i][j + 1];
            }

            if (count >= 4)
            {
                return 1;
                /*
                printf("rYou won!\n");
                printf("i: %d, j: %d\n", i, j);
                exit(0);
                */
            }
            else if (count <= -4)
            {
                return -1;
                /*
                printf("rCPU won :(\n");
                printf("i: %d, j: %d\n", i, j);
                exit(0);
                */
            }
        }
    }


    //  scan columns

    for (int j = 0; j < size.column - 1; j++)
    {
        count = board[0][j];

        for (int i = 0; i < size.row - 1; i++)
        {
            if (board[i][j] == board[i + 1][j])
            {
                count += board[i + 1][j];
            }
            else
            {
                count = board[i + 1][j];
            }

            if (count >= 4)
            {
                return 1;
                /* 
                printf("cYou won!\n");
                printf("i: %d, j: %d\n", i, j);
                exit(0);
                 */
            }
            else if (count <= -4)
            {
                return -1;
                /* 
                printf("cCPU won :(\n");
                printf("i: %d, j: %d\n", i, j);
                exit(0);
                */
            }
        }
    }

    return 11;
}

int restart_game(int size)
{
}