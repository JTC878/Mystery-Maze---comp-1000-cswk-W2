#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <conio.h>
#include <iomanip>

using namespace std;

#include "mazeclass.h"

const int Maze::depthValues[10][13] = { 
  //mazeX  mazeY percPathsofMaze*10  enemyNumber  enemySpotDistance  enemyStep  maxSlow  maxJump  maxTele  maxKill  maxSUTele  maxKeys  maxDoorCount
	{30,     20,         4,              1,               3,			 1,        5,       4,      4,       4,        0,        5,         5},
	{40,     20,         4,              3,               4,			 2,        6,       5,      4,       4,        0,        7,         8},
	{50,     30,         4,              5,               4,			 2,        8,       6,      5,       4,        1,        8,         10},
	{60,     30,         4,              6,               4,			 3,        10,      9,      6,       5,        1,        10,         12},
	{60,     40,         4,              8,               5,			 3,        12,      11,      8,      8,       1,        13,         15},
	{70,     40,         5,              10,			  5,			 4,        14,      13,      12,      10,       1,        15,         17},
	{80,     50,         5,              13,			  6,			 4,        16,      15,      14,      13,       2,        17,         19},
	{90,     50,         5,              16,			  6,			 5,        18,      16,      16,      15,       2,        19,         21},
	{100,    50,         5,              19,			  7,			 6,        20,      18,      19,      18,       1,        21,         23},
	{100,    50,         5,              23,			  8,			 7,        22,      20,      23,      20,      1,        23,         25},
}; //placeholder values 

vector<Item*> Maze::itemList = {};
vector<Enemy*> Maze::enemyList = {};
vector<MazePoint> Maze::doorPoints = {};
vector<MazePoint>& Key::doorPoints = Maze::doorPoints;
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
int initialDepthPrompt();

int main() {

	char key;
	maze1.setDepthCounter(initialDepthPrompt());

	srand(time(0));
	maze1.generateMaze();
	player1.setPos(maze1.midPoint);
	system("cls");
	printFunctions();

	while (true) {
		key = _getche(); //Instead of including multiple maze parameters for playerInput you can just pass a reference to the maze1 object.
		if (player1.playerInput(key, maze1.mazeArr, maze1.pathArr, maze1.exitDoor, maze1.goldenKey, maze1.getMazeSize(), maze1.pathCount)) { //for each item check collect method, if check collect is true then player.collect the item. 
			for (Item* item : maze1.itemList) { //this can be made a function if necessary inside Maze class just make the player object a reference parameter
				if (item->checkCollect(player1.getPlayerPos())) {
					player1.collectItem(item, maze1.itemList);
				}
			}
			for (Enemy* enemy : maze1.enemyList) {
				enemy->movementChoice(maze1.mazeArr, maze1.pathArr, player1.getPlayerPos(), maze1.pathCount);
			}		
		}
		system("cls"); //windows dependant - ncurses?
		printFunctions();
		if (player1.isLevelClear()) {
			levelClearedScreen(maze1.getDepthCounter());
			maze1.clearVectors();
			maze1.generateMaze();
			player1.setPos(maze1.midPoint);
			player1.resetStatus();
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
	system("cls");
	switch (depth) {
	case 1:
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
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		endScreen();
		break;
	}
	
}

void printFunctions() {
	maze1.printMazeArray();
	maze1.printDepthEnemyPathCount();
	Enemy::printEnemyStep();
	player1.printInventory();
}

int initialDepthPrompt() {
	int playerSetDepth = 0;

	cout << "Keybinds: WASD - player movement, SpaceBar - Wait a turn, x - lockpick regular doors, Number keys(1-6) - use items from left to right" << endl << endl;
	cout << "Slow Orb: Slow enemies, useful in the deeper depths. Try to always keep enemy speed at 1 or below." << endl << endl;
	cout << "Jump Orb: Jump in a straight path either over enemies or walls if you stand next to them, these are extremely helpful lifesavers to get away while stuck in a dead end. Direction of the jump is determined by the last movement key pressed." << endl << endl;
	cout << "Tele Orb: Teleport to a random item in the maze, there's no safety precautions with these. Test your luck you might just end up teleporting to a space an enemy is on, or a golden key who knows. If there are no items left in the maze you will teleport to the exit door" << endl << endl;
	cout << "Kill Orb: Kills the nearest enemy, very menacing name but also incredibly valuable. Use these sparingly so you can save them for dire situations or deeper depths..." << endl << endl;
	cout << "Super Tele Orb: A super item. Very rare and also one of the best items to have. Compared to Tele Orbs, these have a safety precaution built in and will never teleport you on top of an enemy. Teleports you to the golden key if you don't have it, if you already do then it will teleport you to the exit door. Almost like a skip button" << endl << endl;
	cout << "Key: Regular rusted old keys that have been scattered throughout the depths, luckily they can unlock any door except for the exit door. Just stand next to one and press the key. Hah." << endl << endl;
	cout << "Golden Key: This is your ticket out of here, this unlocks the exit door to leave the current level, don't need to press anything just step through the door and it'll unlock!! Look for the 'D' at the edge of the maze, you can't miss it." << endl << endl;
	cout << "Lockpicking: Since you're such a skilled and prepared adventurer you brought a magic talking lockpick with you, however it doesn't seem to like you very much, every time you ask for help it asks you to solve one of its riddles first. Be suspicious of this guy." << endl << endl;

	cout << "Depths 1->10 Is there a specific depth you want to start from? Depth 1 is the recommended start." << endl;
	cout << "Enter a Depth: ";
	cin >> playerSetDepth;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "You have entered a wrong input. Enter any depth between 1 and 10: ";
		cin >> playerSetDepth;
	}
	return playerSetDepth;
}








