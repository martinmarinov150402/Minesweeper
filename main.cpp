#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "functionality.cpp"

#define SQUARE_SIZE 30

using namespace std;

vector<sf::RectangleShape> grid;
sf::RectangleShape status;
sf::RectangleShape l1,l2,l3;
sf::Texture defTexture;
sf::Texture numbers[9];
sf::Texture mine;
sf::Texture flag;
sf::Texture happy,cool,sad;
sf::Texture level1, level2, level3;
sf::RectangleShape mCount;
sf::Text mCountText;
sf::RectangleShape timerRect;
sf::Text timerText;
sf::Font arial;
sf::Clock timer;
bool pressed = false;
int ROWS = 16;
int COLUMNS = 16;
int MINES = 40;
bool closed = false;


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
    grid.clear();
    sf::RenderWindow window(sf::VideoMode((cols+4)*SQUARE_SIZE,(rows+4)*SQUARE_SIZE), "Minesweeper");
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
    l1.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
    l2.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
    l3.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
    l1.setPosition(1.5 * SQUARE_SIZE , window.getSize().y - 1.5 * SQUARE_SIZE);
    l2.setPosition(window.getSize().x/2 - SQUARE_SIZE/2, window.getSize().y - 1.5 * SQUARE_SIZE);
    l3.setPosition(window.getSize().x - 2.5 * SQUARE_SIZE, window.getSize().y - 1.5 * SQUARE_SIZE);
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
    if(!level1.loadFromFile("images/button-level_1.png") || !level2.loadFromFile("images/button-level_2.png") || !level3.loadFromFile("images/button-level_3.jpg"))
    {
        cout << "Texture load failed" << endl;
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
                    closed = true;
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
                        /*l1.setPosition(1.5 * SQUARE_SIZE , window.getSize().y - 1.5 * SQUARE_SIZE);
                        l2.setPosition(window.getSize().x/2 - SQUARE_SIZE/2, window.getSize().y - 1.5 * SQUARE_SIZE);
                        l3.setPosition(window.getSize().x - 2.5 * SQUARE_SIZE, window.getSize().y - 1.5 * SQUARE_SIZE);*/
                        if(event.mouseButton.y > (window.getSize().y - 1.5 * SQUARE_SIZE) && event.mouseButton.y < (window.getSize().y - 0.5 * SQUARE_SIZE))
                        {
                            if(event.mouseButton.x > 1.5 * SQUARE_SIZE && event.mouseButton.x < 2.5 * SQUARE_SIZE)
                            {
                                pressed = false;
                                ROWS = 9;
                                COLUMNS = 9;
                                MINES = 10;
                                window.close();
                                //drawMatrix(ROWS, COLUMNS);
                                return 0;
                            }
                            if(event.mouseButton.x > window.getSize().x/2 - 0.5 * SQUARE_SIZE && event.mouseButton.x < window.getSize().x/2 + 0.5 * SQUARE_SIZE)
                            {
                                pressed = false;
                                ROWS = 16;
                                COLUMNS = 16;
                                MINES = 40;
                                window.close();
                                return 0;
                                //drawMatrix(ROWS, COLUMNS);
                            }
                            if(event.mouseButton.x > window.getSize().x - 2.5 * SQUARE_SIZE && event.mouseButton.x < window.getSize().x - 1.5 * SQUARE_SIZE)
                            {
                                pressed = false;
                                ROWS = 16;
                                COLUMNS = 30;
                                MINES = 99;
                                window.close();
                                return 0;
                                //drawMatrix(ROWS, COLUMNS);
                            }

                        }
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
        l1.setTexture(&level1);
        l2.setTexture(&level2);
        l3.setTexture(&level3);
        window.draw(status);
        window.draw(mCount);
        window.draw(mCountText);
        window.draw(timerRect);
        window.draw(timerText);
        window.draw(l1);
        window.draw(l2);
        window.draw(l3);
        window.display();
    }
    return EXIT_SUCCESS;
}
int main()
{
    while(!closed)
    {
        generateScheme(ROWS, COLUMNS, MINES);
        drawMatrix(ROWS, COLUMNS);
    }

}