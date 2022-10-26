#include <iostream>
//using std::cout;
//using std::cin;
//using std::endl;
//using std::vector;
using namespace std;

#include <vector>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#define WIDTH 40
#define HEIGHT 20
#define BODY 'o'
#define APPLE '*'

enum class Direction
{
	up, right, down, left
};

/// <summary>
/// Each point is a part of the snake
/// </summary>
struct Point
{
	int x, y;
};

#pragma region GlobalVariable
/// <summary>
/// Create snake with 5 parts of its body
/// </summary>
vector<Point> snake = {
	Point{ WIDTH / 2 + 2, HEIGHT / 2 },
	Point{ WIDTH / 2 + 1, HEIGHT / 2 },
	Point{ WIDTH / 2, HEIGHT / 2 },
	Point{ WIDTH / 2 - 1, HEIGHT / 2 },
	Point{ WIDTH / 2 - 2, HEIGHT / 2 }
};
Direction direction = Direction::right;
Point apple;
int score = 0;
int speed = 300;
Point prevTail;
#pragma endregion GlobalVariable

#pragma region Prototype
void drawSnakePart(Point);
void drawSnake();
void gotoxy(int, int);
void ShowConsoleCursor(bool);
void move();
void drawBox();
bool isHitWall();
bool isBiteItself();
void drawHeadnTail();
void generateApple();
bool isAteApple();
void growing();
void displayScore();
void showEndMenu();
void startGame();
void resetSnake();
void showStartMenu();
#pragma endregion Prototype

int main()
{
	showStartMenu();
	return 0;
}

#pragma region GameFunction
/// <summary>
/// Draw snakeboard with WIDTH and HEIGHT was set
/// </summary>
void drawBox()
{
	for (size_t i = 0; i < WIDTH; i++)
		cout << '=';
	gotoxy(0, HEIGHT);
	for (size_t i = 0; i < WIDTH; i++)
		cout << '=';
	for (size_t i = 1; i < HEIGHT; i++)
	{
		gotoxy(0, i);
		cout << '|';
	}
	for (size_t i = 1; i < HEIGHT; i++)
	{
		gotoxy(WIDTH, i);
		cout << '|';
	}
}

/// <summary>
/// Check if the snake hit the wall
/// </summary>
/// <returns>true if the snake hits the wall, otherwise false</returns>
bool isHitWall()
{
	return snake[0].x == 0 || snake[0].y == 0 || snake[0].x == WIDTH || snake[0].y == HEIGHT;
}

/// <summary>
/// Generate apple on the board randomly
/// </summary>
void generateApple()
{
	srand(time(0));
	int x = rand() % (WIDTH - 1) + 1;
	int y = rand() % (HEIGHT - 1) + 1;
	apple = { x, y };
	gotoxy(x, y);
	cout << APPLE;
}

/// <summary>
/// Check if the snake ate apple
/// </summary>
/// <returns>true if the apple is eaten, otherwise false</returns>
bool isAteApple()
{
	return snake[0].x == apple.x && snake[0].y == apple.y;
}

/// <summary>
/// Show score on right side of the board
/// </summary>
void displayScore()
{
	gotoxy(WIDTH + 5, 2);
	cout << "Your score: " << score;
}

/// <summary>
/// Show menu at the end of the game
/// </summary>
void showEndMenu()
{
	system("cls");
	gotoxy(0, 0);
	cout << "End game!" << endl;
	cout << "Your score: " << score << endl;
	cout << "Do you want to play again ([y]/[n]): ";
	char option;
	cin >> option;
	option = tolower(option);
	if (option == 'y')
	{
		resetSnake();
		startGame();
	}
	else if (option == 'n')
		exit(1);
}

void startGame()
{
	system("cls");
	ShowConsoleCursor(false);

	drawBox();
	drawSnake();
	generateApple();
	displayScore();

	while (true)
	{
		if (_kbhit()) // _kbhit() returns a nonzero value if a key has been pressed. Otherwise, it returns 0.
		{
			char ch = _getch();
			ch = tolower(ch);
			if (ch == 'a' && direction != Direction::right)
				direction = Direction::left;
			else if (ch == 'w' && direction != Direction::down)
				direction = Direction::up;
			else if (ch == 's' && direction != Direction::up)
				direction = Direction::down;
			else if (ch == 'd' && direction != Direction::left)
				direction = Direction::right;
			else if (ch == 'q') // Quit game
			{
				showEndMenu();
				break;
			}
		}
		move();
		drawHeadnTail();
		if (isAteApple())
		{
			score++;
			displayScore();
			growing();
			generateApple();
		}
		if (isBiteItself())
		{
			ShowConsoleCursor(true);
			showEndMenu();
			break;
		}
		if (isHitWall())
		{
			ShowConsoleCursor(true);
			showEndMenu();
			break;
		}
		Sleep(speed);
	}
}

void resetSnake()
{
	score = 0;
	direction = Direction::right;
	snake = {
		Point{ WIDTH / 2 + 2, HEIGHT / 2 },
		Point{ WIDTH / 2 + 1, HEIGHT / 2 },
		Point{ WIDTH / 2, HEIGHT / 2 },
		Point{ WIDTH / 2 - 1, HEIGHT / 2 },
		Point{ WIDTH / 2 - 2, HEIGHT / 2 }
	};
}

/// <summary>
/// Show start menu at the begining of the game
/// </summary>
void showStartMenu()
{
	system("cls");
	cout << "Welcome to snake game!" << endl;
	cout << "Options:" << endl;
	cout << "1. Start" << endl;
	cout << "2. Quit" << endl;
	cout << "Your choice: ";
	int option;
	cin >> option;
	if (option == 1)
	{
		system("cls");
		cout << "Choose your level (1 - 5): ";
		int t;
		cin >> t;
		speed = 600 - t * 100; // Calculate speed depend on level
		system("cls");
		cout << "Tip: While playing game, you can press 'q' to quit";
		gotoxy(0, 3);
		cout << "Ready!";
		Sleep(1000);
		for (size_t i = 3; i > 0; i--)
		{
			gotoxy(0, 3);
			cout << i << "         ";
			Sleep(1000);
		}
		gotoxy(0, 3);
		cout << "GO!";
		Sleep(1000);
		startGame();
	}
	else if (option == 2)
		exit(1);
}
#pragma endregion GameFunction


#pragma region SnakeFunction
/// <summary>
/// Draw a part of snake
/// </summary>
/// <param name="p">is coordinate of a snake's part</param>
void drawSnakePart(Point p)
{
	gotoxy(p.x, p.y);
	cout << BODY;
}

/// <summary>
/// Draw whole snake
/// </summary>
void drawSnake()
{
	for (size_t i = 0; i < snake.size(); i++)
		drawSnakePart(snake[i]);
}

/// <summary>
/// Move the snake
/// </summary>
void move()
{
	prevTail = snake.back();
	for (size_t i = snake.size() - 1; i > 0; i--)
		snake[i] = snake[i - 1];
	if (direction == Direction::up)
		snake[0].y -= 1;
	else if (direction == Direction::down)
		snake[0].y += 1;
	else if (direction == Direction::left)
		snake[0].x -= 1;
	else if (direction == Direction::right)
		snake[0].x += 1;
}

/// <summary>
/// Redraw head & tail to make the snake move
/// </summary>
void drawHeadnTail()
{
	gotoxy(snake[0].x, snake[0].y);
	cout << BODY;
	gotoxy(prevTail.x, prevTail.y);
	cout << ' '; // Clear the old tail
}

/// <summary>
/// Check if snake bite itself
/// </summary>
/// <returns>true if snake bite itself, otherwise false</returns>
bool isBiteItself()
{
	Point head = snake[0];
	for (size_t i = 1; i < snake.size(); i++)
		if (head.x == snake[i].x && head.y == snake[i].y)
			return true;
	return false;
}

/// <summary>
/// Growing snake when it ate an apple
/// </summary>
void growing()
{
	snake.push_back(prevTail);
}
#pragma endregion SnakeFunction

#pragma region ConsoleFunction
/// <summary>
/// Goto position (x, y)
/// </summary>
/// <param name="x">is horizontal axis coordinate</param>
/// <param name="y">is vertical axis coordinate</param>
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/// <summary>
/// Set the visibility of cursor
/// </summary>
/// <param name="showFlag"></param>
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}
#pragma endregion ConsoleFunction