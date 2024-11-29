#include <iostream>
#include <string>

#define MAZE_X 50
#define MAZE_Y 20

using namespace std;

char mazeArray[MAZE_Y][MAZE_X];
void initialiseMazeArray();
void printMazeArray();
void initialiseRandomMaze();


int main() {
	initialiseMazeArray();
	printMazeArray();
}





void initialiseMazeArray() {
	for (int i = 0; i < MAZE_Y; i++) {
		for (int j = 0; j < MAZE_X; j++) {
			mazeArray[i][j] = '#';
		}
	}
}

void printMazeArray() {
	for (int i = 0; i < MAZE_Y; i++) {
		cout << endl;
		for (int j = 0; j < MAZE_X; j++) {
			cout << mazeArray[i][j];
		}
	}
}

void initialiseRandomMaze() {

}