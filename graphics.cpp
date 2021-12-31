#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "main.cpp"

#define SQUARE_SIZE 30

using namespace std;

vector<sf::RectangleShape> grid;
sf::Texture defTexture;
void updateGameGrid(int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(gameGrid[i][j] == UNOPENED_CELL)
            {
                
            }
        }
    }
}
int drawMatrix(int rows, int cols)
{
    sf::RenderWindow window(sf::VideoMode((rows+2)*SQUARE_SIZE,(cols+2)*SQUARE_SIZE), "Minesweeper");
    if(!defTexture.loadFromFile("images/default.png"))
    {
        cout << "Texture load failed" <<endl;
        
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
                }
            }
            
        }
        window.clear();
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i * cols + j].setTexture(&defTexture);
                window.draw(grid[i * cols + j]);
            }
        }
        window.display();
    }
    return EXIT_SUCCESS;
}
int main()
{
    generateScheme(10, 10, 7);
    drawMatrix(10,10);

}