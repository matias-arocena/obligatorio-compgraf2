#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Surface;

class ImageGenerator
{
public:
	static void generateImage(SDL_Surface* surface, std::string imageName, int width, int height);
private:
	static std::string generateName();
	static void saveImage(SDL_Surface* surface, const char* imageName);
};

