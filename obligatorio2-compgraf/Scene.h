#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "Constants.h"
#include "Color.h"
#include "Light.h"
#include "PositionLight.h"
#include "SceneObject.h"

class Ray;

struct SDL_Renderer;

class Scene
{
public:
	void loadSceneFromFile();
	void render(SDL_Renderer* renderer);
private:
	int maxDepth;
	glm::vec3 projectionCenter;
	std::vector<SceneObject*> objects;
	std::vector<PositionLight*> lights;
	Light ambient;
	Color backgroudColor;

	Color rayTrace(const Ray& ray, int depth);
	Color shadow(CollisionPoint *hit, const Ray& ray, int depth);
	
	CollisionPoint* getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions);
	Color getAmbientColor(const SceneObject& object);
	Ray getReflectiveRay(const Ray& ray, float reflectionCoefficient);
	Ray getTransmissionRay(const Ray& ray, float transmissionCoefficient);
};

