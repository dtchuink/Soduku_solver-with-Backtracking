#include "utils.h"
#include "z3_solver.h"


/**
   \brief Create a variable using the given name and type.
*/
Z3_ast mk_var(Z3_context ctx, const char * name, Z3_sort ty)
{
    Z3_symbol   s  = Z3_mk_string_symbol(ctx, name);
    return Z3_mk_const(ctx, s, ty);
}

/**
   \brief Create a boolean variable using the given name.
*/
Z3_ast mk_bool_var(Z3_context ctx, const char * name)
{
    Z3_sort ty = Z3_mk_bool_sort(ctx);
    return mk_var(ctx, name, ty);
}


/**
   \brief Create a logical context.

   Enable model construction. Other configuration parameters can be passed in the cfg variable.

   Also enable tracing to stderr and register custom error handler.
*/
Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err)
{
    Z3_context ctx;

    Z3_set_param_value(cfg, "model", "true");
    ctx = Z3_mk_context(cfg);
    Z3_set_error_handler(ctx, err);

    return ctx;
}


/**
   \brief exit gracefully in case of error.
*/
void exitf(const char* message)
{
  fprintf(stderr,"BUG: %s.\n", message);
  exit(1);
}


/**
   \brief Simpler error handler.
 */
void error_handler(Z3_context c, Z3_error_code e)
{
    printf("Error code: %d\n", e);
    exitf("incorrect use of Z3");
}


/**
   \brief Display Z3 version in the standard output.
*/
void display_version()
{
    unsigned major, minor, build, revision;
    Z3_get_version(&major, &minor, &build, &revision);
    printf("Z3 %d.%d.%d.%d\n", major, minor, build, revision);
}

/**
   \brief Create a logical context.

   Enable model construction only.

   Also enable tracing to stderr and register standard error handler.
*/
Z3_context mk_context()
{
    Z3_config  cfg;
    Z3_context ctx;
    cfg = Z3_mk_config();
    ctx = mk_context_custom(cfg, error_handler);
    Z3_del_config(cfg);
    return ctx;
}


Z3_solver mk_solver(Z3_context ctx)
{
  Z3_solver s = Z3_mk_solver(ctx);
  Z3_solver_inc_ref(ctx, s);
  return s;
}

void del_solver(Z3_context ctx, Z3_solver s)
{
  Z3_solver_dec_ref(ctx, s);
}

/**
   \brief Check whether the logical context is satisfiable, and compare the result with the expected result.
   If the context is satisfiable, then display the model.
*/
long check(Z3_context ctx, Z3_solver s, Z3_lbool expected_result)
{
    clock_t start;
    Z3_model m      = 0;

    start = clock();
    Z3_lbool result = Z3_solver_check(ctx, s);
    start = clock() - start;

    switch (result) {
    case Z3_L_FALSE:
        printf("unsat\n");
        break;
    case Z3_L_UNDEF:
        printf("unknown\n");
        m = Z3_solver_get_model(ctx, s);
        if (m) Z3_model_inc_ref(ctx, m);
        printf("potential model:\n%s\n", Z3_model_to_string(ctx, m));
        break;
    case Z3_L_TRUE:
        m = Z3_solver_get_model(ctx, s);
        if (m) Z3_model_inc_ref(ctx, m);
//        printf("sat\n%s\n", Z3_model_to_string(ctx, m));
        parseAndDisplaySolution(Z3_model_to_string(ctx, m));
        break;
    }
    if (result != expected_result) {
        exitf("unexpected result");
    }
    if (m) Z3_model_dec_ref(ctx, m);

    return ((double)start)/CLOCKS_PER_SEC;
}

/**
  * @brief uniqueLabelling: There is at most one digit in each cell
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast uniqueLabelling(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]){

  int i,j,k,l, number_constraints = 0;
  Z3_ast or_constraint, constraint;
  int cell_possible_entries = 9;
  Z3_ast or_variables [cell_possible_entries];
  Z3_ast temp1, temp2;
  Z3_ast uniq_label_constraints[2];
  
  Z3_ast total_constraint [6000]; //contains the constraint for each cell that are conjuncted


   //for each cell(i,j)
   for (i = 0; i < NUM_PARTITION; i++){
      for (j = 0; j < NUM_PARTITION; j++){
         for (k = 0; k < NUM_PARTITION; k++){
             or_variables[k] =  variables[i][j][k];//Xijk: possible assignment into a cell(i,j)
             temp1 = Z3_mk_not (*ctx, variables[i][j][k]); //not Xijk
             uniq_label_constraints[0] = temp1;
             for (l = 0; l < NUM_PARTITION; l++){
                if(l!=k){
                  temp2 = Z3_mk_not (*ctx, variables[i][j][l]);//not Xijl with l!=k
                  uniq_label_constraints[1] = temp2;
                  total_constraint[number_constraints] = Z3_mk_or(*ctx, 2, uniq_label_constraints);
                  number_constraints++;
                }
             }   
      }//end of for k
      or_constraint =  Z3_mk_or (*ctx, cell_possible_entries, or_variables); //The content can be k = 1,2, ..., or 9: Xij1 V ...V Xij9
      total_constraint[number_constraints] = or_constraint;
      number_constraints++;
    }//end of for j
  }//end of for i

   
   constraint = Z3_mk_and (*ctx, number_constraints,total_constraint );
   total_number_constraints = total_number_constraints + number_constraints;
  return constraint;
 }

 /**
  * @brief eachDigitAppearOncePerRow: Each digit appears at least once in each row
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerRow(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]){
    int i,j,k,l, number_constraints = 0;
    Z3_ast clause_constraint, constraint;
    int cell_possible_entries = 9;
    Z3_ast clause_variable [cell_possible_entries];
    total_number_constraints = total_number_constraints + number_constraints;
    Z3_ast temp1, temp2;
    Z3_ast uniq_row_constraints[2];
    Z3_ast all_clause_constraint[10000];

     //for each cell(i,j)
    for (i = 0; i < NUM_PARTITION; i++){   //x
    for (j = 0; j < NUM_PARTITION; j++){      //y
      for (k = 0; k < NUM_PARTITION; k++){     //z
          clause_variable[k] =  variables[i][j][k];//Xijk: possible assignment into a cell(i,j)
          temp1 = Z3_mk_not (*ctx, variables[i][j][k]); //not Xijk
          uniq_row_constraints[0] = temp1;
               for (l = 0; l < NUM_PARTITION; l++){
                   if(l!=j){
                        temp2 = Z3_mk_not (*ctx, variables[i][l][k]);//not Xijl with l!=k
                        uniq_row_constraints[1] = temp2;
                        all_clause_constraint[number_constraints] = Z3_mk_or(*ctx, 2, uniq_row_constraints);
                        number_constraints++;
                   }
                  }
               }
        }//end of for k
        clause_constraint =  Z3_mk_or (*ctx, cell_possible_entries, clause_variable); //The content can be k = 1,2, ..., or 9: Xij1 V ...V Xij9
        all_clause_constraint[number_constraints] = clause_constraint;
        number_constraints++;
      }

      constraint = Z3_mk_and (*ctx, number_constraints, all_clause_constraint );
      total_number_constraints = total_number_constraints + number_constraints;

    return constraint;

  }

 /**
  * @brief eachDigitAppearOncePerColumn: Each digit appears at least once in each column
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerColumn(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]){
    int i,j,k,l, number_constraints = 0;
    Z3_ast clause_constraint, constraint;
    int cell_possible_entries = 9;
    Z3_ast clause_variable [cell_possible_entries];
    Z3_ast temp1, temp2;
    Z3_ast uniq_column_constraints[2];
    Z3_ast all_clause_constraint[10000];

     //for each cell(i,j)
     for (i = 0; i < NUM_PARTITION; i++){ //x
      for (j = 0; j < NUM_PARTITION; j++){   //y
       for (k = 0; k < NUM_PARTITION; k++){  //z
            clause_variable[k] =  variables[i][j][k];//Xijk: possible assignment into a cell(i,j)
            temp1 = Z3_mk_not (*ctx, variables[i][j][k]); //not Xijk
            uniq_column_constraints[0] = temp1;
            for (l = 0; l < NUM_PARTITION; l++){
                if(l!=i){
                    temp2 = Z3_mk_not (*ctx, variables[l][j][k]);//not Xijl with l!=k
                    uniq_column_constraints[1] = temp2;
                    all_clause_constraint[number_constraints] = Z3_mk_or(*ctx, 2, uniq_column_constraints);
                    number_constraints++;
                }
                  }
               }
        }//end of for j
        clause_constraint =  Z3_mk_or (*ctx, cell_possible_entries, clause_variable); //The content can be k = 1,2, ..., or 9: Xij1 V ...V Xij9
        all_clause_constraint[number_constraints] = clause_constraint;
        total_number_constraints = total_number_constraints + number_constraints;
        number_constraints++;
      }

     constraint = Z3_mk_and (*ctx, number_constraints, all_clause_constraint );

    return constraint;

  }

 /**
  * @brief eachDigitAppearOncePerGrid: Each digit appears at least once in each 3×3 sub-grid
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerGrid(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]){
    int i,j,k,x,y, start_row, end_row, start_col, end_col;
    int number_grid_constraints = 0, number_constraints = 0, counter = 0;
    Z3_ast one_Grid_clause_constraint[9], all_clause_constraint[1000];


    for (k = 0; k < NUM_PARTITION; k++){  //z
        for (i = 0; i < NUM_SQUARE; i++){ //x
            for (j = 0; j < NUM_SQUARE; j++){
                 start_row = i*3;
                 end_row = start_row+3;
                 start_col = j*3;
                 end_col = start_col+3;

                 number_grid_constraints =0;
                for (x = start_row; x < end_row; x++){ //x
                    for (y = start_col; y < end_col; y++){

                    one_Grid_clause_constraint[number_grid_constraints] = variables[x][y][k];
                    number_grid_constraints++;
                }
               }
                all_clause_constraint[number_constraints] = Z3_mk_or (*ctx, number_grid_constraints, one_Grid_clause_constraint );
                number_constraints++;

         }
        } //end for i
      }

     Z3_ast constraint = Z3_mk_and (*ctx, number_constraints, all_clause_constraint );
     total_number_constraints = total_number_constraints + number_constraints;

    return constraint;

  }
