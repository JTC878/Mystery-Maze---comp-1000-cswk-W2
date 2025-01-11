
#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#define PDC_WIDE
#include <curses.h>
#include <wchar.h>
#include <locale.h>

using namespace std;

#include "mazeclass.h"

const int Maze::depthValues[10][13] = { 
  //mazeX  mazeY percPathsofMaze*10  enemyNumber  enemySpotDistance  enemyStep  maxSlow  maxJump  maxTele  maxKill  maxSUTele  maxKeys  maxDoorCount
	{30,     20,         4,              1,               5,			 1,        6,       10,      2,       6,        0,        1,         1},
	{40,     20,         4,              7,               5,			 2,        8,       1,      4,       7,        0,        1,         0},
	{50,     30,         4,              11,              5,			 3,        12,      1,      6,       10,       0,        1,         0},
	{60,     30,         4,              17,              5,			 4,        16,      1,      8,       10,       0,        1,         0},
	{70,     40,         4,              25,              5,			 5,        20,      1,      10,      16,       0,        1,         0},
	{80,     40,         5,              35,			  5,			 6,        26,      1,      12,      18,       1,        1,         0},
	{90,     50,         5,              50,			  5,			 7,        35,      1,      20,      30,       1,        1,         0},
	{100,    50,         5,              100,			  5,			 8,        45,      1,      35,      40,       1,        1,         0},
	{110,    60,         5,              200,			  5,			 9,        55,      1,      40,      65,       1,        1,         0},
	{120,    60,         5,              300,			  5,			 10,       65,      1,      50,      70,       1,        1,         0},
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
void printFunctions(WINDOW*, WINDOW*, WINDOW*);
int initialDepthPrompt();
void testCurses();
//in ncurses the cursor determines where on the screen things get printed
//the cursor starts at 0, 0 by default which is the top left of the screen

int main() {
	//testCurses();
	//exit(0);
	setlocale(LC_ALL, "");
	initscr();
	WINDOW* mazeWin = newwin(maze1.getMazeSize().y, maze1.getMazeSize().x, 0, 0);
	WINDOW* mazeStatus = newwin(4, 100, maze1.getMazeSize().y + 4, 10);
	box(mazeStatus, 0, 0);
	WINDOW* invWin = newwin(10, 30, 0, 60);
	box(invWin, 0, 0);
	char key;
	//maze1.setDepthCounter(initialDepthPrompt());

	srand(time(0));
	maze1.generateMaze();
	player1.setPos(maze1.midPoint);
	printFunctions(mazeWin, mazeStatus, invWin);

	while (true) {
		key = getch(); //Instead of including multiple maze parameters for playerInput you can just pass a reference to the maze1 object.
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
		printFunctions(mazeWin, mazeStatus, invWin);
		if (player1.isLevelClear()) {
			levelClearedScreen(maze1.getDepthCounter());
			maze1.clearVectors();
			maze1.generateMaze();
			wresize(mazeWin, maze1.getMazeSize().y, maze1.getMazeSize().x);
			player1.setPos(maze1.midPoint);
			player1.resetStatus();
			printFunctions(mazeWin, mazeStatus, invWin);
			
		}
		if (Enemy::isGameOver()) {
			deathScreen();
		}
	}

	return 0;
}

void testCurses() {
	//initialises the screen, sets up memory 
	initscr();

	int height, width, y, x;
	height = 10;
	width = 20;
	y = 10;
	x = 10;

	WINDOW* win = newwin(height, width, y, x); //without doing anything to it, the window is an invisible part of the terminal
	refresh();

	box(win, 0, 0);
	wrefresh(win); //refreshes a specific window, you don't need to refresh the whole screen if you only update one window 
	//everything thats changed specifically in memory will never get updated to the screen until you call refresh

	int c = getch();

	endwin();
}


void deathScreen() {
	cout << endl << endl << "GAME OVER";
	cout << endl << "You have died by getting hit by an enemy" << endl;
	cout << "Press any button to exit" << endl << endl;
	system("pause");
	endwin();
	exit(0);
}

void endScreen() {
	cout << endl << endl << "Congratulations";
	cout << endl << "You have made it out of the maze!" << endl;
	cout << "Press any button to exit" << endl;
	system("pause");
	endwin();
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

void printFunctions(WINDOW* mazeWin, WINDOW* mazeStatus, WINDOW* invWin) {
	clear();
	refresh();
	maze1.printMazeArray(mazeWin);
	maze1.printDepthEnemyPathCount(mazeStatus);
	//Enemy::printEnemyStep();
	player1.printInventory(invWin);
	refresh();
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
		cin.ignore(1000, '\n');
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

>If there are no items left in the maze(including golden key) you should be able to teleport to the door using normal tele orbs.  





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