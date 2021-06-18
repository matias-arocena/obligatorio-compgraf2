#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "Camera.h"
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
	Scene();
	void loadSceneFromFile();
	void render(SDL_Renderer* renderer);
private:
	int maxDepth;
	glm::vec3 projectionCenter;
	std::vector<SceneObject*> objects;
	std::vector<PositionLight*> lights;
	Camera *camera;
	Light *ambientLight;
	Color backgroudColor;

	Color rayTrace(const Ray& ray, int depth);
	Color shadow(const CollisionPoint *hit, const Ray& ray, int depth);
	
	CollisionPoint* getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions);
	Ray getReflectiveRay(const Ray& ray, const CollisionPoint& hit);
	Ray getTransmissionRay(const Ray& ray, const CollisionPoint&hit);
	bool isTotalInternalReflection(double incidenceAngle, double outMediumCoefficient, double inMediumCoefficient);
};

