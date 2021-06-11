#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Surface;

class ImageGenerator
{
public:
	static void generateImage(SDL_Renderer* renderer, std::string imageName, int width, int height);
private:
	static const int RED_MASK = 0x00ff0000;
	static const int GREEN_MASK = 0x0000ff00;
	static const int BLUE_MASK = 0x000000ff;
	static const int ALPHA_MASK = 0xff000000;

	static std::string generateName();
	static void saveImage(SDL_Surface* surface, const char* imageName);
};

