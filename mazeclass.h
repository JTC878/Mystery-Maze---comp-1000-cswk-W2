#pragma once

struct MazePoint {
	int y;
	int x;
};

struct Inventory {
	int slowOrbs;
	int teleOrbs;
	int killOrbs;
	int suteleOrbs;
	int keys;
	bool goldenKey;
};

//(rand() % 101) <= weightedRandomPercY(i)
//(rand() % 101) <= weightedRandomPercX(j)

class Maze {
private:
	int mazeX, mazeY;
	int pathCount;
	int maxPathCount;
	bool** visitedArr;
	stack<MazePoint> backtrack;

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
	unsigned char** mazeArr;
	MazePoint midPoint;
	Maze(int x, int y) : mazeX(x), mazeY(y), pathCount(1), midPoint({ (y / 2), (x / 2) }), mazeArr(new unsigned char* [y]), visitedArr(new bool* [y]) {
		for (int i = 0; i < y; i++) {
			mazeArr[i] = new unsigned char[x]; //dynamically allocate the memory for the ammount of columns for each row that has been initialised to create a 2D array. 
			visitedArr[i] = new bool[x];
		}
		maxPathCount = (mazeX * mazeY) * 0.5;
	}
	void initialiseMazeArray() {
		for (int i = 0; i < mazeY; i++) {
			for (int j = 0; j < mazeX; j++) {
				mazeArr[i][j] = 219;
				visitedArr[i][j] = false;
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
	void printVisitedArray() {
		for (int i = 0; i < mazeY; i++) {
			cout << endl;
			for (int j = 0; j < mazeX; j++) {
				cout << visitedArr[i][j];
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
		visitedArr[i][j] = true;
		MazePoint top;
		bool found = false;
		bool visitedFound;
		int r = 0;
		int loopCounter = 0;
		while (found == false || pathCount < maxPathCount) {
			if (loopCounter > 500) { //if the conditionals for carving a path normally cannot be met and the algorithm gets 'stuck', loopCounter is only reset when a new path is carved.
				if (i != midPoint.y || j != midPoint.x) { //if not at the middle of the maze (when the stack has a size of 1) backtrack to the previous mazePoint on the stack
					backtrack.pop();
					top = backtrack.top();
					i = top.y;
					j = top.x;
				}
				else {
					//loop with random points until you go to a path in the visited array where its set to 'true', set i and j to that point.
					visitedFound = false;
					while (visitedFound == false) {
						i = (rand() % (mazeY - 4)) + 2;
						j = (rand() % (mazeX - 4)) + 2;
						if (visitedArr[i][j] == true) {
							visitedFound = true;
							backtrack.push({ i, j });
						}
					}
				}
			}
			r = rand() % 4; //random variable corresponding to up,down,left,right to check whether carving a path is possible from the current position in the subsequent switch case.
			switch (r) {
			case 0: //ABOVE case - ISSUE Maze will never end at top or bottom but because of the way memory addressing works for arrays ending at the left or right side is possible 
				if ((rand() % 101) <= weightedRandomPercY(i)) { //if left, right and up is a wall
					if (i < 3) { //because of the way memory addressing works for arrays this precaution is necessary
						if (found != true) {
							mazeArr[i - 1][j] = ' ';
							i--;
							backtrack.push({ i, j });
							visitedArr[i][j] = true;
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
					else if (visitedArr[i - 1][j] == false && visitedArr[i - 2][j] == false && visitedArr[i - 1][j - 1] == false && visitedArr[i - 1][j + 1] == false) {
						mazeArr[i - 1][j] = ' ';
						i--;
						backtrack.push({ i, j });
						visitedArr[i][j] = true;
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 1: //LEFT case
				if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
					if (j < 3) {
						if (found != true) {
							mazeArr[i][j - 1] = ' ';
							j--;
							backtrack.push({ i, j });
							visitedArr[i][j] = true;
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
					else if (visitedArr[i][j - 1] == false && visitedArr[i][j - 2] == false && visitedArr[i - 1][j - 1] == false && visitedArr[i + 1][j - 1] == false) {
						mazeArr[i][j - 1] = ' ';
						j--;
						backtrack.push({ i , j });
						visitedArr[i][j] = true;
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 2: //BELOW case
				if ((rand() % 101) <= weightedRandomPercY(i)) { //if left and right is not a path
					if (i > mazeY - 4) {
						if (found != true) {
							mazeArr[i + 1][j] = ' ';
							i++;
							backtrack.push({ i, j });
							visitedArr[i][j] = true;
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
					else if (visitedArr[i + 1][j] == false && visitedArr[i + 2][j] == false && visitedArr[i + 1][j - 1] == false && visitedArr[i + 1][j + 1] == false) {
						mazeArr[i + 1][j] = ' ';
						i++;
						backtrack.push({ i, j });
						visitedArr[i][j] = true;
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			case 3: //RIGHT case
				if ((rand() % 101) <= weightedRandomPercX(j)) { //if up and down is not a path
					if (j > mazeX - 4) {
						if (found != true) {
							mazeArr[i][j + 1] = ' ';
							j++;
							backtrack.push({ i, j });
							visitedArr[i][j] = true;
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
					else if (visitedArr[i][j + 1] == false && visitedArr[i][j + 2] == false && visitedArr[i - 1][j + 1] == false && visitedArr[i + 1][j + 1] == false) {
						mazeArr[i][j + 1] = ' ';
						j++;
						backtrack.push({ i, j });
						visitedArr[i][j] = true;
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			default:
				cout << "Something went wrong";
			}
			if (i == 0 || j == 0 || i == (mazeY - 1) || j == (mazeX - 1)) { //if it's at one of the edges of the maze
				if (found != true) {
					found = true;
					mazeArr[i][j] = 'D'; //door will be added at the edge of the maze
				}
				i = midPoint.y;
				j = midPoint.x;
				while (!backtrack.empty()) { //set back to the midpoint and stack is emptied, the midpoints are pushed back onto the stack.
					backtrack.pop();
				}
				backtrack.push({ i, j });
			}
		}
	}
	void generateItems() {
	} //to be implemented
	~Maze() {
		for (int i = 0; i < mazeY; i++) {
			delete[] mazeArr[i];
			delete[] visitedArr[i];
		}
		delete[] mazeArr;
		delete[] visitedArr;
	}
};

class Player {
	Inventory playerInv;
	MazePoint playerPos;
public:
	Player(MazePoint midPoint) : playerPos(midPoint), playerInv({ 0, 0, 0, 0, 0, false }) {}
	void playerInput(unsigned char keyPress, unsigned char** mazeArr) {
		if ((keyPress == 'w' || keyPress == 'W') && mazeArr[playerPos.y - 1][playerPos.x] == ' ') {
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
		}
		else if ((keyPress == 's' || keyPress == 'S') && mazeArr[playerPos.y + 1][playerPos.x] == ' ') {
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
		}
		else if ((keyPress == 'a' || keyPress == 'A') && mazeArr[playerPos.y][playerPos.x - 1] == ' ') {
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
		}
		else if ((keyPress == 'd' || keyPress == 'D') && mazeArr[playerPos.y][playerPos.x + 1] == ' ') {
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
		}
	}
};