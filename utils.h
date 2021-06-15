#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define NUM_PARTITION 9
#define NUM_SQUARE 3
#define DEBUG 0

#define NUM_VARIABLES 729


int number_of_steps;
int total_number_constraints;
/**
 * @brief The Cell struct: struct created to model a cell in z3
 */
struct Cell {
  int digit;
  int row;
  int col;
};

/**
 * @brief The LogicCell struct: structure create as passerelle from z3 to input file configuration
 */
struct LogicCell{
  int i;
  int j;
  int k;
  char var_name [5];
  int truth_value; //-1 if unknow, 1 if true, and 0 if false

};

/**
 * @brief variable_list: Logic representation of a sudoku with proposition logic
 */
struct LogicCell variable_list [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION];
struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION];


/**
 * @brief resetSudokuMatrix: puts zeros in every cell of the sudoku array
 * @param sudokuMatrix
 */
void resetSudokuMatrix(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]);

/**
 * @brief displaySudoku: Display the content of a given sudoku array
 * @param sudokuMatrix
 */
void displaySudoku(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]);

/**
 * @brief loadConfiguration: read the sudoku from a csv file and store it in an array
 * @param fp
 * @param filePath
 * @param sudokuMatrix
 */
void loadConfiguration(FILE *fp, char* filePath ,struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]);

/**
 * @brief checkInputConfiguration: verify if the input configuration of the input sudoku respect the rules
 * @param sudokuMatrix
 * @return
 */
int checkInputConfiguration(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]);

/**
 * @brief findEmptyCell: Locate a cell without a value
 * @param sudokuMatrix
 * @return
 */
struct Cell findEmptyCell(struct Cell sudokuMatrix [NUM_PARTITION][NUM_PARTITION]);

/**
 * @brief sudokuPropositionalLogicRepresentation: store the input soduku into a proosition logic structure
 * @return
 */
int sudokuPropositionalLogicRepresentation();

/**
 * @brief parseAndDisplaySolution: display the final configuration from the solver from the model
 * @param logic
 */
void parseAndDisplaySolution(char* logic);

#endif // BACKTRACKING_H_INCLUDED
