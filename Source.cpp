#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
const int width = 30, height = 30;
int point = 0, flag = 0, level = 0, TIME = 100;
char screen[1000][1000];
enum sMove { UP, LEFT, DOWN, RIGHT };
//------------------------------------------------------
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
//------------------------------------------------------
struct toaDo {
	int x, y;
};
struct Snake {
	toaDo body[100];
	int n;
	sMove move;
};
struct Food {
	toaDo td;
};
void new_Snake(Snake& snake, Food &food)
{
	snake.n = 2;
	snake.body[0].x = width/2;
	snake.body[0].y = height/2;
	snake.body[1].x = width / 2;
	snake.body[1].y = height / 2-1;
	snake.move = RIGHT;
	food.td.x = rand() % width;
	food.td.y = rand() % height;
}
void draw(Snake snake, Food food)
{
	Nocursortype();
	//Vẽ cái khung (Gán giá trị)
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
				screen[i][j] = 176;
			else
				screen[i][j] = 32;
			if (j == width && i != height - 1)
				screen[i][j] = '\n';
			else if (i == height - 1 && j == width)
				screen[i][j] = ' ';
		}
	}
	screen[food.td.y][food.td.x] = 'A';
	screen[snake.body[0].y][snake.body[0].x] = 'O';
	for (int i = 1; i < snake.n; i++)
	{
		screen[snake.body[i].y][snake.body[i].x] = '-';
	}
	//Vẽ giá trị
	gotoxy(0, 0);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			cout << screen[i][j];
		}

	}
	if (flag == 2)
	{
		level++;
		flag = 0;
		if (TIME >= 20)
			TIME -= 10;
	}
	gotoxy(width, height / 2 + 1);
	cout << "Point: " << snake.n - 1;
	gotoxy(width, height / 2 + 2);
	if (TIME >= 15)
		cout << "Level: " << level;
	else
		cout << "Level MAX!";
	
}
void moving(Snake &snake)
{
	for (int i = snake.n - 1; i > 0; i--)
		snake.body[i] = snake.body[i - 1];
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w': snake.move = UP;
			break;
		case 'a': snake.move = LEFT;
			break;
		case 's': snake.move = DOWN;
			break;
		case 'd': snake.move = RIGHT;
			break;
		}
	}
	switch (snake.move)
	{
	case UP: snake.body[0].y--;
		break;
	case LEFT: snake.body[0].x--;
		break;
	case DOWN: snake.body[0].y++;
		break;
	case RIGHT: snake.body[0].x++;
		break;
	}
}
void gameLogic(Snake &snake, Food &food)
{
	if (snake.body[0].x == food.td.x && snake.body[0].y == food.td.y)
	{
		//Gắn đuôi.
		for (int i = snake.n; i > snake.n - 1; i--)
		{
			snake.body[i] = snake.body[i - 1];
		}
		snake.n++;
		//CHuyển thức ăn
		food.td.x = 1 + rand() % (width - 2);
		food.td.y = 1 + rand() % (height - 2);
		//điểm
		flag++;
	}
}
bool endGame(Snake snake)
{
	for (int i = 1; i < snake.n; i++)
	{
		if (snake.body[i].x == snake.body[0].x && snake.body[i].y == snake.body[0].y)
			return false;
		if (snake.body[0].x == width - 1 || snake.body[0].y == height - 1 || snake.body[0].x == 0 || snake.body[0].y == 0)
			return false;
	}
	return true;
}
int main()
{
	Snake snake;
	Food food;
	new_Snake(snake,food);
	while (1)
	{
		//Hiển thị
		draw(snake, food);

		//Điều khiển
		moving(snake);

		//Xử lí
		gameLogic(snake, food);
		//End game
		if (endGame(snake) == false)
		{
			gotoxy(width, height / 2);
			cout << "YOU LOSE! Press Enter to Exit";
			while (_getch() != 13);
			break;
		}
		//sleep
		Sleep(TIME);
	}
	return 0;
}