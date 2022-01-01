#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "main.cpp"

#define SQUARE_SIZE 30

using namespace std;

vector<sf::RectangleShape> grid;
sf::Texture defTexture;
sf::Texture numbers[9];
sf::Texture mine;
sf::Texture flag;

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
}
int drawMatrix(int rows, int cols)
{
    sf::RenderWindow window(sf::VideoMode((rows+2)*SQUARE_SIZE,(cols+2)*SQUARE_SIZE), "Minesweeper");
    if(!defTexture.loadFromFile("images/default.png") || !mine.loadFromFile("images/mine.png") || !flag.loadFromFile("images/flag.png"))
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
            tmp.setPosition(sf::Vector2f((j+1) * SQUARE_SIZE, (i+1) * SQUARE_SIZE));
            
        
            grid.push_back(tmp);
        }
    }
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            //cout<<event.type<<endl;
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if(gameState == GAME_BOOM_STATE)
                    {
                        break;
                    }
                    if(event.key.code == sf::Mouse::Left)
                    {
                        if(event.mouseButton.x > SQUARE_SIZE && event.mouseButton.x < window.getSize().x - SQUARE_SIZE && event.mouseButton.y > SQUARE_SIZE && event.mouseButton.y < window.getSize().y - SQUARE_SIZE)
                        {
                            int r = (event.mouseButton.y / SQUARE_SIZE) - 1;
                            int c = (event.mouseButton.x / SQUARE_SIZE) - 1;
                            cout<<r<<" "<<c<<endl;
                            openCell(r,c);
                            refreshGrid();
                        //cout<<event.mouseButton.x<<endl;
                        }
                    }
                    else if(event.key.code == sf::Mouse::Right)
                    {
                        if(event.mouseButton.x > SQUARE_SIZE && event.mouseButton.x < window.getSize().x - SQUARE_SIZE && event.mouseButton.y > SQUARE_SIZE && event.mouseButton.y < window.getSize().y - SQUARE_SIZE)
                        {
                            int r = (event.mouseButton.y / SQUARE_SIZE) - 1;
                            int c = (event.mouseButton.x / SQUARE_SIZE) - 1;
                            cout<<r<<" "<<c<<endl;
                            if(gameGrid[r][c] != MINE_CELL)
                            {
                                setAsMine(r,c);
                                refreshGrid();
                            }
                            else
                            {
                                removeMine(r,c);
                                refreshGrid();
                            }
                            
                        }
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
        window.display();
    }
    return EXIT_SUCCESS;
}
int main()
{
    generateScheme(10, 10, 10);
    drawMatrix(10,10);

}