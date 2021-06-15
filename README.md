# Soduku_solver-with-Backtracking

1. How to compile and run:
-------------------------
There is a script file "compile.sh" in the root directory of the project. To run the program:

	a. Run ./compile.sh: the z3 library is included in the folder. (works for ubuntu)
	
	b. If you do not have ubuntu, type the following commands (in the root directory):
		gcc -o result  -Wall main.c utils.c z3_solver.c libz3.so -lpthread -w 
		Replace libz3.so by the library of the OS. Example with windows, the name of the library is libz3.dll
    
	c. To run: ./result sudoku_file_name
	
	
2. Logic Implemented:
--------------------

	a. Cell encoding: Let our boolean variables be {X_i,j,k: i,j,k∈{1,…,9}}. 
		The interpretation is that if X_i,j,k is true then cell (i,j) is labelled with k.
	   
	b. Every cell has exactly one label: for all i,j,k {X_i,j,1 , ..., X_i,j,9} and  
		{for all k,l {{!X_i,j,k} V {!X_i,j,k}}, k!=l}
		
	c. In each row, each digit must appear exactly once: For example, let's consider a 4x4 Sudoku grid, and let's focus on row i and digit k. Then the clause Xi0k ∨ Xi1k ∨ Xi2k ∨ Xi3k will guarantee that digit k appears at least once in row i. To guarantee that it appears at most once, we look at every pair of cells in the row, (i,j) and (i,j'), and require that at most one of them contains k: ¬ Vijk ∨ ¬ Vij'k.
	
	d. In each column, each digit must appear exactly once. Like rows, but fix the column j and the digit k. Generate one clause that guarantees k appears at least once in the cells of the column, and then one clause for each pair of cells in the column that guarantees at most one of them contains k.
	
	e. In each block, each digit must appear exactly once 
	
# References:
-------------
	a. Some examples function in test_capi.c from z3 github (https://github.com/Z3Prover/z3/tree/master/examples/c) such as create and destroy the context have been used when learning Z3 from examples.
	
	b. Others lectures:
		1. https://www.cs.ox.ac.uk/people/james.worrell/lec2-2015.pdf
		2. http://sat.inesc.pt/~ines/publications/aimath06.pdf
		
4. Remarks:
----------
	I have noticed that the z3 solver is even faster than the sodoku implemented in the first homework. Maybe it is because we used only 9 boolean to encode 9 int in contrary to the first homework.
	
	
