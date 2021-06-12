#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "Constants.h"
#include "Color.h"
#include "SceneObject.h"

class Ray;

struct CollisionPoint;
struct SDL_Renderer;

class Scene
{
public:
	void loadSceneFromFile();
	void render(SDL_Renderer* renderer);
private:
	glm::vec3 projectionCenter;
	std::vector<SceneObject*> objects;
	Color backgroudColor;

	Color rayTrace(const Ray& ray, int depth);
	Color shadow(CollisionPoint *hit, const Ray& ray, int depth);

	CollisionPoint* getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions);
};

