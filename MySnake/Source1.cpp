//A simple snake game in C++. Eat the food and grow bigger, if you hit a wall or your tail the game ends.
//Controls: WASD, X to exit.

#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>
using namespace std;

bool gameOver;						//The Game Over condition
const int width = 20;				//The Width and Height of the map
const int height = 20;
const char wall = '#';
//x,y are player coordinates, foodX foodY are food coordinates
//t represents the speed of the game and the lower it is, the faster the game progresses
//nTail is the size of our tail (equal to the score variable), tailX and tailY are pairs of coordinates for the tail
int x, y, foodX, foodY, score, t=500, nTail=0, tailX[100], tailY[100];
enum eDirection {STOP = 0, UP, LEFT, DOWN, RIGHT};
eDirection dir;


//SpawnFood spawns the food somewhere randomly on the map
void SpawnFood()
{
	foodX = rand() % (width-1) + 1;
	foodY = rand() % (height-1) + 1;
	for (int i = 0; i < nTail; i++)
	{
		if (foodX == tailX[i] && foodY == tailY[i])	//if the food spawned on our tail, instead we retry another random value with another seed
		{
			srand(time(0));
			SpawnFood();
		}
	}
}
void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	srand(time(0));
	SpawnFood();
	score = 0;
}
void Draw()
{
	int i, j;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });	//Setting the cursor to the upper left corner of the console
	for (i = 0; i < width+2; i++)
		cout <<  wall;
	cout << endl;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			if (j == 0)
				cout << wall;
			else if (i == y && j == x)	
				cout << "O";	//The player's position
			else if (i == foodY && j == foodX)
				cout << "X";	//The food's position
			else
			{
				bool printTail = false;
				for (int k = 0; k < nTail; k++)
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						printTail = true;
					}
				if (!printTail)	cout << " ";
			}
			
		}
		cout << wall;
		cout << endl;
	}
	for (i = 0; i < width+2; i++)
		cout << wall;

	cout << endl << "Score = "<< score;
}
void Input()
{
	if (_kbhit())
	{
		switch (_getch())	//Checking for button hits
		{
		case 'w':
			dir = UP;
			break;
		case 'a':
			dir = LEFT;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'x':
			gameOver= true;
			break;
		}
	}
}
void Logic()
{
	tailX[nTail] = x;
	tailY[nTail] = y;

	switch (dir)
	{
	case UP:
		y--;
		break;
	case LEFT:
		x--;
		break;
	case DOWN:
		y++;
		break;
	case RIGHT:
		x++;
		break;
	}
	if (x == 0 || x == width+1 || y < 0 || y == height)
		gameOver = true;	//Game Over when we go out of bounds
	for (int i = 0; i < nTail; i++)
		if (x == tailX[i] && y == tailY[i])
			gameOver = true;	//Game OVer when we hit the tail
	if (x == foodX && y == foodY)
	{
		nTail++;
		score++;
		t = 9 * t / 10; //Game speeds up by 10%
		SpawnFood();
	}
	for (int i = 0; i < nTail; i++)
	{
		tailX[i] = tailX[i + 1]; //Updating the tail's coordinates
		tailY[i] = tailY[i + 1];
	}
}
void IsGameOver()
{
	if (gameOver)
	{
		cout <<"\n\nGAME OVER!!!\n";
	}
}
int main()
{
	Setup();
	while(!gameOver)
	{
	
		Input();
		Logic();
		Draw();
		Sleep(t);
	}
	IsGameOver();
	return 0;
}