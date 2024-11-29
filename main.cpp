#include <iostream>
#include <string>
#include <random>

#define MAZE_X 50
#define MAZE_Y 20

struct mazeStartingPoint {
	int y = 1;
	int x = 0;
};

using namespace std;

char mazeArr[MAZE_Y][MAZE_X];
void initialiseMazeArray();
void printMazeArray();
void initialiseMainPath();


int main() {
	initialiseMazeArray();
	printMazeArray();
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

void initialiseMainPath() {
	bool found = false;
	int y;
	while (found == false) {
		for (int i = 1; i < MAZE_Y; i++) {
			for (int j = 0; j < MAZE_X; j++) {
				srand(time(0));
				y = rand();
				if (mazeArr[i - 1][j] == ' ' && y % 10 == 0) {
					if (i == MAZE_Y) {
						mazeArr[i][j] == ' ';
						found = true;
						break;
					}
					else {
						mazeArr[i][j] == ' ';
					}
				}
			}
		}
	}
}