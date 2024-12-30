#pragma once

struct mazePoint {
	int y;
	int x;
};

class Maze {
private:
	int mazeX, mazeY;
	int pathCount;
	int maxPathCount;
	mazePoint midPoint;
	char** mazeArr;
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
	Maze(int x, int y) : mazeX(x), mazeY(y), pathCount(1), midPoint({ (y / 2), (x / 2) }), mazeArr(new char*[y]) {
		for (int i = 0; i < y; i++) {
			mazeArr[i] = new char[x];
		}
		maxPathCount = (mazeX * mazeY) * 0.3;
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
			if (loopCounter > 80) {
				if (i != midPoint.y && j != midPoint.x) {
					backtrack.pop();
					top = backtrack.top();
					i = top.y;
					j = top.x;
				}
				else {
					switch (r) {
					case 0:
						i--;
						backtrack.push({ i, j });
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
				loopCounter = 0;
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
	~Maze() {
		for (int i = 0; i < mazeY; i++) {
			delete[] mazeArr[i];
		}
		delete[] mazeArr;
	}
};