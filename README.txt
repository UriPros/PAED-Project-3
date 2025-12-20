APDS – Project 3 - Non-Linear Data Structures
Code by Sara Gibert, Oriol Pros, Francesc Mateu and Jan Porcar
Group: APDS-P3-G10

---

Description

This project implements and applies three non-linear data structures to solve different problems inspired by the Super Structure Bros videogame universe:

    - Graphs (Kart): Used to represent racing circuits and checkpoints, enabling circuit detection and vehicle route optimization.

    - Trees (Brawl): Used to store and manage fighters ordered by attack power, allowing efficient insertion, deletion, visualization, and range-based queries.

    - Tables (Wonder): Used to store game elements by name, enabling fast lookup, spatial queries by area, and statistical analysis by world.


---

Importnat Information:

The Graphs, Trees and Tables algorithms are in three different .c files, as well as third-party implementations:

    - Grpahs:
        - File: graph.c and graph.h
        - Maze file: graphData.txt (wich is equivalent to graphsXS.paed file provided with the statement)
        - Other files corresponding to graphs: 
            graphPQ.c (includes the implementation of the priority queue and its necessary functions to run the code)
            graphPQ.h
        
        ! To run the graphs, and specifically the main algorithm, all these files must be included when compiling it !

    - Trees:
        - File: tree.c and tree.h
        - Maze file: treeData.txt (which is equivalent to treesXXS.paed file provided with the statement)
        
    - Tables:
        - File: table.c and table.h
        - Maze file: tableData.txt (wich is equivalent to tablesM.paed file provided with the statement)
        - Other files corresponding to graphs: 
            tableLL.c (includes the implementation of the linked list and its necessary functions to run the code)
            talbeLL.h
        
        ! To run the table, and specifically the main algorithm, all these files must be included when compiling it !


! Important !

    - Graphs:
        Line 17
            f = fopen("graphData.txt", "r");
        
        should be changed according to the file the user wants to use.

    - Trees:
        Line 145
            f = fopen("treeData.txt", "r");
        
        should be changed according to the file the user wants to use.

    - Tables:
        Line 128
            fp = fopen("tableData.txt", "r");
        
        should be changed according to the file the user wants to use.

---

Compilation Instructions

Using GCC:

    - Main algorithm:
            
        gcc main.c tree.c graph.c graphPQ.c table.c tableLL.c -o main   

        ./main.exe

---

Code Run Example:

Can be seen in the project's statement, since we followed the exact same output strucutre.


-----------------------------------------

OTHER INFORMATION

---

Prerequisites

- Programming Language: C   
- Compiler: GCC, in matagalls
- IDE: Visual Studio Code  

---

.zip Folder Structure

- `graph.c`, `graph.h`, `graphPQ.c`, graphPQ.h` – Programs implementing Graphs.
- `tree.c`, `tree.h` - Programs implementing Trees.
- `table.c`, `table.h`, `talbeLL.c`, `tableLL.h` – Programs implementing Tables.
- `graphData.txt`, `treeData.txt`, `tableData.txt`- Text files containing data for each functionality.
- `main.c`- Main program which englobes and puts together Graphs, Trees and Tables.


---
