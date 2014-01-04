#include <iostream>
#include <vector>
#include <MyroIO.h>
#include <Myro.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <deque>
#include "robotchallenge_io.h"

using namespace std;

int maze_search(char *, int, int);
 
int main(int argc, char *argv[])
{  
    int rows, cols, result;
    char *mymaze;

    if (argc < 2) {
        cerr << "Usage: ./maze in_filename out_filename" << endl;
        return 1;
    }

    mymaze = read_maze(argv[1], &rows, &cols);

    // For checkpoint 1, just leave maze_search() unedited
    //  and the program should read in the maze, print it
    //  and runs it with the robot
    result = maze_search(mymaze, rows, cols);

    if ( result == 1 ) {
        cout << "Path successfully found!" << endl;
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
    int start, finish, SCount=0, FCount=0, front=0, back=0, check[4], lastIndex, count;
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
        
        cout << rows << " " << cols << endl;
        for (int i=0; i<rows; i++) {
	for (int j=0; j<cols; j++) {
  		cout << maze[i*cols + j];
	}
	cout << endl;
	}

	 connect("/dev/tty.Robby-Fluke2");
	 cout << "Waiting..." << endl;
        
	deque<int> next;
	
        int robotIndex = finish;
        while (robotIndex != start){
        	next.push_front(robotIndex);
        	robotIndex = pred[robotIndex];
	}
	next.push_front(start);
	
	for (int i=0; i<next.size(); i++){
		cout << next[i] << " " << endl;
	}
	
	robotIndex = 0;        
	int curr_dir=next[robotIndex] - next[robotIndex+1], next_dir;
         while (next[robotIndex] != finish){
         next_dir = next[robotIndex] - next[robotIndex+1];
         cout << curr_dir << " " << next_dir << endl;
         if (curr_dir == next_dir){
         	 robot.forward(1.0, 1.8); //don't turn
         //} else if (curr_dir - next_dir == 0){
         	//robot.turnRight(1.0, 0.75); //turn 180 degrees
         	//robot.forward(1.0, 1.8);
    	 } else if (curr_dir - next_dir == 6 || curr_dir - next_dir == -8){
         	robot.turnLeft(1.0, 0.75); //turn 90 degrees counterclockwise
         	robot.forward(1.0, 1.8);
	 } else if (curr_dir - next_dir == -6 || curr_dir - next_dir == 8){
         	robot.turnRight(1.0, 0.75); //turn 90 degrees clockwise
         	robot.forward(1.0, 1.8);
	 }
	 robotIndex++;
	 curr_dir = next_dir;
	 }	    
      
  cout << "Disconnecting" << endl;
  disconnect();
  
  return 1;
}  
