#pragma once

struct mazePoint {
	int y;
	int x;
};

//(rand() % 101) <= weightedRandomPercY(i)
//(rand() % 101) <= weightedRandomPercX(j)

class Maze {
private:
	int mazeX, mazeY;
	int pathCount;
	int maxPathCount;
	mazePoint midPoint;
	char** mazeArr;
	bool** visitedArr;
	stack<mazePoint> backtrack;

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

public:
	Maze(int x, int y) : mazeX(x), mazeY(y), pathCount(1), midPoint({ (y / 2), (x / 2) }), mazeArr(new char* [y]), visitedArr(new bool* [y]) {
		for (int i = 0; i < y; i++) {
			mazeArr[i] = new char[x];
			visitedArr[i] = new bool[x];
		}
		maxPathCount = (mazeX * mazeY) * 0.5;
	}
	void initialiseMazeArray() {
		for (int i = 0; i < mazeY; i++) {
			for (int j = 0; j < mazeX; j++) {
				mazeArr[i][j] = '#';
			}
		}
	}
	void printMazeArray() {
		for (int i = 0; i < mazeY; i++) {
			cout << endl;
			for (int j = 0; j < mazeX; j++) {
				cout << mazeArr[i][j];
			}
		}
	}
	void printPathCount() {
		cout << endl << "Number of paths: " << pathCount << endl;
	}
	void generateMaze() {
		mazeArr[midPoint.y][midPoint.x] = 'C';
		int i = midPoint.y;
		int j = midPoint.x;
		backtrack.push({ i, j });
		mazePoint top;
		bool found = false;
		int r = 0;
		int loopCounter = 0;
		while (found == false || pathCount < maxPathCount) {
			if (loopCounter > 500) {
				if (i != midPoint.y || j != midPoint.x) {
					backtrack.pop();
					top = backtrack.top();
					i = top.y;
					j = top.x;
				}
				else {
					switch (r) {
					case 0:
						if (i > 1) { //continue this tommorow, loopcounter should not be reset until the main loop resets it itself
							mazeArr[i - 1][j] = ' ';
							i--;
							backtrack.push({ i, j });
							pathCount++;
						}
						break;
					case 1:
						j--;
						backtrack.push({ i, j });
						break;
					case 2:
						i++;
						backtrack.push({ i, j });
						break;
					case 3:
						j++;
						backtrack.push({ i, j });
						break;
					}
				}
			}
			r = rand() % 4;
			switch (r) {
			case 0: //ABOVE case - ISSUE Maze will never end at top or bottom but because of the way memory addressing works for arrays ending at the left or right side is possible 
				if ((rand() % 101) <= weightedRandomPercY(i)) { //if left, right and up is a wall
					if (i < 2) {
						if (found != true) {
							mazeArr[i - 1][j] = ' ';
							i--;
							backtrack.push({ i, j });
							loopCounter = 0;
							pathCount++;
						}
						else {
							backtrack.pop();
							top = backtrack.top();
							i = top.y;
							j = top.x;
						}
					}
					else if (mazeArr[i - 1][j] == '#' && mazeArr[i - 1][j - 1] == '#' && mazeArr[i - 1][j + 1] == '#') {
						mazeArr[i - 1][j] = ' ';
						i--;
						backtrack.push({ i, j });
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 1: //LEFT case
				if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
					if (j < 2) {
						if (found != true) {
							mazeArr[i][j - 1] = ' ';
							j--;
							backtrack.push({ i, j });
							loopCounter = 0;
							pathCount++;
						}
						else {
							backtrack.pop();
							top = backtrack.top();
							i = top.y;
							j = top.x;
						}
					}
					else if (mazeArr[i][j - 1] == '#' && mazeArr[i - 1][j - 1] == '#' && mazeArr[i + 1][j - 1] == '#') {
						mazeArr[i][j - 1] = ' ';
						j--;
						backtrack.push({ i , j });
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 2: //BELOW case
				if ((rand() % 101) <= weightedRandomPercY(i)) { //if left and right is not a path
					if (i > mazeY - 3) {
						if (found != true) {
							mazeArr[i + 1][j] = ' ';
							i++;
							backtrack.push({ i, j });
							loopCounter = 0;
							pathCount++;
						}
						else {
							backtrack.pop();
							top = backtrack.top();
							i = top.y;
							j = top.x;
						}
					}
					else if (mazeArr[i + 1][j] == '#' && mazeArr[i + 1][j - 1] == '#' && mazeArr[i + 1][j + 1] == '#') {
						mazeArr[i + 1][j] = ' ';
						i++;
						backtrack.push({ i, j });
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 3: //RIGHT case
				if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
					if (j > mazeX - 3) {
						if (found != true) {
							mazeArr[i][j + 1] = ' ';
							j++;
							backtrack.push({ i, j });
							loopCounter = 0;
							pathCount++;
						}
						else {
							backtrack.pop();
							top = backtrack.top();
							i = top.y;
							j = top.x;
						}
					}
					else if (mazeArr[i][j + 1] == '#' && mazeArr[i - 1][j + 1] == '#' && mazeArr[i + 1][j + 1] == '#') {
						mazeArr[i][j + 1] = ' ';
						j++;
						backtrack.push({ i, j });
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			default:
				cout << "Something went wrong";
			}
			if (i == 0 || j == 0 || i == (mazeY - 1) || j == (mazeX - 1)) {
				if (found != true) {
					found = true;
					mazeArr[i][j] = 'D';
				}
				i = midPoint.y;
				j = midPoint.x;
				while (!backtrack.empty()) {
					backtrack.pop();
				}
				backtrack.push({ i, j });
			}
		}
	}
	void generateMaze2() {
		/* This routine will use the visitedArray, every step taken will log the neighbour cells as visited, a switch case will be used to decide which path to carve 
		out of, with every step the surrounding cells will be checked if they have been visited or not, if they have they won't be carved as a path. If all surrounding cells have been visited
		then you will take a step back/backtrack with a stack. Each iteration*/
	}
	~Maze() {
		for (int i = 0; i < mazeY; i++) {
			delete[] mazeArr[i];
			delete[] visitedArr[i];
		}
		delete[] mazeArr;
		delete[] visitedArr;
	}
};