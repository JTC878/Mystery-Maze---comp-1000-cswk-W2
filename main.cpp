#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <stack>
#include <conio.h>
#include <iomanip>

using namespace std;

#include "mazeclass.h"

const int Maze::depthValues[10][12] = { 
  //mazeX  mazeY percPathsofMaze*10  enemyNumber  enemySpotDistance  enemyStep  maxSlow  maxTele  maxKill  maxSUTele  maxKeys  maxDoorCount
	{30,     20,         4,              3,               5,			 1,        6,       2,       6,        0,        1,         0},
	{40,     20,         4,              7,               5,			 2,        8,       4,       7,        0,        1,         0},
	{50,     30,         4,              11,              5,			 3,        12,      6,       10,       0,        1,         0},
	{60,     30,         4,              17,              5,			 4,        16,      8,       10,       0,        1,         0},
	{70,     40,         4,              25,              5,			 5,        20,      10,      16,       0,        1,         0},
	{80,     40,         5,              35,			  5,			 6,        26,      12,      18,       1,        1,         0},
	{90,     50,         5,              50,			  5,			 7,        35,      20,      30,       1,        1,         0},
	{100,    50,         5,              100,			  5,			 8,        45,      35,      40,       1,        1,         0},
	{110,    60,         5,              200,			  5,			 9,        55,      40,      65,       1,        1,         0},
	{120,    60,         5,              300,			  5,			 10,       65,      50,      70,       1,        1,         0},
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
		if (player1.playerInput(key, maze1.mazeArr, maze1.pathArr, maze1.exitDoor, maze1.goldenKey, maze1.getMazeSize())) { //for each item check collect method, if check collect is true then player.collect the item. 
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

	cout << "Depths 1->10 Is there a specific depth you want to start from? Depth 1 is the start." << endl;
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


/*

Project Backlog

> The rest of the core items should be implemented such as Super Tele Orb and keys. 
> For the keys and doors, when you generate doors you can check if there's a wall to the right and left AND if theres a path in front and behind you or vice versa.
This ensures its more likely to be generated at a key part of the maze. There should be a mechanic to use a key or lockpick the door which will prompt a puzzle(not sure what it will be yet)
if you fail the puzzle while lockpicking you will lose a random amount of items.
There will be a seperate keybind for both using a key and choosing to lockpick, however if you press the key keybind without keys it should instead lockpick.

When you generate a door the position it is at in the pathArr should be set to false


>Enemy pathfinding algorithm should be implemented today. When the player is within the enemySpotDistance(hypotenuse), the enemy marks the point where the player was and pathfinds 
towards that location. When the enemy reaches that location it will return to enemyRandomMove. The location is only updated when the player is within the enemySpotDistance. 

>Jump orbs should be implemented(prob 100% necessary). Not too difficult to implement, lastKeyPressed by the player should be recorded as an attribute and when used the orb will loop until it reaches a wall in a 
straight line and then set the player to the path before it. 

>Fix screen flickering 'system("cls)' with a basic library like ncurses have a look if it includes what you need.

>I think the best avenue for the game would be a fog of war option(fogOption maze attribute and isFogOn() public method) thus allows you to make the game, enemies way easier and items 
more generous(no fog of war easy mode) and an item to expand the vision you have Fog orb. You would do this by editing the printMaze function to have an if statement in the print loop. 
The function would need to take playerPosition as parameter and playerFogRange as parameter(in the Maze class you can assign this to an attribute as a reference to be able to change 
the default value with depth. Aka fogRange attribute). You would check if the current loop for the printed maze is within that range, if it isn't print over it with any character 
not in use in the maze. attribute fogChar. 
Before this you 100% should implement the rest of the items and enemy pathfinding algorithm first. Also fix the screen flickering with some kind of library(search up online for an answer) 
like ncurses.

>Roadside picnic story inspiration~~~

> You should print whatever item the player has last picked up(the name), it should increment if you pick up the same item such as +1 Tele Orb ~~ +2 Tele Orb (will make it better for teleports)





*/








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