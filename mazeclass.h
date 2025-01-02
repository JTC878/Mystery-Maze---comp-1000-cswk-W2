#pragma once

struct MazePoint {
	int y;
	int x;
};

class Item {
public:
	string name;
	MazePoint itemPos;
	int quantity;
	virtual void use() = 0;
};

class SlowOrb : public Item {
public:
	SlowOrb() {
		name = "Slow Orb";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	SlowOrb(MazePoint pos, int quant) {
		name = "Slow Orb";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {

	}
};

class TeleOrb : public Item {
public:
	TeleOrb() {
		name = "Tele Orb";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	TeleOrb(MazePoint pos, int quant) {
		name = "Tele Orb";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {

	}
};

class KillOrb : public Item {
public:
	KillOrb() {
		name = "Kill Orb";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	KillOrb(MazePoint pos, int quant) {
		name = "Kill Orb";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {

	}
};

class SUTeleOrb : public Item {
public:
	SUTeleOrb() {
		name = "Super Tele Orb";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	SUTeleOrb(MazePoint pos, int quant) {
		name = "Super Tele Orb";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {

	}
};

class Key : public Item {
public:
	Key() {
		name = "Key";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	Key(MazePoint pos, int quant) {
		name = "Key";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {

	}
};

class GoldenKey : public Item {
public:
	GoldenKey() {
		name = "Golden Key";
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	GoldenKey(MazePoint pos, int quant) {
		name = "Golden Key";
		itemPos = pos;
		quantity = quant;
	}
	void use() override {
		quantity--;
	}
};

struct Inventory {
	SlowOrb slowOrbs;
	TeleOrb teleOrbs;
	KillOrb killOrbs;
	SUTeleOrb suteleOrbs;
	Key keys;
	GoldenKey goldenKey;
};

//(rand() % 101) <= weightedRandomPercY(i)
//(rand() % 101) <= weightedRandomPercX(j)

class Maze {
private:
	int mazeX, mazeY;
	int pathCount;
	int maxPathCount;
	stack<MazePoint> backtrack;
	vector<Item*> itemList;

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

	void generateGoldenKey() { //only one golden key is generated for each maze
		bool pathFound = false;
		int i, j;
		while (pathFound == false) {
			i = (rand() % (mazeY - 4)) + 2;
			j = (rand() % (mazeX - 4)) + 2;
			int yDifference = abs(exitDoor.y - i);
			int xDifference = abs(exitDoor.x - j);
			//spawn key opposite side of exit door
			if (mazeArr[i][j] == ' ' && (yDifference > midPoint.y || xDifference > midPoint.x)) {
				pathFound = true;
				Item* gKey = new GoldenKey({ i, j }, 1);
				itemList.push_back(gKey);
				mazeArr[i][j] = 184;
			}
		}

	}

public:
	unsigned char** mazeArr;
	bool** pathArr;
	MazePoint midPoint;
	MazePoint exitDoor;
	Maze(int x, int y) : mazeX(x), mazeY(y), pathCount(1), midPoint({ (y / 2), (x / 2) }), exitDoor({ 0, 0 }), mazeArr(new unsigned char* [y]), pathArr(new bool* [y]) {
		for (int i = 0; i < y; i++) {
			mazeArr[i] = new unsigned char[x]; //dynamically allocate the memory for the ammount of columns for each row that has been initialised to create a 2D array. 
			pathArr[i] = new bool[x];
		}
		maxPathCount = (mazeX * mazeY) * 0.5;
	}
	void initialiseMazeArray() {
		for (int i = 0; i < mazeY; i++) {
			for (int j = 0; j < mazeX; j++) {
				mazeArr[i][j] = 219;
				pathArr[i][j] = false;
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
	void printPathArray() {
		for (int i = 0; i < mazeY; i++) {
			cout << endl;
			for (int j = 0; j < mazeX; j++) {
				cout << pathArr[i][j];
			}
		}
	}
	void printPathCount() {
		cout << endl << "Number of paths: " << pathCount << endl;
	}
	void generateMazePaths() {
		mazeArr[midPoint.y][midPoint.x] = 'C';
		int i = midPoint.y;
		int j = midPoint.x;
		backtrack.push({ i, j });
		pathArr[i][j] = true;
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
						if (pathArr[i][j] == true) {
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
							pathArr[i][j] = true;
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
					else if (pathArr[i - 1][j] == false && pathArr[i - 2][j] == false && pathArr[i - 1][j - 1] == false && pathArr[i - 1][j + 1] == false) {
						mazeArr[i - 1][j] = ' ';
						i--;
						backtrack.push({ i, j });
						pathArr[i][j] = true;
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
							pathArr[i][j] = true;
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
					else if (pathArr[i][j - 1] == false && pathArr[i][j - 2] == false && pathArr[i - 1][j - 1] == false && pathArr[i + 1][j - 1] == false) {
						mazeArr[i][j - 1] = ' ';
						j--;
						backtrack.push({ i , j });
						pathArr[i][j] = true;
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
							pathArr[i][j] = true;
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
					else if (pathArr[i + 1][j] == false && pathArr[i + 2][j] == false && pathArr[i + 1][j - 1] == false && pathArr[i + 1][j + 1] == false) {
						mazeArr[i + 1][j] = ' ';
						i++;
						backtrack.push({ i, j });
						pathArr[i][j] = true;
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
							pathArr[i][j] = true;
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
					else if (pathArr[i][j + 1] == false && pathArr[i][j + 2] == false && pathArr[i - 1][j + 1] == false && pathArr[i + 1][j + 1] == false) {
						mazeArr[i][j + 1] = ' ';
						j++;
						backtrack.push({ i, j });
						pathArr[i][j] = true;
						loopCounter = 0;
						pathCount++;
					}
				}
				loopCounter++;
				break;
			default:
				cout << "Something went wrong - generateMaze";
			}
			if (i == 0 || j == 0 || i == (mazeY - 1) || j == (mazeX - 1)) { //if it's at one of the edges of the maze
				if (found != true) {
					found = true;
					mazeArr[i][j] = 'D'; //door will be added at the edge of the maze
					exitDoor.y = i;
					exitDoor.x = j;
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
		generateGoldenKey();
	} //to be implemented - golden key should spawn opposite side of the exit door.
	void generateMaze() {
		initialiseMazeArray();
		generateMazePaths();
		generateItems();
	}
	~Maze() {
		for (int i = 0; i < mazeY; i++) {
			delete[] mazeArr[i];
			delete[] pathArr[i];
		}
		delete[] mazeArr;
		delete[] pathArr;
	}
};

class Player {
	Inventory playerInv;
	MazePoint playerPos;
	bool levelClear;
public:
	Player(MazePoint midPoint) : playerPos(midPoint), playerInv({}), levelClear(false) {}
	bool playerInput(unsigned char keyPress, unsigned char** mazeArr, bool** pathArr, MazePoint exitDoor) {
		if ((keyPress == 'w' || keyPress == 'W') && pathArr[playerPos.y - 1][playerPos.x] == true) {
			if (playerPos.y - 1 == exitDoor.y && playerPos.x == exitDoor.x) {
				if (playerInv.goldenKey.quantity >= 1) {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos.y--;
					mazeArr[playerPos.y][playerPos.x] = 'C';
					levelClear = true;
					playerInv.goldenKey.use();
					return true;
				}
				return false;
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			return true;
		}
		else if ((keyPress == 's' || keyPress == 'S') && pathArr[playerPos.y + 1][playerPos.x] == true) {
			if (playerPos.y + 1 == exitDoor.y && playerPos.x == exitDoor.x) {
				if (playerInv.goldenKey.quantity >= 1) {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos.y++;
					mazeArr[playerPos.y][playerPos.x] = 'C';
					levelClear = true;
					playerInv.goldenKey.use();
					return true;
				}
				return false;
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			return true;
		}
		else if ((keyPress == 'a' || keyPress == 'A') && pathArr[playerPos.y][playerPos.x - 1] == true) {
			if (playerPos.y == exitDoor.y && playerPos.x - 1 == exitDoor.x) {
				if (playerInv.goldenKey.quantity >= 1) {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos.x--;
					mazeArr[playerPos.y][playerPos.x] = 'C';
					levelClear = true;
					playerInv.goldenKey.use();
					return true;
				}
				return false;
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			return true;
		}
		else if ((keyPress == 'd' || keyPress == 'D') && pathArr[playerPos.y][playerPos.x + 1] == true) {
			if (playerPos.y == exitDoor.y && playerPos.x + 1 == exitDoor.x) {
				if (playerInv.goldenKey.quantity >= 1) {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos.x++;
					mazeArr[playerPos.y][playerPos.x] = 'C';
					levelClear = true;
					playerInv.goldenKey.use();
					return true;
				}
				return false;
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			return true;
		}
		return false;
	}
	MazePoint getPlayerPos() {
		return playerPos;
	}
	bool isLevelClear() {
		return levelClear;
	}
	void printInventory() {
		cout << playerInv.slowOrbs.name << " : " << playerInv.slowOrbs.quantity << endl;
		cout << playerInv.teleOrbs.name << " : " << playerInv.teleOrbs.quantity << endl;
		cout << playerInv.killOrbs.name << " : " << playerInv.killOrbs.quantity << endl;
		cout << playerInv.suteleOrbs.name << " : " << playerInv.suteleOrbs.quantity << endl;
		cout << playerInv.keys.name << " : " << playerInv.keys.quantity << endl;
		cout << playerInv.goldenKey.name << " : " << playerInv.goldenKey.quantity << endl;
	}
};

class Enemy {
	MazePoint enemyPos;
	static int enemyStep;
	static bool gameOver;
public:
	Enemy(unsigned char** mazeArr, int mazeX, int mazeY) : enemyPos({ 0, 0 }) {
		MazePoint midPoint = { mazeY / 2, mazeX / 2 };
		int radiusY = mazeY * 0.1; //these can be changed later if necessary
		int radiusX = mazeX * 0.1;
		bool pathFound = false;
		int i, j;
		while (pathFound == false) {
			i = (rand() % (mazeY - 4)) + 2;
			j = (rand() % (mazeX - 4)) + 2;
			if (i < (midPoint.y + radiusY) && i >(midPoint.y - radiusY) && j < (midPoint.x + radiusX) && j >(midPoint.x - radiusX)) {} //do nothing if enemy is within certain range of the middle
			else if (mazeArr[i][j] == ' ') {
				pathFound = true;
				enemyPos.y = i;
				enemyPos.x = j;
				mazeArr[i][j] = 'E';
			}
		}
	}
	void enemyRandomMove(unsigned char** mazeArr, bool** pathArr, MazePoint playerPos) {
		bool hasMoved = false;
		int r;
		while (hasMoved == false) {
			r = rand() % 4;
			switch (r) {
			case 0:
				if (pathArr[enemyPos.y - 1][enemyPos.x] == true && mazeArr[enemyPos.y -1][enemyPos.x] != 'D') {
					mazeArr[enemyPos.y][enemyPos.x] = ' ';
					enemyPos.y--;
					mazeArr[enemyPos.y][enemyPos.x] = 'E';
					hasMoved = true;
				}
				break;
			case 1:
				if (pathArr[enemyPos.y + 1][enemyPos.x] == true && mazeArr[enemyPos.y + 1][enemyPos.x] != 'D') {
					mazeArr[enemyPos.y][enemyPos.x] = ' ';
					enemyPos.y++;
					mazeArr[enemyPos.y][enemyPos.x] = 'E';
					hasMoved = true;
				}
				break;
			case 2:
				if (pathArr[enemyPos.y][enemyPos.x - 1] == true && mazeArr[enemyPos.y][enemyPos.x - 1] != 'D') {
					mazeArr[enemyPos.y][enemyPos.x] = ' ';
					enemyPos.x--;
					mazeArr[enemyPos.y][enemyPos.x] = 'E';
					hasMoved = true;
				}
				break;
			case 3:
				if (pathArr[enemyPos.y][enemyPos.x + 1] == true && mazeArr[enemyPos.y][enemyPos.x + 1] != 'D') {
					mazeArr[enemyPos.y][enemyPos.x] = ' ';
					enemyPos.x++;
					mazeArr[enemyPos.y][enemyPos.x] = 'E';
					hasMoved = true;
				}
				break;
			default:
				cout << "Something went wrong - enemyRandomMove";
			}
		}
		if (enemyPos.y == playerPos.y && enemyPos.x == playerPos.x) {
			gameOver = true;
		}
	}
	void enemyTargetedMove() {}
	void movementChoice() {}
	static bool isGameOver() {
		return gameOver;
	}
	int getEnemyStep() {
		return enemyStep;
	}
};