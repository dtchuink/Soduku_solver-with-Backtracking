#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"



void resetSudokuMatrix(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]){
    int i, j;

    number_of_steps = 0;
    for (i = 0; i < NUM_PARTITION; i++)
      for (j = 0; j < NUM_PARTITION; j++){
           sudokuMatrix[i][j].digit = 0;
           sudokuMatrix[i][j].row = i;
           sudokuMatrix[i][j].col = j;
      }
}

void displaySudoku(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]){
    int i, j;

    for (i = 0; i < NUM_PARTITION; i++){
        if((i%3==0)&&(i!=0))
           printf(" - - - - - - - - - - -\n");
      for (j = 0; j < NUM_PARTITION; j++){
           if((j%3==0)&&(j!=0))
              printf(" |");
         printf(" %d", sudokuMatrix[i][j].digit);
      }
      printf("\n");
    }
}

void printGrid(int sudoku[NUM_PARTITION][NUM_PARTITION]){
    for (int row = 0; row < NUM_PARTITION; row++) {
       for (int col = 0; col <NUM_PARTITION; col++){
             printf("%2d", sudoku[row][col]);
             if (col%3 == 2) printf("|");
       }
        if (row%3 == 2)printf("\n+-----+------+------+");
        printf("\n");
    }
}

void loadConfiguration(FILE *fp, char* filePath,struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]){
    int i, j, convertedValue, counter = 0;
    char readValue;

    //fp = fopen("sudoku1.txt", "r");
    fp = fopen(filePath, "r");

    for (i = 1; i <= NUM_PARTITION; i++){
        for (j = 1; j <= (NUM_PARTITION + NUM_PARTITION ); j++){
           readValue = fgetc(fp);
           if((j % 2) != 0){//we collect the values from the text file
             convertedValue = atoi(&readValue);
             sudokuMatrix[i-1][counter].digit = convertedValue;
             counter++;
           }
        }
       counter = 0;
    }
    fclose(fp);
}

int checkInputConfiguration(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]){
   int violation = 0,i,j,k,q;
   int begin_i, end_i, begin_j, end_j, counter_1 = 1;

   //We first check is a digit appears more than once on a column and row
   for (i = 0; i < NUM_PARTITION; i++){
    for (j = 0; j < NUM_PARTITION; j++){ //for the cell [i,j]
      if(sudokuMatrix[i][j].digit!=0){//we don't consider cells containing 0
        //we first look into its line (line i) to see if another cell holds the same value
        for(k=0;k<NUM_PARTITION;k++)
            if(k!=j)
                if(sudokuMatrix[i][k].digit == sudokuMatrix[i][j].digit){
                   printf("\nERROR: Conflict between cells - sudoku[%d][%d]=%d and sudoku[%d][%d]=%d\n",i+1,j+1, sudokuMatrix[i][j].digit, i+1,k+1, sudokuMatrix[i][k].digit);
                   violation = 1;
                   return  violation;
            }

        //we then look into its column (column j) to see if another cell holds the same value
        for(k=0;k<NUM_PARTITION;k++)
             if(k!=i)
                if(sudokuMatrix[k][j].digit == sudokuMatrix[i][j].digit){
                   printf("\nERROR: Conflict between cells - sudoku[%d][%d]=%d and sudoku[%d][%d]=%d\n",i+1,j+1, sudokuMatrix[i][j].digit, k+1,j+1, sudokuMatrix[k][j].digit);
                   violation = 1;
                   return  violation;
            }
      }
    }
   }

   begin_i = 0;
   end_i =2;
   begin_j = 0;
   end_j = 2;
   while(end_i<=8){
       if(counter_1==3){
          begin_i+=3;
          end_i+=3;
          begin_j=0;
          end_j=2;
          counter_1 = 1;
       }

       //Now we check each square of 9 digits to see if a value appears more than once
       for(i=begin_i;i<=end_i; i++){
        for(j=begin_j;j<=end_j; j++){
           if(sudokuMatrix[i][j].digit!=0){//we don't consider cells containing 0
              for(k=begin_i;k<=end_i; k++){
                for(q=begin_j;q<=end_j; q++){
                   if(k!=i && q!=j)
                      if(sudokuMatrix[k][q].digit == sudokuMatrix[i][j].digit){
                       printf("\nERROR: Conflict between cells - sudoku[%d][%d]=%d and sudoku[%d][%d]=%d\n",i+1,j+1, sudokuMatrix[i][j].digit, k+1,q+1, sudokuMatrix[k][q].digit);
                       violation = 1;
                       return  violation;
                      }
                }
              }
           }
        }
       }

       counter_1 ++;
       begin_j+=3;
       end_j+=3;
   }
    return violation;
}

struct Cell findEmptyCell(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]){
  int i, j;
  struct Cell cell;

  cell.col = -1;
  cell.digit = -1;
  cell.row = -1;

  for(i=0; i< NUM_PARTITION;i++)
    for(j=0;j<NUM_PARTITION;j++)
       if(sudokuMatrix[i][j].digit==0)
            return sudokuMatrix[i][j];//just to get the i,j coordinate of a cell that is empty

   return cell;
}


int isPlacementValid(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION], int digit, int row, int col){

  int i,j;
  int square_i,square_j;

  //we first check the whole row
  for(j=0;j<NUM_PARTITION;j++)
    if(sudokuMatrix[row][j].digit == digit)
           return 0;

  //Then we check the whole column
  for(i=0;i<NUM_PARTITION;i++)
    if(sudokuMatrix[i][col].digit == digit)
           return 0;

  //Finaly, we check the square box when we want to insert
  square_i = row/3;
  square_j = col/3;

  for(i=0;i<NUM_SQUARE;i++)
    for(j=0;j<NUM_SQUARE;j++)
      if(sudokuMatrix[(square_i*3)+i][(square_j*3)+j].digit == digit)
           return 0;
   return 1;
}


/** FUNCTIONS USED IN THE SUDOKU SOLVING WITH Z3**/


int sudokuPropositionalLogicRepresentation(){
  
  int i, j, k, counter=0;
 
  printf("\n");


  for (i = 0; i < NUM_PARTITION; i++)
      for (j = 0; j < NUM_PARTITION; j++)
          for (k = 0; k < NUM_PARTITION; k++){
              variable_list[i][j][k].i = i;
              variable_list[i][j][k].j = j;
              variable_list[i][j][k].k = k;
              
              variable_list[i][j][k].var_name[0] = 'x';
              variable_list[i][j][k].var_name[1] = i+'0';
              variable_list[i][j][k].var_name[2] = j+'0';
              variable_list[i][j][k].var_name[3] = (k+1)+'0';
              variable_list[i][j][k].var_name[4] = '\0';
              
              if(sudokuMatrix[i][j].digit==k+1)
                 variable_list[i][j][k].truth_value = 1;
              else 
                 variable_list[i][j][k].truth_value = -1; 

              //printf(" >>Var %d: %s",counter, variable_list[i][j][k].var_name);
              //printf("  -->Truth Value: %d\n", variable_list[i][j][k].truth_value);
              counter++;
          }

}

void parseAndDisplaySolution(char *logic){
    int row, column, digit, j=0;
    char *p = strtok (logic, "\n");
    char *array [NUM_VARIABLES];
    int sudoku_result [NUM_PARTITION][NUM_PARTITION];

    while (p != NULL) {
            array[j++] = p;
            p = strtok (NULL, "\n");
        }
    printf(" \n Result Generated by Z3 \n");
    printf(" ---------------------- \n");

    for (int k = 0; k < NUM_VARIABLES; ++k){
        char * found = strstr( array[k], "true" );

        if(found!=NULL){
            row = atoi(&array[k][1])/100;
            column = atoi(&array[k][2])/10;
            digit = atoi(&array[k][3]);
            sudoku_result[row][column] = digit;

        }
    }

    printGrid(sudoku_result);


}



