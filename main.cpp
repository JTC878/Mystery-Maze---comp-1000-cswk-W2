#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <stack>

using namespace std;

#include "mazeclass.h"


int main() {
	{
		Maze maze1(100, 50);
		maze1.initialiseMazeArray();
		srand(time(0));
		maze1.generateMaze();
		maze1.printMazeArray();
		maze1.printPathCount();
		maze1.printVisitedArray();
	}

	return 0;
}











/* MAZE GENERATION STEPS

1. Make a starting path in the Center Point of the Maze
2. Loop through the entire maze array while checking whether there are paths around the current element
3. If there is a path around the current element, there should be a weighted random chance the path spreads to the current element
4. This weighted random chance will depend on how far away the current element is from the edge of the maze(Top,Bottom,Right side,Left side)
For example if the path is beneath the current element, then the current element will be compared to the top of the Maze ABS(current Y element - (MAZE_Y / 2 - 1))
This absolute value is then 1 - ANSWER/(MAZE_Y / 2 - 1) to get the percentage value that the path will be initialised in the current element.
5. We must also check whether we have reached the edge of the maze on our current element (eg. IF a path is above us we check if we are at the bottom of the maze - If we are, found
variable will return as true)
6. Additionally if we want one way paths we must check LEFT and RIGHT for other PATHS if you are checking ABOVE OR DOWN from the current element. Vice versa we must check ABOVE and BELOW for 
other PATHS if we are checking RIGHT OR LEFT from the current element.

DUMMY PATH CHARACTERISTICS

1. Paths must always start from the main path (or dummy path) 
2. The number of paths must be between a certain range eg. 100-300 the only exception is if there is no wall to make a path from, the dummy path will end prematurely.
3. Dummy paths must always end in a dead end, they should never connect to another path or reach the end of the maze
4. Dummy paths should be initialised until a total pathCounter reaches a certain number (30% of the maze should be paths when this number is reached, dummy paths will stop being generated)

There should be a mainPaths vector, allPaths vector which stores the struct coords of every paths, and also a dummyPaths vector for storing dummy paths (in order to make puzzles later)
*/