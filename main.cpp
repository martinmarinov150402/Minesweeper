#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "functionality.cpp"

#define SQUARE_SIZE 30
#define ROWS 9
#define COLUMNS 9
#define MINES 10

using namespace std;

vector<sf::RectangleShape> grid;
sf::RectangleShape status;
sf::Texture defTexture;
sf::Texture numbers[9];
sf::Texture mine;
sf::Texture flag;
sf::Texture happy,cool,sad;
sf::RectangleShape mCount;
sf::Text mCountText;
sf::RectangleShape timerRect;
sf::Text timerText;
sf::Font arial;
sf::Clock timer;
bool pressed = false;


string intToString(int number)
{
    ostringstream convertToString;
    convertToString.clear();
    convertToString << number;
    return convertToString.str();
}
void updateGameGrid(int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(gameGrid[i][j] != UNOPENED_CELL)
            {
                if(gameGrid[i][j] == '0'+HAS_MINE)
                {
                    grid[i*cols + j].setTexture(&mine);
                }
                else if(gameGrid[i][j] == MINE_CELL)
                {
                    grid[i*cols + j].setTexture(&flag);
                }
                else
                {
                    grid[i*cols + j].setTexture(&numbers[gameGrid[i][j]-'0']);
                }
            }
            else
            {
                grid[i*cols + j].setTexture(&defTexture);
            }
        }
    }
    if(gameState == GAME_WIN_STATE)
    {
        status.setTexture(&cool);
    }
    else if(gameState == GAME_BOOM_STATE)
    {
        status.setTexture(&sad);
    }
    else
    {
        status.setTexture(&happy);
    }
    
    string tmp = intToString(minesCount - puttedMines);
    mCountText.setString(tmp);
    if(gameState == GAME_UNFINISHED_STATE)
    {
        sf::Time time = timer.getElapsedTime();
        string timeString="";
        if(pressed)
        {
            timeString = intToString(time.asSeconds());
        }
        else
        {
            timeString = "0";
        }
        timerText.setString(timeString);
    } 


}
int drawMatrix(int rows, int cols)
{
    sf::RenderWindow window(sf::VideoMode((rows+4)*SQUARE_SIZE,(cols+4)*SQUARE_SIZE), "Minesweeper");
    status.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
    status.setPosition(window.getSize().x / 2 - SQUARE_SIZE / 2, SQUARE_SIZE / 2);
    mCount.setSize(sf::Vector2f(3*SQUARE_SIZE, SQUARE_SIZE));
    mCount.setPosition(SQUARE_SIZE,SQUARE_SIZE/2);
    mCount.setFillColor(sf::Color::White);
    mCountText.setPosition(2.0*SQUARE_SIZE,SQUARE_SIZE/2);
    mCountText.setFillColor(sf::Color::Red);
    timerRect.setSize(sf::Vector2f(3*SQUARE_SIZE, SQUARE_SIZE));
    timerRect.setPosition(window.getSize().x - 4 * SQUARE_SIZE, SQUARE_SIZE / 2);
    timerRect.setFillColor(sf::Color::White);
    timerText.setPosition(window.getSize().x - 3 * SQUARE_SIZE, SQUARE_SIZE / 2);
    timerText.setFillColor(sf::Color::Red);
    timerText.setFont(arial);
    timerText.setCharacterSize(25);
    if(!arial.loadFromFile("fonts/arial.ttf"))
    {
        cout << "Font load failed" << endl;
    }
    mCountText.setFont(arial);
    mCountText.setCharacterSize(25);

    
    
    if(!defTexture.loadFromFile("images/default.png") || !mine.loadFromFile("images/mine.png") || !flag.loadFromFile("images/flag.png") || !happy.loadFromFile("images/happy.png") || !cool.loadFromFile("images/cool.jpeg") || !sad.loadFromFile("images/sad.jpeg"))
    {
        cout << "Texture load failed" <<endl;
        
    }
    for(int i = 0; i <= 8; i++)
    {
        char tmp = i + '0';
        string a = "images/";
        a.push_back(tmp);
        a+=".png";
        if(!numbers[i].loadFromFile(a))
        {
            cout << "Texture load failed" <<endl;
        }
    }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            sf::RectangleShape tmp;
            tmp.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
            tmp.setPosition(sf::Vector2f((j+2) * SQUARE_SIZE, (i+2) * SQUARE_SIZE));
            
        
            grid.push_back(tmp);
        }
    }
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if(event.key.code == sf::Mouse::Left && event.mouseButton.x > window.getSize().x / 2 - SQUARE_SIZE / 2 && event.mouseButton.x < window.getSize().x / 2 + SQUARE_SIZE / 2 && event.mouseButton.y > SQUARE_SIZE / 2 && event.mouseButton.y < (3*SQUARE_SIZE/2))
                    {
                        pressed = false;
                        generateScheme(ROWS,COLUMNS,MINES);
                        break;
                    }
                    if(gameState == GAME_BOOM_STATE || gameState == GAME_WIN_STATE)
                    {
                        break;
                    }
                    if(event.key.code == sf::Mouse::Left)
                    {
                        
                        if(event.mouseButton.x > 2*SQUARE_SIZE && event.mouseButton.x < window.getSize().x - 2*SQUARE_SIZE && event.mouseButton.y > 2*SQUARE_SIZE && event.mouseButton.y < window.getSize().y - 2*SQUARE_SIZE)
                        {
                            if(!pressed)
                            {
                                timer.restart();
                                pressed = true;
                            }
                            int r = (event.mouseButton.y / SQUARE_SIZE) - 2;
                            int c = (event.mouseButton.x / SQUARE_SIZE) - 2;
                            if(gameGrid[r][c] == UNOPENED_CELL)
                            {
                                openCell(r,c);
                            }
                            else
                            {
                                openIfCompleted(r,c);
                            }
                        }
                        
                        //status.setPosition(window.getSize().x / 2 - SQUARE_SIZE / 2, SQUARE_SIZE / 2);
                    }
                    else if(event.key.code == sf::Mouse::Right)
                    {
                        if(event.mouseButton.x > 2*SQUARE_SIZE && event.mouseButton.x < window.getSize().x - 2*SQUARE_SIZE && event.mouseButton.y > 2*SQUARE_SIZE && event.mouseButton.y < window.getSize().y - 2*SQUARE_SIZE)
                        {
                            if(!pressed)
                            {
                                timer.restart();
                                pressed = true;
                            }
                            int r = (event.mouseButton.y / SQUARE_SIZE) - 2;
                            int c = (event.mouseButton.x / SQUARE_SIZE) - 2;
                            if(gameGrid[r][c] != MINE_CELL)
                            {
                                setAsMine(r,c);
                            }
                            else
                            {
                                removeMine(r,c);
                            }
                            
                        }
                    }
                    if(setted == schemeRows * schemeColumns)
                    {
                        gameState = GAME_WIN_STATE;
                    }
                    break;
                }
            }
            
        }
        window.clear();
        
        updateGameGrid(rows,cols);
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                //grid[i * cols + j].setTexture(&defTexture);
                window.draw(grid[i * cols + j]);
            }
        }
        window.draw(status);
        window.draw(mCount);
        window.draw(mCountText);
        window.draw(timerRect);
        window.draw(timerText);
        window.display();
    }
    return EXIT_SUCCESS;
}
int main()
{
    generateScheme(ROWS, COLUMNS, MINES);
    drawMatrix(ROWS, COLUMNS);

}