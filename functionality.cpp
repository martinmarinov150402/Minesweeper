#include <iostream>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <stdlib.h>

using namespace std;

#define MAX_SCHEME_SIZE 200
#define HAS_MINE -1
#define UNINITIALIZED -2
#define UNOPENED_CELL ' '
#define MINE_CELL '*'

#define GAME_UNFINISHED_STATE 0
#define GAME_WIN_STATE 1
#define GAME_BOOM_STATE -1

#define CMD_OPEN 100
#define CMD_SETMINE 101
#define CMD_UNSETMINE 102
#define CMD_COMPLETED_OPEN 103

int scheme[MAX_SCHEME_SIZE][MAX_SCHEME_SIZE];

char gameGrid[MAX_SCHEME_SIZE][MAX_SCHEME_SIZE];

int gameState;

int setted=0;
int puttedMines=0;
int schemeRows, schemeColumns, minesCount;

int countSurroundingMines(int x, int y)
{
    int result = 0;
    if(x > 0 && gameGrid[x-1][y] == '*') result++; 
    if(x < schemeRows-1 && gameGrid[x+1][y] == '*') result++; 
    if(y > 0 && gameGrid[x][y-1] == '*') result++; 
    if(y < schemeColumns-1 && gameGrid[x][y+1] == '*') result++;
    if(x > 0 && y > 0 && gameGrid[x-1][y-1] == '*') result++;
    if(x > 0 && y < schemeColumns-1 && gameGrid[x-1][y+1] == '*') result++;
    if(x < schemeRows-1 && y > 0 && gameGrid[x+1][y-1] == '*') result++;
    if(x < schemeRows-1 && y < schemeColumns-1 && gameGrid[x+1][y+1] == '*') result++;
    return result;
}
void generateScheme(int rows, int columns, int mines)
{
    srand(time(NULL));
    schemeRows = rows;
    minesCount = mines;
    setted = 0;
    puttedMines = 0;
    schemeColumns = columns;
    gameState = GAME_UNFINISHED_STATE;
    int mineScheme[MAX_SCHEME_SIZE][MAX_SCHEME_SIZE];
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            scheme[i][j] = UNINITIALIZED;
            gameGrid[i][j] = UNOPENED_CELL;
            mineScheme[i][j] = 0;
        }
    }
    for(int i = 0; i < mines; i++)
    {
        int mineX = 0, mineY = 0;
        do
        {
            mineX = rand() % rows;
            mineY = rand() % columns;
        }
        while(scheme[mineX][mineY] == HAS_MINE);
        scheme[mineX][mineY] = HAS_MINE;
        mineScheme[mineX][mineY] = 1;
    }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(scheme[i][j] == UNINITIALIZED)
            {
                scheme[i][j] = 0;
                if(i > 0) scheme[i][j] += mineScheme[i-1][j];
                if(i < rows-1) scheme[i][j] += mineScheme[i+1][j];
                if(j > 0) scheme[i][j] += mineScheme[i][j-1];
                if(j < columns-1) scheme[i][j] += mineScheme[i][j+1];
                if(i > 0 && j > 0) scheme[i][j] += mineScheme[i-1][j-1];
                if(i > 0 && j < columns-1) scheme[i][j] += mineScheme[i-1][j+1];
                if(i < rows-1 && j > 0) scheme[i][j] += mineScheme[i+1][j-1];
                if(i < rows-1 && j < columns-1) scheme[i][j] += mineScheme[i+1][j+1];
            }
        }
    }
}
void removeMine(int x, int y)
{
    if(x < 0 || x >= schemeRows || y < 0 || y >= schemeColumns)
    {
        return;
    }
    if(gameGrid[x][y] != MINE_CELL)
    {
        return;
    }
    gameGrid[x][y] = UNOPENED_CELL;
    setted--;
    puttedMines--;
}
void setAsMine(int x, int y)
{
    if(x < 0 || x >= schemeRows || y < 0 || y >= schemeColumns)
    {
        return;
    }
    if(gameGrid[x][y] != UNOPENED_CELL)
    {
        return;
    }
    gameGrid[x][y] = MINE_CELL;
    setted++;
    puttedMines++;
}
void openCell(int x, int y)
{
    if(x < 0 || x >= schemeRows || y < 0 || y >= schemeColumns)
    {
        return;
    }
    if(gameGrid[x][y] != UNOPENED_CELL)
    {
        return;
    }
    setted++;
    gameGrid[x][y] = '0' + scheme[x][y];
    if(scheme[x][y] == 0)
    {
        openCell(x-1, y);
        openCell(x+1, y);
        openCell(x, y-1);
        openCell(x, y+1);
        openCell(x+1, y+1);
        openCell(x-1, y+1);
        openCell(x+1, y-1);
        openCell(x-1, y-1);
    }
    if(scheme[x][y] == HAS_MINE)
    {
        gameState = GAME_BOOM_STATE;
    }
}
void openIfCompleted(int x,int y)
{
    if(countSurroundingMines(x,y) == scheme[x][y] && gameGrid[x][y] != UNOPENED_CELL)
    {
        openCell(x-1, y);
        openCell(x+1, y);
        openCell(x, y-1);
        openCell(x, y+1);
        openCell(x+1, y+1);
        openCell(x-1, y+1);
        openCell(x+1, y-1);
        openCell(x-1, y-1);
    }
}