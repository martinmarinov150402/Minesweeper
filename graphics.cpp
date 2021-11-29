#include <iostream>
#include <SDL2/SDL.h>

#define SQUARE_SIZE 50

using namespace std;

void drawMatrix(int n, int m)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          0);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        return;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return;
    }
    SDL_UpdateWindowSurface(window);
    int curX = 0, curY = 0;

    

    // Set render color to red ( background will be rendered in this color )
   

    
    for(int i = 0; i < n; i++)
    {
        curX=0;
        for(int j=0;j < m;j++)
        {
            SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
             SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

            // Clear winow
            SDL_RenderClear( renderer );

            // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
            

            // Set render color to blue ( rect will be rendered in this color )
            SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
            SDL_Rect r;
            r.x = curX;
            r.y = curY;
            r.w = SQUARE_SIZE;
            r.h = SQUARE_SIZE;
            SDL_RenderFillRect( renderer, &r );

            SDL_RenderPresent(renderer);

            curX+=SQUARE_SIZE;
        }
        curY+=SQUARE_SIZE;
    }
}