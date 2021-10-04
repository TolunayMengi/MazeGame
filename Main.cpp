// 
// SIMPLE MAZE GAME
// 
// Author: Tolunay Mengi
// Maze Generation Algorithm Link: https://en.wikipedia.org/wiki/Maze_generation_algorithm
// Iterative implementation algorithm used

// define this directive  to disable unsafe error messages
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <stack>
#include <vector>
#include <conio.h>
#include <ctime>

// Maze variables
#define ROAD			(char(0))
#define WALL			(char(1))
#define MONSTER			(char(2))
#define GOLD			(char(3))
#define PLAYER			(char(4))
#define EXIT			(char(5))

// Printed character variables
#define CHARACTER_1		(char(1))
#define CHARACTER_2		(char(2))
#define CHAR_MAZE_WALL	(char(219))
#define CHAR_GOLD		(char(71))
#define CHAR_MONSTER	(char(77))
#define CHAR_EXIT		(char(69))

// Decleration Cell structure of maze
struct Cell
{
	int x;	// x coordinate of cell
	int y;	// y coordinate of cell
};

using namespace std;

// Function forward declerations
unsigned char selectCharacter();
void generateMaze(unsigned char** &maze, int &dimentionOfMaze, int &goldCount);
void movePlayer(unsigned char** maze, int dimentionOfMaze, unsigned char selectedCharacter, int &gameScore, FILE * logFile, int &collectedGoldCount);
void printMaze(unsigned char** maze, int dimentionOfMaze, unsigned char selectedPlayer);
bool openLogFile(FILE * &logFile);
bool closeLogFile(FILE * &logFile);
void copyPointer2Pointer(unsigned char** &source, unsigned char** &destination, int dimentionOfMaze);

int main(int argc, char** argv)
{
	// log file decleration
	FILE * logFile;

	// open log file for writing log informations
	openLogFile(logFile);

	// Use current time as seed for random generator
	srand(time(NULL));

	// 2 dimentional maze declaration
	unsigned char ** maze;
	
	// Selected character variable
	unsigned char selectedCharacter;

	// Dimention of maze
	int dimentionOfMaze;

	// Count of gold in maze
	int goldCount;

	// Game score
	int gameScore = 0;

	// Collected gold count
	int collectedGoldCount = 0;

	// Assign selected caracter
	selectedCharacter = selectCharacter();

	// Generate Maze
	generateMaze(maze, dimentionOfMaze, goldCount);

	// Move Player
	movePlayer(maze, dimentionOfMaze, selectedCharacter, gameScore, logFile, collectedGoldCount);

	// Game over, print game score
	cout << "Collected Gold Count: " << collectedGoldCount << endl;
	cout << "TOTAL SCORE: " << gameScore << endl;

	// Close log file
	closeLogFile(logFile);

	system("pause");
	return 0;
}

void printMaze(unsigned char** maze, int dimentionOfMaze, unsigned char selectedCharacter)
{
	// Print maze's up frame
	for (int i = 0; i < dimentionOfMaze + 2; i++)
	{
		cout << CHAR_MAZE_WALL << CHAR_MAZE_WALL;
	}
	cout << endl;

	// Print maze
	for (int index_y = 0; index_y < dimentionOfMaze; index_y++)
	{
		// Print maze's left frame
		cout << CHAR_MAZE_WALL << CHAR_MAZE_WALL;
		// Print maze
		for (int index_x = 0; index_x < dimentionOfMaze; index_x++)
		{
			switch (maze[index_x][index_y])
			{
			case WALL:
				cout << CHAR_MAZE_WALL << CHAR_MAZE_WALL;
				break;
			case ROAD:
				cout << "  ";
				break;
			case GOLD:
				cout << CHAR_GOLD << " ";
				break;
			case MONSTER:
				cout << CHAR_MONSTER << " ";
				break;
			case PLAYER:
				cout << selectedCharacter << " ";
				break;
			case EXIT:
				cout << CHAR_EXIT << " ";
				break;
			default:
				;
			}
		}
		// Print maze's right frame
		cout << CHAR_MAZE_WALL << CHAR_MAZE_WALL;
		cout << endl;
	}

	// Print maze's down frame
	for (int i = 0; i < dimentionOfMaze + 2; i++)
	{
		cout << CHAR_MAZE_WALL << CHAR_MAZE_WALL;
	}
	cout << endl;
}

void movePlayer(unsigned char** maze, int dimentionOfMaze, unsigned char selectedCharacter, int &gameScore, FILE * logFile, int &collectedGoldCount)
{
	// Set players initial coordinate in maze
	maze[0][0] = PLAYER;

	// Copy of maze
	unsigned char** copyMaze;

	// Memory allocation for First dimention 
	copyMaze = new unsigned char *[dimentionOfMaze];

	// Memory allocation for Second dimention
	for (int i = 0; i < dimentionOfMaze; i++)
	{
		copyMaze[i] = new unsigned char[dimentionOfMaze];
	}

	copyPointer2Pointer(maze, copyMaze, dimentionOfMaze);

	// Assign it to Cell struct
	Cell playerIndex = { 0,0 };

	// Print log file to visited coordinates
	fprintf(logFile, "PATH COORDINATES\n");
	fprintf(logFile, "X: %2d, Y: %2d\n", playerIndex.x, playerIndex.y);

	// Holds variable if game is over
	bool isGameOver = false;

	// Character that hold player movement
	unsigned char playerMovement;

	// While game is not over
	while (isGameOver == false)
	{
		// Clear command prompt
		system("CLS");
		// Print score
		cout << "SCORE:" << gameScore << endl;
		// Print game informations
		cout << "G:GOLD     M:MONSTER     E:EXIT" << endl;
		// Print movement informations
		cout << "Move player(W:Up, A:Left, S:Down, D:Right): " << endl;
		// Print maze
		printMaze(maze, dimentionOfMaze, selectedCharacter);
		// Get char from user
		playerMovement = _getch();

		// Move player according to gotten character
		switch (playerMovement)
		{
		case 'w':
			if (playerIndex.y - 1 >= 0 && maze[playerIndex.x][playerIndex.y - 1] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.y = playerIndex.y - 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if(maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'W':
			if (playerIndex.y - 1 >= 0 && maze[playerIndex.x][playerIndex.y - 1] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.y = playerIndex.y - 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'a':
			if (playerIndex.x - 1 >= 0 && maze[playerIndex.x - 1][playerIndex.y] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.x = playerIndex.x - 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'A':
			if (playerIndex.x - 1 >= 0 && maze[playerIndex.x - 1][playerIndex.y] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.x = playerIndex.x - 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 's':
			if (playerIndex.y + 1 <= dimentionOfMaze - 1 && maze[playerIndex.x][playerIndex.y + 1] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.y = playerIndex.y + 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'S':
			if (playerIndex.y + 1 <= dimentionOfMaze - 1 && maze[playerIndex.x][playerIndex.y + 1] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.y = playerIndex.y + 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'd':
			if (playerIndex.x + 1 <= dimentionOfMaze - 1 && maze[playerIndex.x + 1][playerIndex.y] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.x = playerIndex.x + 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		case 'D':
			if (playerIndex.x + 1 <= dimentionOfMaze - 1 && maze[playerIndex.x + 1][playerIndex.y] != WALL)
			{
				maze[playerIndex.x][playerIndex.y] = ROAD;
				playerIndex.x = playerIndex.x + 1;
				if (maze[playerIndex.x][playerIndex.y] == GOLD)
				{
					gameScore += 10;
					collectedGoldCount++;
				}
				else if (maze[playerIndex.x][playerIndex.y] == MONSTER)
				{
					copyPointer2Pointer(copyMaze, maze, dimentionOfMaze);
					gameScore = 0;
					playerIndex = { 0,0 };
				}
				else if (maze[playerIndex.x][playerIndex.y] == EXIT)
				{
					isGameOver = true;
					gameScore += 50;
				}
				maze[playerIndex.x][playerIndex.y] = PLAYER;
			}
			break;
		default:
			;
		}
		// Print player coordinate to the log file
		fprintf(logFile, "X: %2d, Y: %2d\n", playerIndex.x, playerIndex.y);
	}
}

void generateMaze(unsigned char** &maze, int &dimentionOfMaze, int &goldCount)
{
	// Clear command prompt
	system("CLS");
	// Get dimention variable from user
	cout << "Please enter dimention of maze between 10 - 100: ";
	cin >> dimentionOfMaze;

	// If entered value is not in desired range
	while (dimentionOfMaze < 10 || dimentionOfMaze > 100)
	{
		cout << "Input value is not in desired range!" << endl;
		cout << "Please enter dimention of maze between 10 - 100: ";
		cin >> dimentionOfMaze;
	}

	// Stack structure contains cells will be visited
	stack<Cell> stackOfCells;

	// Hold that mase's visited cell status for every cell
	bool ** isCellVisited;

	// If entered value is in desired range
	// Allocate memory for maze

	// Memory allocation for First dimention 
	maze = new unsigned char *[dimentionOfMaze];
	isCellVisited = new bool *[dimentionOfMaze];

	// Memory allocation for Second dimention
	for (int i = 0; i < dimentionOfMaze; i++)
	{
		maze[i] = new unsigned char[dimentionOfMaze];
		isCellVisited[i] = new bool[dimentionOfMaze];
	}

	// Set maze as all WALL
	for (int i = 0; i < dimentionOfMaze; i++)
	{
		for (int j = 0; j < dimentionOfMaze; j++)
		{
			maze[i][j] = WALL;
			isCellVisited[i][j] = false;
		}
	}


	// ***MAZE GENERATİON***

	// 1. Choose the initial cell, mark it as visited and push it to the stack

	// Initial cell definition
	Cell initialCell = { 0, 0 };

	//Cell* exitCell = NULL;
	// Vector that holds exit cell's coordinates
	vector<Cell> mazeEndCells;
	bool isEnd = true;

	// Set initial ROAD coordinate for maze creation algorithm
	maze[0][0] = ROAD;

	// Set visited cell index as true
	isCellVisited[initialCell.x][initialCell.y] = true;

	// Push initial cell to the stack
	stackOfCells.push(initialCell);

	// 2. While the stack is not empty
	while (!stackOfCells.empty())
	{	
		// Holds current cell coordinate
		Cell currentCell;

		// Get current cell informations from stack
		currentCell = stackOfCells.top();

		// Indexes of cell's
		int x = currentCell.x;
		int y = currentCell.y;

		// Define neighbour cells for computation
		Cell leftNeighbour = { x - 1, y };
		Cell rightNeighbour = { x + 1, y };
		Cell upNeighbour = { x, y + 1 };
		Cell downNeighbour = { x, y - 1 };

		// visitible neighbour cells
		vector<Cell> visitibleNeighbourCells;

		// 2.2. If the current cell has any neighbours which have not been visited
		if ((leftNeighbour.x >= 0) && !isCellVisited[leftNeighbour.x][leftNeighbour.y])
		{
			if (((leftNeighbour.x - 1 < 0) || maze[leftNeighbour.x - 1][leftNeighbour.y]) && ((leftNeighbour.y - 1 < 0) || maze[leftNeighbour.x][leftNeighbour.y - 1]) && ((leftNeighbour.y + 1 > dimentionOfMaze - 1) || maze[leftNeighbour.x][leftNeighbour.y + 1]))
				visitibleNeighbourCells.push_back(leftNeighbour);
		}

		if ((rightNeighbour.x <= dimentionOfMaze - 1) && !isCellVisited[rightNeighbour.x][y])
		{
			if (((rightNeighbour.x + 1 > dimentionOfMaze - 1) || maze[rightNeighbour.x + 1][rightNeighbour.y]) && ((rightNeighbour.y - 1 < 0) || maze[rightNeighbour.x][rightNeighbour.y - 1]) && ((rightNeighbour.y + 1 > dimentionOfMaze - 1) || maze[rightNeighbour.x][rightNeighbour.y + 1]))
				visitibleNeighbourCells.push_back(rightNeighbour);
		}

		if ((upNeighbour.y <= dimentionOfMaze - 1) && !isCellVisited[upNeighbour.x][upNeighbour.y])
		{
			if (((upNeighbour.x - 1 < 0) || maze[upNeighbour.x - 1][upNeighbour.y]) && ((upNeighbour.x + 1 > dimentionOfMaze - 1) || maze[upNeighbour.x + 1][upNeighbour.y]) && ((upNeighbour.y + 1 > dimentionOfMaze - 1) || maze[upNeighbour.x][upNeighbour.y + 1]))
				visitibleNeighbourCells.push_back(upNeighbour);
		}

		if ((downNeighbour.y >= 0) && !isCellVisited[downNeighbour.x][downNeighbour.y])
		{
			if (((downNeighbour.x - 1 < 0) || maze[downNeighbour.x - 1][downNeighbour.y]) && ((downNeighbour.x + 1 > dimentionOfMaze - 1) || maze[downNeighbour.x + 1][downNeighbour.y]) && ((downNeighbour.y - 1 < 0) || maze[downNeighbour.x][downNeighbour.y - 1]))
				visitibleNeighbourCells.push_back(downNeighbour);
		}

		// 2.2.2. Choose one of the unvisited neighbours
		if (visitibleNeighbourCells.size() != 0)
		{
			int randomNeighbourIndex = rand() % visitibleNeighbourCells.size();

			if (currentCell.x - visitibleNeighbourCells.at(randomNeighbourIndex).x == 0)
			{
				if (leftNeighbour.x >= 0)
					isCellVisited[leftNeighbour.x][leftNeighbour.y] = true;
				if (rightNeighbour.x <= dimentionOfMaze - 1)
					isCellVisited[rightNeighbour.x][rightNeighbour.y] = true;
			}
			else
			{
				if (leftNeighbour.y <= dimentionOfMaze - 1)
					isCellVisited[upNeighbour.x][upNeighbour.y] = true;
				if (leftNeighbour.y >= 0)
					isCellVisited[downNeighbour.x][downNeighbour.y] = true;
			}

			// 2.2.3. Remove the wall between the current cell and the chosen cell
			maze[visitibleNeighbourCells.at(randomNeighbourIndex).x][visitibleNeighbourCells.at(randomNeighbourIndex).y] = ROAD;

			// 2.2.4. Mark the chosen cell as visited and push it to the stack
			isCellVisited[visitibleNeighbourCells.at(randomNeighbourIndex).x][visitibleNeighbourCells.at(randomNeighbourIndex).y] = true;
			stackOfCells.push(visitibleNeighbourCells.at(randomNeighbourIndex));
			// Set flag false: this path has end
			isEnd = true;
		}
		// Set exit cell coordinate
		// And pop Cell from stack
		else
		{
			if (isEnd == true)
			{
				// Push end Cell to the vector
				mazeEndCells.push_back(Cell{ currentCell.x,currentCell.y });
				// Set flag false
				isEnd = false;
			}
			// Pop Cell from stack
			stackOfCells.pop();
		}

		visitibleNeighbourCells.clear();

	}
	//// Set maze exit coordinate
	//maze[(*exitCell).x][(*exitCell).y] = EXIT;

	int exitCount = mazeEndCells.size() / 3;
	if (exitCount < 1)
		exitCount = 1;
	int monsterCount = mazeEndCells.size() - exitCount;
	if (monsterCount < 1)
		monsterCount = 0;
	for (int i = 0; i < exitCount; i++)
	{
		int index_x = mazeEndCells[i].x;
		int index_y = mazeEndCells[i].y;

		maze[index_x][index_y] = EXIT;
	}
	for (int i = exitCount; i < (exitCount + monsterCount); i++)
	{
		int index_x = mazeEndCells[i].x;
		int index_y = mazeEndCells[i].y;

		maze[index_x][index_y] = MONSTER;
	}

	// ***MAZE GENERATİON***

	// Assign gold count
	goldCount = (dimentionOfMaze / 2) + 1;
	// Holds created gold count
	int createdGoldCount = 0;

	// While created gold count not equal desired value
	while (createdGoldCount < goldCount)
	{
		// Generate random x coordinate
		int randomGoldIndex_x = rand() % dimentionOfMaze;
		// Generate random y coordinate
		int randomGoldIndex_y = rand() % dimentionOfMaze;
		// If created coordinate is ROAD
		if (maze[randomGoldIndex_x][randomGoldIndex_y] == ROAD)
		{
			// Assign gold
			maze[randomGoldIndex_x][randomGoldIndex_y] = GOLD;
			// Increase created gold count
			createdGoldCount++;
		}
	}

	// Holds created monster count
	int createdMonsterCount = 0;

	// Deallocete memory for will not be used pointers
	//for (int i = 0; i < dimentionOfMaze; i++)
	//{
	//	delete[] isCellVisited[i];
	//}
	delete[] isCellVisited;
}

unsigned char selectCharacter()
{
	// Character informations
	cout << "CHARACTER LIST: " << endl;

	cout << "1)JACK: " << CHARACTER_1 << endl;
	cout << "2)MICHEAL: " << CHARACTER_2 << endl;

	// Selected character index
	int selectedCharacterIndex;
	// Assign selected character index
	cout << "Select your character: ";
	cin >> selectedCharacterIndex;

	// If entered value is not in desired range
	while (selectedCharacterIndex < 1 || selectedCharacterIndex > 2)
	{
		cout << "Character index is not valid!" << endl;
		cout << "Select your character: ";
		cin >> selectedCharacterIndex;
	}

	// Return selected character value
	if (selectedCharacterIndex == 1)
		return CHARACTER_1;
	else
		return CHARACTER_2;
}

bool openLogFile(FILE * &logFile)
{
	// Current date/time based on current system
	time_t now = time(0);

	// Get local time informations
	tm *ltm = localtime(&now);

	// Create file path and name
	std::string fileName = to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_mon + 1) + "-" + to_string(ltm->tm_year + 1900) + " " +
							to_string(ltm->tm_hour) + "." + to_string(ltm->tm_min) + "." + to_string(ltm->tm_sec) + ".txt";

	errno_t err;

	// Open file for write
	err = fopen_s(&logFile, fileName.c_str(), "w+");
	if (err == 0)
	{
		printf("The log file was opened succesfully!\n");
		return true;
	}
	else
	{
		printf("The log file was not opened!\n");
		return false;
	}
}

bool closeLogFile(FILE * &logFile)
{
	errno_t err;

	// Close file if it isn't NULL
	if (logFile)
	{
		err = fclose(logFile);
		if (err == 0)
		{
			return true;
		}
		else
		{
			printf("The log file was not closed!\n");
			return false;
		}
	}
}

void copyPointer2Pointer(unsigned char** &source, unsigned char** &destination, int dimentionOfMaze)
{
	// Set maze as all WALL
	for (int i = 0; i < dimentionOfMaze; i++)
	{
		for (int j = 0; j < dimentionOfMaze; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
}
