//by Erika Johnson 11/3/12

#include <iostream>
#include <fstream>
#include "maze_io.h"

using namespace std;

/*************************************************
 * Open the file specified by filename, and read the 
 * maze into a dynamically allocated array.
 * 
 * Return the pointer to that array (don't delete it..
 * we'll do that in main() ).  Return NULL if
 * you are unable to open the file or can't read
 * the dimensions.
 *
 * We also pass in two pointers to integers. Fill 
 *  those values in with the number of rows and 
 *  columns read by from the file (first two value
 *  in the file)
 *
 *************************************************/

char * read_maze(char *filename, int *rows, int *cols )
{
ifstream ifile(filename);
if ( ! ifile.good() ) {
    return NULL;
  }
ifile >> *rows >> *cols;
char * maze = new char [(*rows)*(*cols)];
for (int i=0; i<(*rows)*(*cols); i++) {
  ifile >> maze[i];
}
return maze;
ifile.close();
}

/*************************************************
 * Print the maze contents in a 2D format to the
 * screen
 *
 *************************************************/
void print_maze(char *maze, int rows, int cols)
{
cout << rows << " " << cols << endl;
for (int i=0; i<rows; i++) {
  for (int j=0; j<cols; j++) {
  		cout << maze[i*cols + j];
	}
	cout << endl;
}

}

/*************************************************
 * Write maze should open the file specified by 
 * filename and write the contents of the maze
 * array to the file with the dimensions on the first
 * line.
 *
 *************************************************/
void write_maze(char *filename, char *maze, int rows, int cols)
{
ofstream ofile(filename);
ofile << rows << " " << cols << endl;
for (int i=0; i<rows; i++) {
	for (int j=0; j<cols; j++) {
  		ofile << maze[i*cols + j];
	}
	ofile << endl;
}
ofile.close();
}