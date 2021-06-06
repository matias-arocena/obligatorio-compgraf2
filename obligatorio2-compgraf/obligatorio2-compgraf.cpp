#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "ImageGenerator.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Error inicializando SDL Image: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Iluminacion Global", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_GetRenderer(window);
    
    ImageGenerator::generateImage(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    IMG_Quit();
    SDL_Quit();

    return 0;
}