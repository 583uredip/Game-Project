#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point {
    int xCoord;
    int yCoord;
    Point(int x = 0, int y = 0) : xCoord(x), yCoord(y) {}
};

class Snake
{
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) : length(1), direction(DIR_RIGHT) { body[0] = Point(x, y); }

    int getLength() { return length; }

    void changeDirection(char newDirection)
    {
        if ((newDirection == DIR_UP && direction != DIR_DOWN) ||
            (newDirection == DIR_DOWN && direction != DIR_UP) ||
            (newDirection == DIR_LEFT && direction != DIR_RIGHT) ||
            (newDirection == DIR_RIGHT && direction != DIR_LEFT))
        {
            direction = newDirection;
        }
    }

    bool move(Point food)
    {
        for (int i = length - 1; i > 0; i--)
        {
            body[i] = body[i - 1];
        }
        switch (direction)
        {
        case DIR_UP: body[0].yCoord--; break;
        case DIR_DOWN: body[0].yCoord++; break;
        case DIR_RIGHT: body[0].xCoord++; break;
        case DIR_LEFT: body[0].xCoord--; break;
        }

        if (body[0].xCoord < 0 || body[0].xCoord >= consoleWidth || 
            body[0].yCoord < 0 || body[0].yCoord >= consoleHeight)
        {
            return false;
        }

        for (int i = 1; i < length; i++)
        {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
            {
                return false;
            }
        }

        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord)
        {
            body[length++] = Point(body[length - 1].xCoord, body[length - 1].yCoord);
            return true;
        }
        return true;
    }
};

class Board
{
    Snake* snake;
    const char SNAKE_BODY = 'O';
    const char FOOD = 'o';
    Point food;
    int score;
public:
    Board() : score(0) {
        spawnFood();
        snake = new Snake(10, 10);
    }
    ~Board() { delete snake; }
    int getScore() { return score; }
    void spawnFood() { food = Point(rand() % (consoleWidth - 1), rand() % (consoleHeight - 1)); }
    void displayCurrentScore() {
        gotoxy(consoleWidth / 2, 0);
        cout << "Current Score: " << score;
    }
    void gotoxy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    void draw() {
        system("cls");
        displayCurrentScore();
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;
        gotoxy(0, 0);
    }
    bool update() {
        bool isAlive = snake->move(food);
        if (!isAlive) return false;
        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();
        }
        return true;
    }
    void getInput() {
        if (GetAsyncKeyState(VK_UP) & 0x8000) snake->changeDirection(DIR_UP);
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) snake->changeDirection(DIR_DOWN);
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) snake->changeDirection(DIR_LEFT);
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) snake->changeDirection(DIR_RIGHT);
    }
};

int main()
{
    srand(time(0));
    initScreen();
    Board* board = new Board();
    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(100);
    }
    system("cls");
    cout << "Game Over" << endl;
    cout << "Final score is: " << board->getScore() << endl;
    delete board;
    return 0;
}
