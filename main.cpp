#include <iostream>
#include <string>
#include <random>
#include <cmath>

#define MAZE_X 99 //make sure these are even
#define MAZE_Y 49
#define ABOVE mazeArr[i - 1][j]
#define BELOW mazeArr[i + 1][j]
#define RIGHT mazeArr[i][j+1]
#define LEFT mazeArr[i][j-1]


struct mazeMidPoint {
	int y;
	int x;
};

using namespace std;

char mazeArr[MAZE_Y][MAZE_X];
int pathCount = 1;
mazeMidPoint midPoint = {(MAZE_Y / 2), (MAZE_X / 2)};
void initialiseMazeArray();
void printMazeArray();
void initialiseMainPath();
int checkForPath(int i, int j);
void initialiseMainPath2();


int main() {
	initialiseMazeArray();
	srand(time(0));
	initialiseMainPath2();
	printMazeArray();
	cout << endl << pathCount << endl;

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
	float yPercValue = 2;
	float xPercValue = 2;
	srand(time(0));
	int r = rand() % 10;
	/*
	if (i == midPoint.y || i == 0 || i >= MAZE_Y - 2) yPercValue = 5;
	else {
		yPercValue = abs(i - midPoint.y);
		yPercValue = yPercValue / midPoint.y;
		yPercValue = 1 - yPercValue;
		yPercValue = 10 * yPercValue;
	}
	if (j == midPoint.x || j == 0 || j >= MAZE_X - 2) xPercValue = 5;
	else {
		xPercValue = abs(j - midPoint.x);
		xPercValue = xPercValue / midPoint.x;
		xPercValue = 1 - xPercValue;
		xPercValue = 10 * xPercValue;
	}*/

	if (i != 0 && ABOVE == ' ' && r < (int)yPercValue) {
		if (i == (MAZE_Y-1)) return 1;
		else return 2;
	}
	else if (j != 0 && LEFT == ' ' && r < (int)xPercValue) {
		if (j == (MAZE_X - 1)) return 1;
		else return 2;
	}
	else if (i != (MAZE_Y - 1) && BELOW == ' ' && r < (int)yPercValue) {
		if (i == 0) return 1;
		else return 2;
	}
	else if (j != (MAZE_X - 1) && RIGHT == ' ' && r < (int)xPercValue) {
		if (j == 0) return 1;
		else return 2;
	}
	else {
		return 0;
	}
}

void initialiseMainPath() {
	mazeArr[midPoint.y][midPoint.x] = ' ';
	bool found = false;
	while (found == false) {
		for (int i = 0; i < MAZE_Y && found == false; i++) {
			for (int j = 0; j < MAZE_X && found == false; j++) {
				switch (checkForPath(i, j)) {
				case 1:
					mazeArr[i][j] = ' ';
					found = true;
					break;
				case 2:
					mazeArr[i][j] = ' ';
				default:
					continue;
				}
			}
		}
	}
}

int weightedRandomPercX(int k) {
	float PercValue;
	PercValue = abs(k - midPoint.x);
	PercValue = PercValue / midPoint.x;
	PercValue = 1 - PercValue;
	PercValue = 100 * PercValue;
	if (PercValue < 1) PercValue = 1;
	return int(PercValue);
}

int weightedRandomPercY(int k) {
	float PercValue;
	PercValue = abs(k - midPoint.y);
	PercValue = PercValue / midPoint.y;
	PercValue = 1 - PercValue;
	PercValue = 100 * PercValue;
	if (PercValue < 1) PercValue = 1;
	return int(PercValue);
}

void initialiseMainPath2() { //works to a certain extent but gets stuck because of the wall checks - need to find out how that happens. 
	mazeArr[midPoint.y][midPoint.x] = ' ';
	int i = midPoint.y;
	int j = midPoint.x;
	bool found = false;
	int r = 0;
	int loopCounter = 0;
	while (found == false) {
		r = rand() % 4;
		switch (r) {
		case 0: //ABOVE case - ISSUE Maze will never end at top or bottom but because of the way memory addressing works for arrays ending at the left or right side is possible 
			if ((rand() % 101) <= weightedRandomPercY(i)) { //if left, right and up is a wall
				if (i < 5 || loopCounter > 40) {
					mazeArr[i - 1][j] = ' ';
					i--;
					loopCounter = 0;
					pathCount++;
				}
				else if (mazeArr[i-1][j] == '#' && mazeArr[i - 2][j] == '#' && mazeArr[i - 3][j] == '#' && mazeArr[i - 4][j] == '#') {
					mazeArr[i - 1][j] = ' ';
					i--;
					loopCounter = 0;
					pathCount++;
				}
			}
			loopCounter++;
			break;
		case 1: //LEFT case
			if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
				if (j < 5 || loopCounter > 40) {
					mazeArr[i][j-1] = ' ';
					j--;
					loopCounter = 0;
					pathCount++;
				}
				else if (mazeArr[i][j-1] == '#' && mazeArr[i][j - 2] == '#' && mazeArr[i][j - 3] == '#' && mazeArr[i][j - 4] == '#') {
					mazeArr[i][j-1] = ' ';
					j--;
					loopCounter = 0;
					pathCount++;
				}
			}
			loopCounter++;
			break;
		case 2: //BELOW case
			if ((rand() % 101) <= weightedRandomPercY(i)) { //if left and right is not a path
				if (i > MAZE_Y - 5 || loopCounter > 40) {
					mazeArr[i+1][j] = ' ';
					i++;
					loopCounter = 0;
					pathCount++;
				}
				else if (mazeArr[i+1][j] == '#' && mazeArr[i + 2][j] == '#' && mazeArr[i + 3][j] == '#' && mazeArr[i + 4][j] == '#') {
					mazeArr[i+1][j] = ' ';
					i++;
					loopCounter = 0;
					pathCount++;
				}
			}
			loopCounter++;
			break;
		case 3: //RIGHT case
			if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
				if (j > MAZE_X - 5 || loopCounter > 40) {
					mazeArr[i][j+1] = ' ';
					j++;
					loopCounter = 0;
					pathCount++;
				}
				else if (mazeArr[i][j+1] == '#' && mazeArr[i][j + 2] == '#' && mazeArr[i][j + 3] == '#' && mazeArr[i][j + 4] == '#') {
					mazeArr[i][j+1] = ' ';
					j++;
					loopCounter = 0;
					pathCount++;
				}
			}
			loopCounter++;
			break;
		default:
			cout << "Something went wrong";
		}
		if (i == 0 || j == 0 || i == (MAZE_Y - 1) || j == (MAZE_X - 1)) {
			found = true;
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
6. Additionally if we want one way paths we must check LEFT and RIGHT for other PATHS if you are checking ABOVE OR DOWN from the current element. Vice versa we must check ABOVE and BELOW for 
other PATHS if we are checking RIGHT OR LEFT from the current element.



*/