#ifndef Z3_SOLVER_H
#define Z3_SOLVER_H

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<memory.h>
#include<setjmp.h>
#include <time.h>
#include<z3.h>
#include "utils.h"
#include "z3_solver.h"

/**
   \brief Create a variable using the given name and type.
*/
Z3_ast mk_var(Z3_context ctx, const char * name, Z3_sort ty);

/**
   \brief Create a boolean variable using the given name.
*/
Z3_ast mk_bool_var(Z3_context ctx, const char * name);


/**
   \brief Create a logical context.

   Enable model construction. Other configuration parameters can be passed in the cfg variable.

   Also enable tracing to stderr and register custom error handler.
*/
Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err);


/**
   \brief exit gracefully in case of error.
*/
void exitf(const char* message);


/**
   \brief Simpler error handler.
 */
void error_handler(Z3_context c, Z3_error_code e);


/**
   \brief Display Z3 version in the standard output.
*/
void display_version();

/**
   \brief Create a logical context.

   Enable model construction only.

   Also enable tracing to stderr and register standard error handler.
*/
Z3_context mk_context();


Z3_solver mk_solver(Z3_context ctx);

void del_solver(Z3_context ctx, Z3_solver s);

/**
   \brief Check whether the logical context is satisfiable, and compare the result with the expected result.
   If the context is satisfiable, then display the model.
*/
long check(Z3_context ctx, Z3_solver s, Z3_lbool expected_result);

/**
  * @brief uniqueLabelling: There is at most one digit in each cell
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast uniqueLabelling(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]);

 /**
  * @brief eachDigitAppearOncePerRow: Each digit appears at least once in each row
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerRow(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]);

 /**
  * @brief eachDigitAppearOncePerColumn: Each digit appears at least once in each column
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerColumn(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]);

 /**
  * @brief eachDigitAppearOncePerGrid: Each digit appears at least once in each 3×3 sub-grid
  * @param ctx
  * @param variables
  * @return
  *
  */
 Z3_ast eachDigitAppearOncePerGrid(Z3_context* ctx, Z3_ast variables [NUM_PARTITION][NUM_PARTITION][NUM_PARTITION]);

#endif // Z3_SOLVER_H
