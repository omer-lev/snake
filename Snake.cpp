#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

const int WIDTH = 40, HEIGHT = 30;
string map[HEIGHT][WIDTH];

vector<int> snakeX, snakeY;
int fruitX, fruitY;

int score=0;
bool running;


enum eDir { STOP, LEFT, RIGHT, UP, DOWN };


void SetupMap()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0)
				map[i][j] = "#";

			else if (i == HEIGHT - 1)
			{
				if (j == 0)
					map[i][j] = "\n#";
				else
					map[i][j] = "#";
			}
			else if (j == 0 && i <= WIDTH - 2)
				map[i][j] = "\n#";

			else if (j == WIDTH - 1)
				map[i][j] = "#";

			else
				map[i][j] = " ";


			// fruit
			if (j == fruitX && i == fruitY)
				map[i][j] = "G";
		}
	}
}

void ClearScreen()
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void Draw(int rows, int cols, vector<int> snakeX, vector<int> snakeY)
{
	SetupMap();
	ClearScreen();

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// snake body
			for (int z = snakeX.size() - 1; z >= 0; z--)
			{
				if (j == snakeX[z] && i == snakeY[z])
				{
					if (z == 0)
						map[i][j] = "O";
					else 
						map[i][j] = "o";
				}
			}


			cout << map[i][j];
		}
	}

	cout << "\nSCORE: " << score;
}

void SpawnFruit()
{
	vector<int> freeX, freeY;

	for (int i = 1; i < HEIGHT - 1; i++)
	{
		for (int j = 1; j < WIDTH - 1; j++)
		{
			if (map[i][j] != "o" && map[i][j] != "O")
			{
				freeX.push_back(j);
				freeY.push_back(i);
			}
		}
	}

	int idx = rand() % (freeX.size() - 1);

	fruitX = freeX[idx];
	fruitY = freeY[idx];
}


int main()
{
	snakeX = { 15, 14, 13, 12 }, snakeY = { 3, 3, 3, 3 };
	int prevPosX, prevPosY;


	eDir dir;
	dir = STOP;

	srand(time(NULL));
	SpawnFruit();

	running = true;

	while (running)
	{
		prevPosX = snakeX[0], prevPosY = snakeY[0];

		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
				if (dir != DOWN) 
					dir = UP;

				break;

			case 's':
				if (dir != UP)
					dir = DOWN;
				break;

			case 'a':
				if (dir != RIGHT && dir != STOP)
					dir = LEFT;
				break;

			case 'd':
				if (dir != LEFT)
					dir = RIGHT;
				break;

			default:
				break;
			}
		}


		switch (dir)
		{
			case UP:
				snakeY[0]--;
				break;

			case DOWN:
				snakeY[0]++;
				break;

			case LEFT:
				snakeX[0]--;
				break;

			case RIGHT:
				snakeX[0]++;
				break;

			default:
				break;
		}

		if (dir != STOP)
		{
			for (int i = 1; i < snakeX.size(); i++)
			{
				int x = snakeX[i], y = snakeY[i];

				snakeX[i] = prevPosX;
				snakeY[i] = prevPosY;

				prevPosX = x;
				prevPosY = y;

				if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0])
					running = false;
			}
		}

		// hit wall
		if (snakeX[0] <= 0 || snakeX[0] >= WIDTH || snakeY[0] <= 0 || snakeY[0] == HEIGHT - 1)
			break;

		// hit fruit
		if (snakeX[0] == fruitX && snakeY[0] == fruitY)
		{
			SpawnFruit();

			snakeX.push_back(prevPosX);
			snakeY.push_back(prevPosY);

			score++;
		}

		
		Draw(HEIGHT, WIDTH, snakeX, snakeY);

		Sleep(50); // speed (higher - slower)
	}

	return 0;
}