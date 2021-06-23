#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Constants.h"
#include "ImageGenerator.h"
#include "Scene.h"

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

    //SDL_Window *window = SDL_CreateWindow("Iluminacion Global", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Surface* mainSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(mainSurface);

    SDL_Surface* reflectionSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer* reflectionRenderer = SDL_CreateSoftwareRenderer(reflectionSurface);

    SDL_Surface* transmissionSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer* transmissionRenderer = SDL_CreateSoftwareRenderer(transmissionSurface);

    SDL_Surface* ambientSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer* ambientRenderer = SDL_CreateSoftwareRenderer(ambientSurface);

    SDL_Surface* diffuseSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer* diffuseRenderer = SDL_CreateSoftwareRenderer(diffuseSurface);

    SDL_Surface* specularSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_Renderer* specularRenderer = SDL_CreateSoftwareRenderer(specularSurface);

    Scene* scene = new Scene();
    scene->loadSceneFromFile();

    scene->render(renderer, reflectionRenderer, transmissionRenderer, ambientRenderer, diffuseRenderer, specularRenderer);

    ImageGenerator::generateImage(mainSurface, "final", SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageGenerator::generateImage(reflectionSurface, "reflection", SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageGenerator::generateImage(transmissionSurface, "transmission", SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageGenerator::generateImage(ambientSurface, "ambient", SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageGenerator::generateImage(diffuseSurface, "diffuse", SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageGenerator::generateImage(specularSurface, "specular", SCREEN_WIDTH, SCREEN_HEIGHT);

    IMG_Quit();
    SDL_Quit();

    return 0;
}