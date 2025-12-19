APDS – Project 3 - Non-Linear Data Structures
Code by Sara Gibert, Oriol Pros, Francesc Mateu and Jan Porcar
Group: APDS-P3-G10

---

Description

This project implements two combinatorial optimization algorithms: Backtracking (a recursive method that explores all possible paths while pruning unpromising ones)
and Branch and Bound (an approach that evaluates partial solutions with a cost estimate to discard paths that cannot improve the current best solution).


---

Importnat Information!

The Backtracking and Branch and Bound algorithms are in two different .c files:

    - If the user wants to run it using a Backtracking approach:
        - File: Project2_Backtracking.c
        - Maze file: ourMaze.txt (includes the maze we created to test if the algorithm works)


    - If the user wants to run it using a Branch and Bound approach:
        - File: Project2_BranchBound.c
        - Maze file: ourMaze.txt (includes the maze we created to test if the algorithm works)
        - Other files: 
            myCONFIG.c  (includes all side functions that are used in our program)
            myCONFIG.h
            myPQ.c (includes the implementation of the priority queue and its necessary functions to run the code)
            myPQ.h

        ! To run the Branch and Bound algorithm all these files must be included when compiling it !


! Important !

    Line 
        FILE *f = fopen("ourMaze.txt", "r");

    found in Project2_Backtracking for the backtracking (line 53) algorithm, and in the myCONFIG.c for the 
    branch and bound algorithm (line 9), should be changed according to the file the user wants to use.

---

Compilation Instructions

Using GCC:

    - Backtracking algorithm:
            
        gcc Project2_Backtracking.c -o Project2_Backtracking.exe    

        ./Project2_Backtracking.exe 

    - Branch and Bound algorithm:

        gcc Project2_BranchBound.c myCONFIG.c myPQ.c -o Project2_BranchBound.exe  

        ./Project2_BranchBound.exe                                              


---

Code Run Example:

    - Backtracking algorithm:

        FILE *f = fopen("ourMaze.txt", "r")                             (write the file name of the maze the user wants to use)

        gcc Project2_Backtracking.c -o Project2_Backtracking.exe        (compile the program)

        ./Project2_Backtracking.exe                                     (run it)


        FINAL PATH with Backtracking                                    (wait for output)

        ###########
        ## #^>>>>>#
        ##v#^    v#
        #<v#^   #8#
        #v>>>     #
        ###########

        Moves: vv<v>>>^^^>>>>>vv

        Total steps: 17


    - Branch and Bound algorithm:

        FILE *f = fopen("ourMaze.txt", "r")                                             (write the file name of the maze the user wants to use)

        gcc Project2_BranchBound.c myCONFIG.c myPQ.c -o Project2_BranchBound.exe        (compile the program using necessary files)

        ./Project2_BranchBound.exe                                                      (run it)


        FINAL PATH with Branch and Bound                                                (wait for output)

        ###########
        ## #^>>>>>#
        ##v#^    v#
        #<v#^   #8#
        #v>>>     #
        ###########

        Moves: vv<v>>>^^^>>>>>vv

        Total steps: 17



-----------------------------------------

OTHER INFORMATION

---

Prerequisites

- Programming Language: C   
- Compiler: GCC, in matagalls
- IDE: Visual Studio Code  

---

.zip Folder Structure

- `Project2_Backtracking.c`, `Project2_BranchBound.c` – Programs implementing Backtracking and Branch and Bound algorithms.
- `myCONFIG.c`, `myCONFIG.h`, `myPQ.c`, `myPQ.h` - Necessary files to run the Branch and Bound algorithm.  
- `ourMaze.txt` - Example of maze to solve we created

---
