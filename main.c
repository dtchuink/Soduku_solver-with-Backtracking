#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<memory.h>
#include<setjmp.h>
#include <time.h>
#include<z3.h>
#include "utils.h"
#include "z3_solver.h"


#define LOG_Z3_CALLS

#ifdef LOG_Z3_CALLS
#define LOG_MSG(msg) Z3_append_log(msg)
#else
#define LOG_MSG(msg) ((void)0)
#endif


/**
  Main Function
*/
int main(int argc, char **argv) {
#ifdef LOG_Z3_CALLS
    Z3_open_log("z3.log");
#endif

    FILE *fp=NULL; //file pointer
    char* filePath = argv[1];
    int i,j,k, constraint_counter = 0;
    Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION], true_node;
    Z3_ast constraints[10000];
    clock_t start;
    double cpu_time_used;

    total_number_constraints = 0;

    resetSudokuMatrix(sudokuMatrix);//puts zeros in every cell of the sudoku array
    loadConfiguration(fp, filePath, sudokuMatrix);

    printf("\n###########################################################\n");
    printf("############# WELCOME TO SUDOKU Z3 SOLVER ##################\n");
    printf("###########################################################\n\n");

    if(!checkInputConfiguration(sudokuMatrix))
        printf("  The initial SUDOKU configuration is valid\n");
    else{
        printf("\nThe initial SUDOKU configuration isn't valid\n\n");
        displaySudoku(sudokuMatrix);
        return 0;
    }

    printf("\n Input Configuration \n");
    printf(" -------------------\n");
    displaySudoku(sudokuMatrix);

    sudokuPropositionalLogicRepresentation();

    //Here we can start building our Z3 Model
    Z3_context ctx = mk_context();
    Z3_solver  s = mk_solver(ctx);

    true_node = Z3_mk_true (ctx);

    printf("\nBUILDING CONSTRAINTS ... \n\n");
    printf("    **** Each Cell must have one and only digit ..\n");
    printf("    **** Each value is unique on every row ...\n");
    printf("    **** Each value is unique on every column ...\n");
    printf("    **** Each value is unique on each grid column 3x3 ...\n\n");
    start = clock();

    //We dynamically create our 9*9*9=729 boolean variables
    // Each variable will have the name X_ijk -> Cell (i, j) has the label k)
    for (i = 0; i < NUM_PARTITION; i++)
      for (j = 0; j < NUM_PARTITION; j++)
          for (k = 0; k < NUM_PARTITION; k++){
              variables[i][j][k] = mk_bool_var(ctx, variable_list[i][j][k].var_name); 

              if(variable_list[i][j][k].truth_value==1){ // the cell is already assigned a content
                constraints[constraint_counter] = Z3_mk_eq (ctx, variables[i][j][k], true_node);
                constraint_counter ++;  
              }
    }
    
    constraints[constraint_counter] = uniqueLabelling(&ctx,variables);
    constraint_counter ++;
    constraints[constraint_counter] = eachDigitAppearOncePerColumn(&ctx,variables);
    constraint_counter ++;
    constraints[constraint_counter] = eachDigitAppearOncePerRow(&ctx,variables);
    constraint_counter ++;
    constraints[constraint_counter] = eachDigitAppearOncePerGrid(&ctx,variables);
    constraint_counter ++;

    Z3_ast system = Z3_mk_and (ctx, constraint_counter,constraints );
    start = clock() - start;
    cpu_time_used = ((double)start)/CLOCKS_PER_SEC; // in seconds

    printf("*********** It took %f seconds to set up constraints \n", cpu_time_used);
    printf("----------------------------------------------------------\n");

    start = clock();
    Z3_solver_assert(ctx, s, system);
    start = clock() - start;
    cpu_time_used = ((double)start)/CLOCKS_PER_SEC; // in seconds

    cpu_time_used = cpu_time_used + check(ctx, s, Z3_L_TRUE);

    printf("\n*********** It took %f seconds to check the satifiability the model with Z3 \n", cpu_time_used);
    printf("------------------------------------------------------------------------------\n\n");
  
    del_solver(ctx, s);
    Z3_del_context(ctx);
    return 0;

}
