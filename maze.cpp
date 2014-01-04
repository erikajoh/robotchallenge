//by Erika Johnson 11/3/12
//This program reads in a maze from a specified file, finds the shortest path from start to finish, prints the solved maze, and writes the solution printout to a specified file.

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "maze_io.h"
using namespace std;

// Prototype for maze_search
int maze_search(char *, int, int);

int main(int argc, char *argv[])
{
    int rows, cols, result;
    char *mymaze;

    if (argc < 3) {
        cerr << "Usage: ./maze in_filename out_filename" << endl;
        return 1;
    }

    mymaze = read_maze(argv[1], &rows, &cols);

    // For checkpoint 1, just leave maze_search() unedited
    //  and the program should read in the maze, print it
    //  and write it back to the output file
    result = maze_search(mymaze, rows, cols);

    if ( result == 1 ) {
        cout << "Path successfully found!" << endl;
        print_maze(mymaze,rows,cols);
        write_maze(argv[2],mymaze,rows,cols);
    }
    else if (result == 0) {
        cout << "No path could be found!" << endl;
    }
    else {
        cout << "Error occurred during search!" << endl;
    }

    delete [] mymaze;

    return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 * -1 if an error occurred (no start or finish
 *     cell present in the maze)
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 *************************************************/
int maze_search(char *maze, int rows, int cols)
{
    int start, finish, SCount=0, FCount=0, front=0, back=0, check[4], lastIndex;
    int * pred = new int [rows*cols];
    int * bfsq = new int [rows*cols];

//locate start and finish, mark start as -2, and mark all other cells as -1 (unexplored)
    for (int i=0; i<rows*cols; i++) {
        if (maze[i] == 'S') {
            start = i;
            pred[i] = -2;
            SCount++;
        } else if (maze[i] == 'F') {
            finish = i;
            pred[i] = -1;
            FCount++;
        } else {
            pred[i] = -1;
        }
    }

//make sure there is one start cell and one finish cell (to confirm maze is valid)
    if (SCount != 1 || FCount != 1) {
        return -1;
    }

//put start cell index in breadth-first-search queue
    bfsq[back++] = start;

    while (back > front) { //while bfsq is not empty and finish has not yet been reached
        if (bfsq[front] >= cols) {
            check[0] = bfsq[front] - cols; //sets value for north cell if valid
        } else {
            check[0] = start; //sets value for north cell to start if invalid/ north cell does not exist (will prevent if statement in for loop below from being executed for north)
        }
        if (bfsq[front]%cols != 0) {
            check[1]= bfsq[front] - 1; //sets value for west cell if valid
        } else {
            check[1] = start; //sets value for west cell to start if invalid
        }
        if (bfsq[front] < rows*cols - cols) {
            check[2]= bfsq[front] + cols; //sets value for south cell if valid
        } else {
            check[2] = start; //sets value for south cell if invalid
        }
        if (bfsq[front]%cols != cols - 1) {
            check[3] = bfsq[front] + 1; //sets value for east cell if valid
        } else {
            check[3] = start; //sets value for east cell if invalid
        }

//carries out breadth-first-search, checking in NWSE order, changing predecessor value, and adding value to bfs queue if the cell being checked is unfound and not a wall
        for (int j=0; j<4; j++) {
            if (maze[ check[j] ] != '#' && pred[ check[j] ] == -1) {
                pred[ check[j] ] = bfsq[front];
                bfsq[back] = check[j];
                lastIndex = bfsq[back++]; //keeps track of last index so this information is not lost when bfs is empty
            }
        }
        front++; //drop the first value of the bfsq
    }

//uses predecessor array to follow maze solution path from finish back to start point; changes cells on this path to "*"
    lastIndex = finish;
    lastIndex = pred[lastIndex];
    while (pred[lastIndex] != -2) {
        maze[lastIndex] = '*';
        lastIndex = pred[lastIndex];
    }
    return 1;
    
    delete [] bfsq;
    delete [] pred;
}
