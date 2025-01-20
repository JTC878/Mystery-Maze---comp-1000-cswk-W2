
#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#define PDC_WIDE //enable wide char functions within pdcurses
#include <curses.h>
#include <wchar.h>
#include <locale.h>

using namespace std;

#include "mazeclass.h"

const int Maze::depthValues[10][13] = {
	//mazeX  mazeY percPathsofMaze*10  enemyNumber  enemySpotDistance  enemyStep  maxSlow  maxJump  maxTele  maxKill  maxSUTele  maxKeys  maxDoorCount
	  {30,     20,         4,              1,              3,			   1,        5,       4,       4,       4,        0,        5,         5},
	  {40,     20,         4,              3,              4,			   2,        6,       5,       4,       4,        0,        7,         8},
	  {50,     30,         4,              5,              4,			   2,        8,       6,       5,       4,        1,        8,         10},
	  {60,     30,         4,              6,              4,			   3,        10,      9,       6,       5,        1,        10,        12},
	  {60,     40,         4,              8,              5,			   3,        12,      11,      8,       8,        1,        13,        15},
	  {70,     40,         5,              10,			   5,			   4,        14,      13,      12,      10,       1,        15,        17},
	  {80,     50,         5,              13,			   6,			   4,        16,      15,      14,      13,       2,        17,        19},
	  {90,     50,         5,              16,			   6,			   5,        18,      16,      16,      15,       2,        19,        21},
	  {100,    50,         5,              19,			   7,			   6,        20,      18,      19,      18,       1,        21,        23},
	  {100,    50,         5,              23,			   8,			   7,        22,      20,      23,      20,       1,        23,        25},
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

void levelClearedScreen(int depth, WINDOW*);
void deathScreen();
void endScreen();
void printFunctions(WINDOW*, WINDOW*, WINDOW*, int, MazePoint);
int initialDepthPrompt(WINDOW*);
void resizeAndMoveWindows(WINDOW*, WINDOW*, WINDOW*, int mazeWinY, int mazeWinX);
void testCurses();
//in ncurses the cursor determines where on the screen things get printed
//the cursor starts at 0, 0 by default which is the top left of the screen

int main() {
	setlocale(LC_ALL, "");
	initscr();
	int mazeWinY = player1.getPlayerVisionDistance() * 2 + 3;
	int mazeWinX = player1.getPlayerVisionDistance() * 4 + 3;
	WINDOW* mazeWin = newwin(mazeWinY, mazeWinX, 0, 0);
	WINDOW* mazeStatus = newwin(4, 90, 20, 0);
	WINDOW* invWin = newwin(18, 30, 0, mazeWinX + 2);
	WINDOW* promptWindow = newwin(30, 120, 0, 0);
	char key;
	maze1.setDepthCounter(initialDepthPrompt(promptWindow));

	srand(time(0));
	maze1.generateMaze();
	player1.setPos(maze1.midPoint);
	resizeAndMoveWindows(mazeWin, mazeStatus, invWin, mazeWinY, mazeWinX);
	printFunctions(mazeWin, mazeStatus, invWin, player1.getPlayerVisionDistance(), player1.getPlayerPos());

	while (true) {
		key = getch(); //Instead of including multiple maze parameters for playerInput you can just pass a reference to the maze1 object.
		if (player1.playerInput(key, maze1.mazeArr, maze1.pathArr, maze1.exitDoor, maze1.goldenKey, maze1.getMazeSize(), maze1.pathCount, promptWindow)) { //for each item check collect method, if check collect is true then player.collect the item. 
			for (Item* item : maze1.itemList) { //this can be made a function if necessary inside Maze class just make the player object a reference parameter
				if (item->checkCollect(player1.getPlayerPos())) {
					player1.collectItem(item, maze1.itemList);
				}
			}
			for (Enemy* enemy : maze1.enemyList) {
				enemy->movementChoice(maze1.mazeArr, maze1.pathArr, player1.getPlayerPos(), maze1.pathCount);
			}		
		}
		printFunctions(mazeWin, mazeStatus, invWin, player1.getPlayerVisionDistance(), player1.getPlayerPos());

		if (player1.isLevelClear()) {
			levelClearedScreen(maze1.getDepthCounter(), promptWindow);
			maze1.clearVectors();
			maze1.generateMaze();
			resizeAndMoveWindows(mazeWin, mazeStatus, invWin, mazeWinY, mazeWinX);
			player1.setPos(maze1.midPoint);
			player1.resetStatus();
			printFunctions(mazeWin, mazeStatus, invWin, player1.getPlayerVisionDistance(), player1.getPlayerPos());
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

void levelClearedScreen(int depth, WINDOW* promptWindow) {
	clear();
	refresh();
	box(promptWindow, 0, 0);
	wrefresh(promptWindow);
	mvwprintw(promptWindow, 1, 1, "<enter to continue>");
	wrefresh(promptWindow);
	wgetch(promptWindow);
	int wPos;
	switch (depth) {
	case 1:
		for (int i = 0; i < 4; i++) {
			wPos = 3 * (i + 1);
			mvwprintw(promptWindow, wPos, 1, "%s", levelClearedDialogue[i].c_str());
			mvwprintw(promptWindow, wPos + 1, 1, "<enter to continue>");
			wrefresh(promptWindow);
			wgetch(promptWindow);
		}
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
	wclear(promptWindow);
}

void printFunctions(WINDOW* mazeWin, WINDOW* mazeStatus, WINDOW* invWin, int playerVisionDistance, MazePoint playerPos) {
	wclear(mazeWin);
	wclear(mazeStatus);
	wclear(invWin);
	clear();
	refresh();
	maze1.printMazeArray(mazeWin, playerVisionDistance, playerPos);
	maze1.printDepthEnemyPathCount(mazeStatus);
	player1.printInventory(invWin);
	refresh();
}

int initialDepthPrompt(WINDOW* promptWindow) {
	char playerSetDepth[] = "0;"; //here ; almost acts like a delimiter for the strtol function with end 
	char* end = &playerSetDepth[1];
	refresh();
	box(promptWindow, 0, 0);

	mvwprintw(promptWindow, 1, 1, "Depths 1->10 Is there a specific depth you want to start from? Depth 1 is the recommended start.");
	mvwprintw(promptWindow, 2, 1, "Enter a Depth: ");
	wrefresh(promptWindow);
	refresh();
	wgetnstr(promptWindow, playerSetDepth, 2);
	wclear(promptWindow);
	long res = strtol(playerSetDepth, &end, 10); //third argument is the radix(base) which is 10 for decimal. Will convert the string to an integer until it reaches the delimiter char in end.
	return int(res);
}

void resizeAndMoveWindows(WINDOW* mazeWin, WINDOW* mazeStatus, WINDOW* invWin, int mazeWinY, int mazeWinX) {
	wresize(mazeWin, mazeWinY, mazeWinX);
	mvwin(invWin, 0, mazeWinX + 2);
	mvwin(mazeStatus, 20, 0);
}
/*

Project Backlog

>I think the best avenue for the game would be a fog of war option(fogOption maze attribute and isFogOn() public method) thus allows you to make the game, enemies way easier and items 
more generous(no fog of war easy mode) and an item to expand the vision you have Fog orb. You would do this by editing the printMaze function to have an if statement in the print loop. 
The function would need to take playerPosition as parameter and playerFogRange as parameter(in the Maze class you can assign this to an attribute as a reference to be able to change 
the default value with depth. Aka fogRange attribute). You would check if the current loop for the printed maze is within that range, if it isn't print over it with any character 
not in use in the maze. attribute fogChar. 
Before this you 100% should implement the rest of the items and enemy pathfinding algorithm first. Also fix the screen flickering with some kind of library(search up online for an answer) 
like ncurses.

>Roadside picnic story inspiration~~~


After implementing fog of war
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Add a new item 'Vision orb' which reduces enemy spot distance and increases visibility for the player within the fog.

Add a new item 'super kill orb' which kills a random 50% of the enemies within the maze.

Put all classes with their declarations and definitions in seperate files.

Implement NPC traders using pdcurses, whenever you interact with them open up a shop window UI.

Print the distance to the nearest enemy and enemy vision distance so you can more accurately plan and predict your path in the fog. 

Implement a main menu and save/load file saves 

Make it so your vision is only what's printed and scrolls as you move.

Maybe, Increase the width of the paths more than one tile wide if you plan to implement other mechanics.

*/




