#pragma once

struct MazePoint {
	int y;
	int x;
};

class Enemy {
	MazePoint enemyPos;
	static bool gameOver;
	static float stepRemainder;
public:
	static float enemyStep;
	static int enemySpotDistance;
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
			else if (mazeArr[i][j] == ' ') { //should be compared to pathArr if you want enemies to spawn on items
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
				if (pathArr[enemyPos.y - 1][enemyPos.x] == true && mazeArr[enemyPos.y - 1][enemyPos.x] != 'D') {
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
	MazePoint getEnemyPos() {
		return enemyPos;
	}
	static bool isGameOver() {
		return gameOver;
	}
	static int getEnemyStep() { //use getEnemyStep instead of the enemyStep attribute when you want to apply this behaviour(only when the enemy makes movements)
		int rmDec = (int)enemyStep;
		stepRemainder += enemyStep - rmDec;
		if (stepRemainder >= 1.0) {
			rmDec = (int)enemyStep + 1;
			stepRemainder = 0;
		}
		if (enemyStep < 0) enemyStep = 0;
		return rmDec;
	} //This allows fractional numbers to be faster or slower than whole numbers - this means slowOrbs have an impact even if they remove a half a step for example.
	static void printEnemyStep() {
		cout << "      Enemy Speed: " << enemyStep << endl;
	}
};

class Item {
public:
	string name;
	unsigned char mazeChar;
	MazePoint itemPos;
	int quantity;
	Item() {
		name = "Item";
		mazeChar = ' ';
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	virtual bool use() {
		quantity--;
		return true;
	}
	bool checkCollect(MazePoint playerPos) {
		if (playerPos.y == itemPos.y && playerPos.x == itemPos.x) {
			return true;
		}
		else {
			return false;
		}
	}
}; //each item has placeholder mazeChars. Probably should be changed later.

class SlowOrb : public Item {
	float slowValue;
public:
	SlowOrb() : slowValue(0.5) {
		name = "Slow Orb";
		mazeChar = 248;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	SlowOrb(MazePoint pos, int quant) : slowValue(0.5) {
		name = "Slow Orb";
		mazeChar = 248;
		itemPos = pos;
		quantity = quant;
	}
	bool use() override {
		if (Enemy::enemyStep == 0) {
			return false;
		}
		Enemy::enemyStep -= slowValue; 
		quantity--;
		return true;
	}
};
//jump orb?? = lastKeyPressed recorded and it will teleport your position in a straight line until there is no more path.
class TeleOrb : public Item {
public:
	static vector<Item*> &itemList;
	TeleOrb() {
		name = "Tele Orb";
		mazeChar = 94;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	TeleOrb(MazePoint pos, int quant) {
		name = "Tele Orb";
		mazeChar = 94;
		itemPos = pos;
		quantity = quant;
	}
	bool use(MazePoint& playerPos, unsigned char** mazeArr) { //you can spawn on enemies 
		if (itemList.empty()) {
			return false;
		}
		int size = itemList.size();
		int i = rand() % size;
		int index = 0;
		for (Item* item : itemList) {
			if (i == index) {
				mazeArr[playerPos.y][playerPos.x] = ' ';
				playerPos = item->itemPos;
				mazeArr[playerPos.y][playerPos.x] = 'C';
				quantity--;
				return true;
			}
			index++;
		}
		return false;
	}
}; //teleport to a random item? or teleport to a random path

class KillOrb : public Item {
	int noOfKills;
public:
	static vector<Enemy*>& enemyList;
	KillOrb() : noOfKills(1) {
		name = "Kill Orb";
		mazeChar = 167;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	KillOrb(MazePoint pos, int quant) : noOfKills(1) {
		name = "Kill Orb";
		mazeChar = 167;
		itemPos = pos;
		quantity = quant;
	}
	bool use(MazePoint playerPos, unsigned char** mazeArr) {
		if (enemyList.empty()) {
			return false;
		}
		double closestDistance = 1000;
		vector<Enemy*>::iterator closestEnemyIndex;
		double xDifference, yDifference, distanceFromPlayer;
		MazePoint enemyPos;
		Enemy* closestEnemy;
		for (int i = 0; i < noOfKills; i++) {
			if (enemyList.empty()) {
				return true;
			}
			for (Enemy* enemy : enemyList) {
				enemyPos = enemy->getEnemyPos();
				xDifference = enemyPos.x - playerPos.x;
				yDifference = enemyPos.y - playerPos.y;
				distanceFromPlayer = hypot(xDifference, yDifference);
				if (distanceFromPlayer < closestDistance) {
					closestDistance = distanceFromPlayer;
					closestEnemyIndex = find(enemyList.begin(), enemyList.end(), enemy);
				}
			}
			closestEnemy = *closestEnemyIndex;
			enemyPos = closestEnemy->getEnemyPos();
			mazeArr[enemyPos.y][enemyPos.x] = ' ';
			enemyList.erase(closestEnemyIndex);
			delete closestEnemy;
		}
		quantity--;
		return true;
	}
}; //kill orb kills the nearest enemy

class SUTeleOrb : public Item {
public:
	SUTeleOrb() {
		name = "Super Tele Orb";
		mazeChar = 173;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	SUTeleOrb(MazePoint pos, int quant) {
		name = "Super Tele Orb";
		mazeChar = 173;
		itemPos = pos;
		quantity = quant;
	}
	bool use(MazePoint& playerPos, unsigned char** mazeArr, MazePoint exitDoor, MazePoint goldenKey, int playerGKeyQuant, MazePoint mazeSize)  {
		if (playerGKeyQuant < 1) {
			if (mazeArr[goldenKey.y][goldenKey.x] == 'E') return false; //we can add more to this check later, for example for fog of war or normal doors return false;
			else {
				mazeArr[playerPos.y][playerPos.x] = ' ';
				playerPos = goldenKey;
				mazeArr[playerPos.y][playerPos.x] = 'C';
				quantity--;
				return true;
			}
		}
		else {
			if (exitDoor.y == 0) {
				if (mazeArr[exitDoor.y + 1][exitDoor.x] == 'E') return false;
				else {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos = {exitDoor.y + 1, exitDoor.x};
					mazeArr[playerPos.y][playerPos.x] = 'C';
					quantity--;
					return true;
				}
			}
			else if (exitDoor.y == mazeSize.y - 1) {
				if (mazeArr[exitDoor.y - 1][exitDoor.x] == 'E') return false;
				else {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos = { exitDoor.y - 1, exitDoor.x };
					mazeArr[playerPos.y][playerPos.x] = 'C';
					quantity--;
					return true;
				}
			}
			else if (exitDoor.x == 0) {
				if (mazeArr[exitDoor.y][exitDoor.x + 1] == 'E') return false;
				else {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos = { exitDoor.y, exitDoor.x + 1 };
					mazeArr[playerPos.y][playerPos.x] = 'C';
					quantity--;
					return true;
				}
			}
			else if (exitDoor.x == mazeSize.x - 1) {
				if (mazeArr[exitDoor.y][exitDoor.x - 1] == 'E') return false;
				else {
					mazeArr[playerPos.y][playerPos.x] = ' ';
					playerPos = { exitDoor.y, exitDoor.x - 1 };
					mazeArr[playerPos.y][playerPos.x] = 'C';
					quantity--;
					return true;
				}
			}
			return false;
		}
	}
};
//super kill orb?? = kills randomly 50% of the enemies in the current maze
class Key : public Item {
	const static string lockpickRiddles[38][2];
public:
	static vector<MazePoint>& doorPoints;
	Key() {
		name = "Key";
		mazeChar = 191;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	Key(MazePoint pos, int quant) {
		name = "Key";
		mazeChar = 191;
		itemPos = pos;
		quantity = quant;
	}
	
	bool use(unsigned char** mazeArr, bool** pathArr, unsigned char lastMoveKeyPressed, MazePoint playerPos) {
		int doorCounter = 0;
		int i = 0;
		int doorArr[4];
		for (vector<MazePoint>::iterator it = doorPoints.begin(); it != doorPoints.end(); it++, i++) { //an iterator loop basically just a safe/smart pointer to an element of a vector. 
			if (((playerPos.y - 1 == it->y || playerPos.y + 1 == it->y) && playerPos.x == it->x) ||
				(playerPos.y == it->y && (playerPos.x - 1 == it->x || playerPos.x + 1 == it->x))) { //if theres a door somewhere around the player
				if ((lastMoveKeyPressed == 'w' || lastMoveKeyPressed == 'W') && playerPos.y - 1 == it->y ||
					(lastMoveKeyPressed == 's' || lastMoveKeyPressed == 'S') && playerPos.y + 1 == it->y ||
					(lastMoveKeyPressed == 'a' || lastMoveKeyPressed == 'A') && playerPos.x - 1 == it->x ||
					(lastMoveKeyPressed == 'd' || lastMoveKeyPressed == 'D') && playerPos.x + 1 == it->x) { //If the door is associated with the direction of the lastKeyPress unlock that door.
					pathArr[it->y][it->x] = true;
					mazeArr[it->y][it->x] = ' ';
					doorPoints.erase(it);
					quantity--;
					return true;
				}
				doorArr[doorCounter] = i;
				doorCounter++;
			}
		}
		if (doorCounter > 0) { //remove a random door
			int randNum = rand() % doorCounter;
			vector<MazePoint>::iterator it = next(doorPoints.begin(), doorArr[randNum]);
			pathArr[it->y][it->x] = true;
			mazeArr[it->y][it->x] = ' ';
			doorPoints.erase(it);
			quantity--;
			return true;
		}
		return false;
	}
	int useLockpick(unsigned char** mazeArr, bool** pathArr, unsigned char lastMoveKeyPressed, MazePoint playerPos) { //https://logiclike.com/en/famous-riddles
		int doorCounter = 0;
		int i = 0;
		int doorArr[4];
		for (vector<MazePoint>::iterator it = doorPoints.begin(); it != doorPoints.end(); it++, i++) { //an iterator loop basically just a safe/smart pointer to an element of a vector. 
			if (((playerPos.y - 1 == it->y || playerPos.y + 1 == it->y) && playerPos.x == it->x) ||
				(playerPos.y == it->y && (playerPos.x - 1 == it->x || playerPos.x + 1 == it->x))) { //if theres a door somewhere around the player
				if ((lastMoveKeyPressed == 'w' || lastMoveKeyPressed == 'W') && playerPos.y - 1 == it->y ||
					(lastMoveKeyPressed == 's' || lastMoveKeyPressed == 'S') && playerPos.y + 1 == it->y ||
					(lastMoveKeyPressed == 'a' || lastMoveKeyPressed == 'A') && playerPos.x - 1 == it->x ||
					(lastMoveKeyPressed == 'd' || lastMoveKeyPressed == 'D') && playerPos.x + 1 == it->x) { //If the door is associated with the direction of the lastKeyPress unlock that door.
					if (!solveRiddle()) {
						return 2;
					}
					pathArr[it->y][it->x] = true;
					mazeArr[it->y][it->x] = ' ';
					doorPoints.erase(it);
					return 1;
				}
				doorArr[doorCounter] = i;
				doorCounter++;
			}
		}
		if (doorCounter > 0) { //remove a random door
			if (!solveRiddle()) {
				return 2;
			}
			int randNum = rand() % doorCounter;
			vector<MazePoint>::iterator it = next(doorPoints.begin(), doorArr[randNum]);
			pathArr[it->y][it->x] = true;
			mazeArr[it->y][it->x] = ' ';
			doorPoints.erase(it);
			quantity--;
			return 1;
		}
		return 3;
	}
	bool solveRiddle() {
		string answer;
		int randRiddle = rand() % 38;
		system("cls");
		cout << "Solve this riddle to lockpick the door successfully." << endl << endl;
		cout << lockpickRiddles[randRiddle][0] << endl << endl;
		cout << "Your answer: ";
		cin >> answer;
		string riddleAnswer = lockpickRiddles[randRiddle][1];
		transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
		transform(riddleAnswer.begin(), riddleAnswer.end(), riddleAnswer.begin(), ::tolower);
		if (answer == riddleAnswer) {
			cout << endl << endl << "Correct.";
			return true;
		}
		else {
			cout << endl << endl << "The answer is " << lockpickRiddles[randRiddle][1] << endl;
			return false;
		}
	}
};
//Potential random item generation with a random mazeChar??
class GoldenKey : public Item {
public:
	GoldenKey() {
		name = "Golden Key";
		mazeChar = 184;
		itemPos = { NULL, NULL };
		quantity = 0;
	}
	GoldenKey(MazePoint pos, int quant) {
		name = "Golden Key";
		mazeChar = 184;
		itemPos = pos;
		quantity = quant;
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
	int depthCounter;
	const static int depthValues[10][12]; //mazeX, mazeY, percPathsofMaze*10, enemyNumber, enemySpotDistance, enemyStep, maxSlow, maxTele, maxKill, maxSUTele, maxKeys, doorCount
	int mazeX, mazeY;
	int pathCount;
	int maxPathCount;
	float percPathsofMaze;
	int enemyNumber;
	unsigned char mazeWallChar, doorChar;
	int maxSlow, maxTele, maxKill, maxSUTele, maxKeys, maxDoorCount;
	const float minItemPercOfMax, minSUItemPercOfMax, minDoorPercOfMax;
	stack<MazePoint> backtrack;

	void deleteMazeArrays() {
		for (int i = 0; i < mazeY; i++) {
			delete[] mazeArr[i];
			delete[] pathArr[i];
		}
		delete[] mazeArr;
		delete[] pathArr;
		while (!backtrack.empty()) {
			backtrack.pop();
		}
	}
	void setMaxPathCount() {
		maxPathCount = (mazeX * mazeY) * percPathsofMaze;
		pathCount = 1;
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
	void generateGoldenKey() { //only one golden key is generated for each maze
		bool pathFound = false;
		int i, j;
		while (pathFound == false) {
			i = (rand() % (mazeY - 2)) + 1;
			j = (rand() % (mazeX - 2)) + 1;
			int yDifference = abs(exitDoor.y - i);
			int xDifference = abs(exitDoor.x - j);
			//spawn key opposite side of exit door
			if (mazeArr[i][j] == ' ' && 
				((exitDoor.y == 0 || exitDoor.y == mazeY - 1) && yDifference > midPoint.y || 
				(exitDoor.x == 0 || exitDoor.x == mazeX - 1) && xDifference > midPoint.x)) { //very important to remember the boolean operator precedance here - ANDs are done first over ORs so you have to add brackets to achieve the right results
				pathFound = true;
				Item* gKey = new GoldenKey({ i, j }, 1);
				goldenKey = { i, j };
				itemList.push_back(gKey);
				mazeArr[i][j] = gKey->mazeChar;
			}
		}
	}
	void generateSlowOrbs() {
		//random range dependant on max SlowOrbs
		int loopCounter = 0;
		int range = (maxSlow + 1) * (1 - minItemPercOfMax); 
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxSlow - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if (mazeArr[i][j] == ' ') {
					loopCounter = 0;
					pathFound = true;
					Item* newSlowOrb = new SlowOrb({ i, j }, 1);
					itemList.push_back(newSlowOrb);
					mazeArr[i][j] = newSlowOrb->mazeChar;
				}
			}
		}
	}
	void generateTeleOrbs() {
		int loopCounter = 0;
		int range = (maxTele + 1) * (1 - minItemPercOfMax);
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxTele - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if (mazeArr[i][j] == ' ') {
					loopCounter = 0;
					pathFound = true;
					Item* newTeleOrb = new TeleOrb({ i, j }, 1);
					itemList.push_back(newTeleOrb);
					mazeArr[i][j] = newTeleOrb->mazeChar;
				}
			}
		}
	}
	void generateKillOrbs() {
		int loopCounter = 0;
		int range = (maxKill + 1) * (1 - minItemPercOfMax);
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxKill - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if (mazeArr[i][j] == ' ') {
					loopCounter = 0;
					pathFound = true;
					Item* newKillOrb = new KillOrb({ i, j }, 1);
					itemList.push_back(newKillOrb);
					mazeArr[i][j] = newKillOrb->mazeChar;
				}
			}
		}
	}
	void generateSUTeleOrbs() {
		int loopCounter = 0;
		int range = (maxSUTele + 1) * (1 - minSUItemPercOfMax);
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxSUTele - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if (mazeArr[i][j] == ' ') {
					loopCounter = 0;
					pathFound = true;
					Item* newSUTeleOrb = new SUTeleOrb({ i, j }, 1);
					itemList.push_back(newSUTeleOrb);
					mazeArr[i][j] = newSUTeleOrb->mazeChar;
				}
			}
		}
	}
	void generateKeys() {
		int loopCounter = 0;
		int range = (maxKeys + 1) * (1 - minItemPercOfMax);
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxKeys - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if (mazeArr[i][j] == ' ') {
					loopCounter = 0;
					pathFound = true;
					Item* newKey = new Key({ i, j }, 1);
					itemList.push_back(newKey);
					mazeArr[i][j] = newKey->mazeChar;
				}
			}
		}
	}
	void generateKeyDoors() {
		/*When you generate a door the position it is at in the pathArr should be set to false
		when you generate doors you should check if there's a wall to the right and left AND if theres a path in front and behind you or vice versa.*/
		int loopCounter = 0;
		int range = (maxDoorCount + 1) * (1 - minDoorPercOfMax);
		if (range == 0) range = 1;
		int rRange = rand() % (range);
		for (int z = 0; z < (maxDoorCount - rRange); z++) {
			bool pathFound = false;
			int i, j;
			while (pathFound == false && loopCounter < 50000) {
				i = (rand() % (mazeY - 2)) + 1;
				j = (rand() % (mazeX - 2)) + 1;
				loopCounter++;
				if ((mazeArr[i - 1][j] == ' ' && mazeArr[i + 1][j] == ' ' && mazeArr[i][j - 1] == mazeWallChar && mazeArr[i][j + 1] == mazeWallChar) ||
					(mazeArr[i][j - 1] == ' ' && mazeArr[i][j + 1] == ' ' && mazeArr[i - 1][j] == mazeWallChar && mazeArr[i + 1][j] == mazeWallChar)) {
					loopCounter = 0;
					pathFound = true;
					doorPoints.push_back({ i, j });
					pathArr[i][j] = false;
					mazeArr[i][j] = doorChar;
				}
			}
		}
	}
	


public:
	unsigned char** mazeArr;
	bool** pathArr;
	static vector<Item*> itemList;
	static vector<Enemy*> enemyList;
	static vector<MazePoint> doorPoints;
	MazePoint midPoint;
	MazePoint exitDoor;
	MazePoint goldenKey;
	Maze() : depthCounter(0), percPathsofMaze((float)depthValues[depthCounter][2] * 0.1), mazeX(depthValues[depthCounter][0]), mazeY(depthValues[depthCounter][1]),
		pathCount(1), midPoint({ (mazeY / 2), (mazeX / 2) }), exitDoor({ 0, 0 }), mazeArr(new unsigned char* [mazeY]), pathArr(new bool* [mazeY]), enemyNumber(depthValues[depthCounter][3]),
		maxSlow(depthValues[depthCounter][6]), maxTele(depthValues[depthCounter][7]), maxKill(depthValues[depthCounter][8]), maxSUTele(depthValues[depthCounter][9]), maxKeys(depthValues[depthCounter][10]),
		maxDoorCount(depthValues[depthCounter][11]), minItemPercOfMax(0.5), minSUItemPercOfMax(1), minDoorPercOfMax(0), mazeWallChar(219), doorChar(194), goldenKey({NULL, NULL}) {
		for (int i = 0; i < mazeY; i++) {
			mazeArr[i] = new unsigned char[mazeX]; //dynamically allocate the memory for the ammount of columns for each row that has been initialised to create a 2D array. 
			pathArr[i] = new bool[mazeX];
		}
		maxPathCount = (mazeX * mazeY) * percPathsofMaze;
	}
	void initialiseMazeArray() { //need to delete the arrays like in the destructor, then allocate new memory to the arrays with the same name, then initialise for new depth mazes

		unsigned char** newMazeArray = new unsigned char* [mazeY];
		bool** newPathArray = new bool* [mazeY];
		for (int i = 0; i < mazeY; i++) {
			newMazeArray[i] = new unsigned char[mazeX]; //'mazeX' ammount of pointers are initialised for each row
			newPathArray[i] = new bool[mazeX];
		}

		mazeArr = newMazeArray;
		pathArr = newPathArray; //assign the members to the new arrays

		for (int i = 0; i < mazeY; i++) {
			for (int j = 0; j < mazeX; j++) {
				mazeArr[i][j] = mazeWallChar;
				pathArr[i][j] = false;
			}
		}
	}
	void printMazeArray() {
		//seperate for each loop - get the position of each item and compare it to the mazeArr position, if there is no enemies on the space assign the position to the item.
		for (Item* item : itemList) {
			MazePoint pos = item->itemPos;
			if (mazeArr[pos.y][pos.x] != 'E') {
				mazeArr[pos.y][pos.x] = item->mazeChar;
			}
		}

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
	void printDepthEnemyPathCount() {
		cout << endl;
		cout << endl << "Depth: " << depthCounter << "       Number of paths: " << pathCount << "      Number of enemies: " << enemyList.size();
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
		generateSlowOrbs();
		generateTeleOrbs();
		generateKillOrbs();
		generateSUTeleOrbs();
		generateKeys();
	} //to be implemented - golden key should spawn opposite side of the exit door.
	void generateEnemies() { //make sure enemy objects are deleted whenever a kill orb is used or when you go to the next level/depth
		for (int i = 0; i < enemyNumber; i++) {
			Enemy* newEnemy = new Enemy(mazeArr, mazeX, mazeY);
			enemyList.push_back(newEnemy);
		}
	}
	void generateMaze() {
		deleteMazeArrays();
		depthUpdateValues();
		setMaxPathCount();
		initialiseMazeArray();
		generateMazePaths();
		generateKeyDoors();
		generateItems();
		generateEnemies();
	}
	void depthUpdateValues() {
		mazeX = depthValues[depthCounter][0];
		mazeY = depthValues[depthCounter][1];
		midPoint.x = mazeX / 2;
		midPoint.y = mazeY / 2;
		percPathsofMaze = (float)depthValues[depthCounter][2] * 0.1;
		enemyNumber = depthValues[depthCounter][3];
		Enemy::enemySpotDistance = depthValues[depthCounter][4];
		Enemy::enemyStep = depthValues[depthCounter][5];
		maxSlow = depthValues[depthCounter][6];
		maxTele = depthValues[depthCounter][7];
		maxKill = depthValues[depthCounter][8];
		maxSUTele = depthValues[depthCounter][9];
		maxKeys = depthValues[depthCounter][10];
		maxDoorCount = depthValues[depthCounter][11];
		depthCounter++;
	}
	int getDepthCounter() {
		return depthCounter;
	}
	void setDepthCounter(int playerInput) {
		if (playerInput < 1) depthCounter = 0;
		if (playerInput > 10) depthCounter = 9;
		depthCounter = playerInput - 1;
	}
	MazePoint getMazeSize() {
		return { mazeY, mazeX };
	}
	void clearVectors() {
		enemyList.clear();
		itemList.clear();
		doorPoints.clear();
		enemyList.shrink_to_fit();
		itemList.shrink_to_fit();
		doorPoints.shrink_to_fit();
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
	string lastItemCollected;
	int lastItemCollectedCounter;
	unsigned char lastMoveKeyPressed;
	bool levelClear;

	bool checkExitDoor(int playerPosY, int playerPosX, MazePoint exitDoor) {
		if (playerPosY == exitDoor.y && playerPosX == exitDoor.x) {
			return true;
		}
		else {
			return false;
		}
	}
	bool checkGoldenKey(int playerPosY, int playerPosX, unsigned char** mazeArr) {
		if (playerInv.goldenKey.quantity >= 1) {
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y = playerPosY;
			playerPos.x = playerPosX;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			levelClear = true;
			return playerInv.goldenKey.use();
		}
		else {
			return false;
		}
	}
	void resetItemQuantity() {
		bool done = false;
		int loopCounter = 0;
		int randNum;
		while (done != true || loopCounter < 100) {
			randNum = rand() % 5;
			switch (randNum) {
			case 0:
				if (playerInv.slowOrbs.quantity > 0) {
					playerInv.slowOrbs.quantity = 0;
					cout << "Somehow your slow orbs have completely disappeared from your pouch";
					done = true;
				}
				break;
			case 1:
				if (playerInv.teleOrbs.quantity > 0) {
					playerInv.teleOrbs.quantity = 0;
					cout << "Somehow your teleport orbs have completely disappeared from your pouch";
					done = true;
				}
				break;
			case 2:
				if (playerInv.killOrbs.quantity > 0) {
					playerInv.killOrbs.quantity = 0;
					cout << "Somehow your kill orbs have completely disappeared from your pouch";
					done = true;
				}
				break;
			case 3:
				if (playerInv.suteleOrbs.quantity > 0) {
					playerInv.suteleOrbs.quantity = 0;
					cout << "Somehow your super teleport orbs have completely disappeared from your pouch";
					done = true;
				}
				break;
			case 4:
				if (playerInv.keys.quantity > 0) {
					playerInv.keys.quantity = 0;
					cout << "Somehow your keys have completely disappeared from your pouch";
					done = true;
				}
				break;
			}
		}
		getchar();
	}

public:
	Player(MazePoint midPoint) : playerPos(midPoint), playerInv({}), levelClear(false), lastItemCollected("None"), lastItemCollectedCounter(0), lastMoveKeyPressed(' ') {}
	bool playerInput(unsigned char keyPress, unsigned char** mazeArr, bool** pathArr, MazePoint exitDoor, MazePoint goldenKeyPos, MazePoint mazeSize) {
		if ((keyPress == 'w' || keyPress == 'W') && pathArr[playerPos.y - 1][playerPos.x] == true) {
			if (checkExitDoor(playerPos.y - 1, playerPos.x, exitDoor)) {
				return checkGoldenKey(playerPos.y - 1, playerPos.x, mazeArr);
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			lastMoveKeyPressed = keyPress;
			return true;
		}
		else if ((keyPress == 's' || keyPress == 'S') && pathArr[playerPos.y + 1][playerPos.x] == true) {
			if (checkExitDoor(playerPos.y + 1, playerPos.x, exitDoor)) {
				return checkGoldenKey(playerPos.y + 1, playerPos.x, mazeArr);
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.y++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			lastMoveKeyPressed = keyPress;
			return true;
		}
		else if ((keyPress == 'a' || keyPress == 'A') && pathArr[playerPos.y][playerPos.x - 1] == true) {
			if (checkExitDoor(playerPos.y, playerPos.x - 1, exitDoor)) {
				return checkGoldenKey(playerPos.y, playerPos.x - 1, mazeArr);
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x--;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			lastMoveKeyPressed = keyPress;
			return true;
		}
		else if ((keyPress == 'd' || keyPress == 'D') && pathArr[playerPos.y][playerPos.x + 1] == true) {
			if (checkExitDoor(playerPos.y, playerPos.x + 1, exitDoor)) {
				return checkGoldenKey(playerPos.y, playerPos.x + 1, mazeArr);
			}
			mazeArr[playerPos.y][playerPos.x] = ' ';
			playerPos.x++;
			mazeArr[playerPos.y][playerPos.x] = 'C';
			lastMoveKeyPressed = keyPress;
			return true;
		}
		else if (keyPress == ' ') { //space to pass your turn
			return true;
		}
		else if (keyPress == 'x' || keyPress == 'X') {
			int outcome = playerInv.keys.useLockpick(mazeArr, pathArr, lastMoveKeyPressed, playerPos);
			if (outcome == 1) return true;
			if (outcome == 2) { 
				resetItemQuantity(); 
				return false;
			}
			if (outcome == 3) return false;
		}
		else if ((keyPress == '1' || keyPress == '!') && playerInv.slowOrbs.quantity > 0) { //use slowOrb
			return playerInv.slowOrbs.use();
		}
		else if ((keyPress == '2' || keyPress == '"') && playerInv.teleOrbs.quantity > 0) { //use teleOrb
			return playerInv.teleOrbs.use(playerPos, mazeArr);
		}
		else if ((keyPress == '3' || keyPress == '£') && playerInv.killOrbs.quantity > 0) { //use killOrb
			return playerInv.killOrbs.use(playerPos, mazeArr);
		}
		else if ((keyPress == '4' || keyPress == '$') && playerInv.suteleOrbs.quantity > 0) {
			return playerInv.suteleOrbs.use(playerPos, mazeArr, exitDoor, goldenKeyPos, playerInv.goldenKey.quantity, mazeSize);
		}
		else if ((keyPress == '5' || keyPress == '%') && playerInv.keys.quantity > 0) {
			return playerInv.keys.use(mazeArr, pathArr, lastMoveKeyPressed, playerPos);
		}
		return false;
	}
	MazePoint getPlayerPos() {
		return playerPos;
	}
	bool isLevelClear() {
		return levelClear;
	}
	void collectItem(Item* itemObject, vector<Item*>& itemList) {
		vector<Item*>::iterator index = find(itemList.begin(), itemList.end(), itemObject);
		itemList.erase(index);
		if (itemObject->name != lastItemCollected) {
			lastItemCollected = itemObject->name;
			lastItemCollectedCounter = 0;
		}
		lastItemCollectedCounter++;
		if (itemObject->name == "Golden Key") {
			playerInv.goldenKey.quantity += itemObject->quantity;
		}
		else if (itemObject->name == "Slow Orb") {
			playerInv.slowOrbs.quantity += itemObject->quantity;
		}
		else if (itemObject->name == "Tele Orb") {
			playerInv.teleOrbs.quantity += itemObject->quantity;
		}
		else if (itemObject->name == "Kill Orb") {
			playerInv.killOrbs.quantity += itemObject->quantity;
		}
		else if (itemObject->name == "Super Tele Orb") {
			playerInv.suteleOrbs.quantity += itemObject->quantity;
		}
		else if (itemObject->name == "Key") {
			playerInv.keys.quantity += itemObject->quantity;
		}
		delete itemObject;
	}
	void printInventory() {
		cout << endl;
		cout << playerInv.slowOrbs.name << "(" << playerInv.slowOrbs.mazeChar << ")" << " : " << playerInv.slowOrbs.quantity;
		cout << setw(18) << playerInv.teleOrbs.name << "(" << playerInv.teleOrbs.mazeChar << ")" << " : " << playerInv.teleOrbs.quantity;
		cout << setw(20) << playerInv.killOrbs.name << "(" << playerInv.killOrbs.mazeChar << ")" << " : " << playerInv.killOrbs.quantity;
		cout << setw(22) << playerInv.suteleOrbs.name << "(" << playerInv.suteleOrbs.mazeChar << ")" << " : " << playerInv.suteleOrbs.quantity;
		cout << setw(15) << playerInv.keys.name << "(" << playerInv.keys.mazeChar << ")" << " : " << playerInv.keys.quantity;
		cout << setw(20) << playerInv.goldenKey.name << "(" << playerInv.goldenKey.mazeChar << ")" << " : " << playerInv.goldenKey.quantity;
		if (lastItemCollectedCounter != 0) {
			cout << endl;
			cout << "+" << lastItemCollectedCounter << " " << lastItemCollected;
		}
	}
	void setPos(MazePoint midpoint) {
		playerPos = midpoint;
	}
	void resetStatus() {
		levelClear = false;
		lastItemCollected = "None";
		lastItemCollectedCounter = 0;
		lastMoveKeyPressed = ' ';
	}
};



const string Key::lockpickRiddles[38][2] = {
	//Reference https://logiclike.com/en/famous-riddles
	{"As I walked along the path I saw something with four fingers and one thumb,\nbut it was not flesh, fish, bone or fowl.", "Glove"}, 
	{"The sun bakes them,\nThe hand breaks them,\nThe foot treads on them,\nAnd the mouth tastes them.\nWhat are they ?", "Grapes"}, 
	{"A precious stone, as clear as diamond.\nSeek it out whilst the sun's near the horizon.\nThough you can walk on water with its power,\nTry to keep it, and it'll vanish within an hour.", "Ice"}, 
	{"I soar without wings, I see without eyes.\nI've traveled the universe to and fro.\nI've conquered the world, yet I've never been anywhere but home.\nWho am I ? ", "Imagination"}, 
	{"Iron roof, glass walls Burns and burns And never falls.", "Lantern"}, 
	{"Walk on the living, they don't even mumble.\nWalk on the dead, they mutter and grumble.", "Leaves"}, 
	{"My tines are long.\nMy tines are short.\nMy tines end ere.\nMy first report.\nWhat am I ? ", "Lightning"}, 
	{"What is always coming but never arrives?", "Tomorrow"}, 
	{"Look at me. I can bring a smile to your face, A tear to your eye,\nOr even a thought to your mind.But, I can't be seen. What am I?", "Memories"}, 
	{"I look at you, you look at me I raise my right,\nyou raise your left What is this object ? ", "Mirror"},
	{"I work when I play and play when I work.", "Musician"}, 
	{"What is so delicate that saying its name breaks it?", "Silence"}, 
	{"What goes up the hill and down the hill, And spite of all, yet standeth still?", "Road"}, 
	{"What is that which belongs to you\nBut others use it more than you do?", "Name"}, 
	{"I have streets, but no pavement.\nI have cities, but no buildings.\nI have forests, yet no trees.\nI have rivers, yet no water.", "Map"}, 
	{"The root tops the trunk on this backward thing,\nthat grows in the winter and dies in the spring.", "Icicle"}, 
	{"What can travel around the world while staying in a corner?", "Stamp"}, 
	{"What has to be broken before you use it?", "Egg"}, 
	{"What has many keys but can't open a single lock?", "Piano"}, 
	{"What runs all around a backyard, yet never moves?", "Fence"},
	{"What has a bottom at the top?", "Legs"}, 
	{"I am an odd number. Take away a letter and I become even.\nWhat number am I?", "Seven"}, 
	{"What goes through cities and fields, but never moves?", "Road"}, 
	{"I'm tall when I'm young and I'm short when I'm old.\nWhat am I ? ", "Candle"}, 
	{"What has hands but can not clap?", "Clock"}, 
	{"You can drop me from the tallest building and I'll be fine,\nbut if you drop me in water I die.\nWhat am I ? ", "Paper"}, 
	{"What has an eye but can not see?", "Needle"}, 
	{"What gets wetter and wetter the more it dries?", "Towel"}, 
	{"There was a green house. Inside the green house there was a white house.\nInside the white house there was a red house.\nInside the red house there were lots of babies.\nWhat is it ? ", "Watermelon"}, 
	{"What kind of room has no doors or windows?", "Mushroom"},
	{"What kind of tree can you carry in your hand?", "Palm"}, 
	{"Which creature walks on four legs in the morning,\ntwo legs in the afternoon, and three legs in the evening ? ", "Man"}, 
	{"Which word in the dictionary is spelled incorrectly?", "Incorrectly"}, 
	{"If you have me, you want to share me. If you share me, you haven't got me.\nWhat am I ? ", "Secret"}, 
	{"What gets broken without being held?", "Promise"}, 
	{"Feed me and I live, yet give me a drink and I die.", "Fire"}, 
	{"Take off my skin - I won't cry, but you will! What am I?", "Onion"}, 
	{"What invention lets you look right through a wall?", "Window"}
};