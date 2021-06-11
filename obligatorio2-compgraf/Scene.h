#pragma once

struct SDL_Renderer;

class Scene
{
public:
	void loadSceneFromFile();
	void render(SDL_Renderer* renderer, int width, int height);

};

