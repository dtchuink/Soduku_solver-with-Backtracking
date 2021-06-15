#include <stdio.h>
/**
 * @brief row_configuration
 * @param sudoku
 * @param row
 * @param current_number
 * @return true if the row configuration is good and false otherwise
 */
int row_configuration(int sudoku[9][9], int row, int current_number){
    for (int var = 0; var < 9; ++var) {
        if(sudoku[row][var]==current_number)
            return 0;
    }
    return 1;
}

/**
 * @brief column_configuration
 * @param sudoku
 * @param column
 * @param current_number
 * @return  true if the column configuration is good and false otherwise
 */
int column_configuration(int sudoku[9][9], int column, int current_number){
    for (int var = 0; var < 9; ++var) {
        if(sudoku[var][column]==current_number)
            return 0;
    }
    return 1;
}

/**
 * @brief square_configuration
 * @param sudoku
 * @param row
 * @param column
 * @param current_number
 * @return return true if the local square configuration is good and false otherwise
 */
int square_configuration(int sudoku[9][9], int start_row, int start_column, int current_number){
    for (int i = start_row; i<start_row+3; ++i) {
        for (int j=start_column; j<start_column+3; ++j) {
            if(sudoku[start_row][start_column]==current_number)
                return 0;
        }
    }
    return 1;
}

int check_sudoku_input_configuration(int sudoku[9][9], int *row, int *column){

    int square_row = *row/3;
    int square_column = *column/3;
    int number = sudoku[*row][*column];

    if(number !=0){
        if(row_configuration(sudoku, *row, number)==1 & column_configuration(sudoku, *column, number)==1
                && square_configuration(sudoku, square_row, square_column, number)){
                if(*column<8)
                    check_sudoku_input_configuration(sudoku, row, column+1);
                else if(*row<8)
                    check_sudoku_input_configuration(sudoku, row+1, 0);
                else
                    return 1;

        }
        else {
            printf("!!!!!! WRONG Input Configuration. Please check element at line: %d and column %d", *row, *column);
            return 0;
        }
    }
    else{
        if (*column<8){
            check_sudoku_input_configuration(sudoku, row, column+1);
        }
        else if (*row<8){
           check_sudoku_input_configuration(sudoku, row+1, 0);
        } else {
            return 1;
        }
    }

}

void solve(int sudoku[9][9]){

    if(check_sudoku_input_configuration(sudoku, 0, 0)==1){

        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                if(sudoku[i][j]==0){

                }

            }

        }
    }

}



void display(int sudoku[9][9]) {//The function to display the solved Sudoku
    int row,column;
    printf("################# Sudoku Configuration ################\n");
    for(row=0;row<9;row++) {
        for(column=0;column<9;column++){
        printf("%d ",sudoku[row][column]);
        printf("\n");
        }
    }
}

int main()
{
    printf("Hello World!\n");
    return 0;
}
