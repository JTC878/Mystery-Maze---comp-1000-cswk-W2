#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <stack>
#include <conio.h>

using namespace std;

#include "mazeclass.h"

const int Maze::depthValues[10][11] = { //mazeX, mazeY, percPathsofMaze*10, enemyNumber, enemySpotDistance, enemyStep, maxSlow, maxTele, maxKill, maxSUTele, maxKeys
	{30, 20, 3, 5, 5, 3, 10, 5, 10, 1, 1},
	{50, 30, 3, 8, 5, 3, 10, 5, 10, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	{100, 50, 5, 50, 20, 19, 30, 15, 20, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
}; //placeholder values

vector<Item*> Maze::itemList = {};
vector<Enemy*> Maze::enemyList = {};
vector<Item*>& TeleOrb::itemList = Maze::itemList;
vector<Enemy*>& KillOrb::enemyList = Maze::enemyList;

bool Enemy::gameOver = false;
int Enemy::enemySpotDistance = 5;
float Enemy::enemyStep = 3;
float Enemy::stepRemainder = 0;

Maze maze1; 
Player player1(maze1.midPoint);

void levelClearedScreen(int depth);
void deathScreen();
void endScreen();
void printFunctions();

int main() {
	char key;
	int playerSetDepth = 0;

	cout << "Depths 1->10 Is there a specific depth you want to start from? Depth 1 is the start of the game." << endl;
	cout << "Enter a Depth: ";
	cin >> playerSetDepth;
	maze1.setDepthCounter(playerSetDepth);

	srand(time(0));
	maze1.generateMaze();
	player1.setPos(maze1.midPoint);
	system("cls");
	printFunctions();

	while (true) {
		key = _getche();
		if (player1.playerInput(key, maze1.mazeArr, maze1.pathArr, maze1.exitDoor)) { //for each item check collect method, if check collect is true then player.collect the item. 
			for (Item* item : maze1.itemList) { //this can be made a function if necessary inside Maze class just make the player object a reference parameter
				if (item->checkCollect(player1.getPlayerPos())) {
					player1.collectItem(item, maze1.itemList);
				}
			}
			int enemyStep = Enemy::getEnemyStep();
			for (int i = 0; i < enemyStep; i++) { //all of this can easily be made into a function inside Maze class with player pos parameter
				for (Enemy* enemy : maze1.enemyList) {
					enemy->enemyRandomMove(maze1.mazeArr, maze1.pathArr, player1.getPlayerPos());
				}
			}
		}
		system("cls"); //windows dependant - ncurses?
		printFunctions();
		if (player1.isLevelClear()) {
			levelClearedScreen(maze1.getDepthCounter());
			maze1.enemyList.clear();
			maze1.itemList.clear();
			maze1.generateMaze();
			player1.setPos(maze1.midPoint);
			player1.setLevelClear(false);
			system("cls");
			printFunctions();
			
		}
		if (Enemy::isGameOver()) {
			deathScreen();
		}
	}

	return 0;
}


void deathScreen() {
	cout << endl << endl << "GAME OVER";
	cout << endl << "You have died by getting hit by an enemy" << endl;
	cout << "Press any button to exit" << endl << endl;
	system("pause");
	exit(0);
}

void endScreen() {
	cout << endl << endl << "Congratulations";
	cout << endl << "You have made it out of the maze!" << endl;
	cout << "Press any button to exit" << endl;
	system("pause");
	exit(0);
}

void levelClearedScreen(int depth) {
	char c;
	system("pause");
	system("cls");
	switch (depth) {
	case 1:
		cout << "<enter to continue>" << endl;
		c = getchar();
		cout << "*As you crawl through the pitch black sewers you hear a faint whisper in your ear*" << endl << "<enter to continue>" << endl;
		c = getchar();
		cout << "Well done. However, you have only cleared the very first hurdle." << endl << "<enter to continue>" << endl;
		c = getchar();
		cout << "Be prepared for what lurks in the depths, stock up on anything you can get your hands on." << endl << "<enter to continue>" << endl;
		c = getchar();
		cout << "I'll be waiting for you at the bottom~" << endl << "<enter to continue>" << endl;
		c = getchar();
		break;
	case 2:
		break;
	case 3:
		break;
	}
	
}

void printFunctions() {
	maze1.printMazeArray();
	maze1.printDepthEnemyPathCount();
	Enemy::printEnemyStep();
	player1.printInventory();
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