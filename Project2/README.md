# Implementation of Algorithms

The following algorithms have been implemented in C.
- Optimal binary search tree algorithm
- Tromino tiling algorithm
- Floyd’s algorithm

Files: makefile, obst.c, tromino.c, floyd.c, Floydinput.txt

Compilation: make

Executing code: 
./obst.out <value of n> <n probabilities>
 - Example: ./obst.out 4 0.1 0.2 0.3 0.4
./tromino.out <value of k> <hole position row number> <hole position column number>
 - Example: ./tromino.out 3 4 5
./floyd.out <input file path>