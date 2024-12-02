#include <iostream>
#include <string>
#include <random>
#include <cmath>

#define MAZE_X 50 //make sure these are even
#define MAZE_Y 20
#define ABOVE mazeArr[i - 1][j]
#define BELOW mazeArr[i + 1][j]
#define RIGHT mazeArr[i][j+1]
#define LEFT mazeArr[i][j-1]


struct mazeMidPoint {
	int y = (MAZE_Y / 2) - 1;
	int x = (MAZE_X / 2) - 1;
};

using namespace std;

char mazeArr[MAZE_Y][MAZE_X];
mazeMidPoint midPoint;
void initialiseMazeArray();
void printMazeArray();
void initialiseMainPath();
int checkForPath(int i, int j);


int main() {
	initialiseMazeArray();
	printMazeArray();
	system("pause");
	initialiseMainPath();
	printMazeArray();

	return 0;
}





void initialiseMazeArray() {
	for (int i = 0; i < MAZE_Y; i++) {
		for (int j = 0; j < MAZE_X; j++) {
			mazeArr[i][j] = '#';
		}
	}
}

void printMazeArray() {
	for (int i = 0; i < MAZE_Y; i++) {
		cout << endl;
		for (int j = 0; j < MAZE_X; j++) {
			cout << mazeArr[i][j];
		}
	}
}

int checkForPath(int i, int j) {
	int yPercValue = 0;
	int xPercValue = 0;
	if (i == midPoint.y) yPercValue = 100;
	else if (j == midPoint.x) xPercValue = 100;
	else {
		yPercValue = 100 * (1 - abs(i - midPoint.y) / midPoint.y);
		xPercValue = 100 * (1 - abs(j - midPoint.x) / midPoint.x);
	}
	if (i != 0 && ABOVE == ' ' && rand() % 101 < yPercValue) {
		if (i == (MAZE_Y-1)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (j != 0 && LEFT == ' ' && rand() % 101 < xPercValue) {
		if (j == (MAZE_X - 1)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (i != (MAZE_Y - 1) && BELOW == ' ' && rand() % 101 < yPercValue) {
		if (i == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (j != (MAZE_X - 1) && RIGHT == ' ' && rand() % 101 < xPercValue) {
		if (j == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

void initialiseMainPath() {
	mazeArr[midPoint.y][midPoint.x] = ' ';
	bool found = false;
	while (found == false) {
		for (int i = 0; i < MAZE_Y && found == false; i++) {
			for (int j = 0; j < MAZE_X && found == false; j++) {
				srand(time(0));
				switch (checkForPath(i, j)) {
				case 0:
					mazeArr[i][j] == ' ';
					break;
				case 1:
					mazeArr[i][j] == ' ';
					found = true;
					break;
				default:
					continue;
				}
			}
		}
	}
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
6. Additionally if we want one way paths we must  



*/