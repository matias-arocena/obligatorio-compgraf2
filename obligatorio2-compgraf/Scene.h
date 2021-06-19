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
	
	void fresnel(const glm::vec3& I, const glm::vec3& N, const float& ior, float& kr);
	CollisionPoint* getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions);
	Ray getReflectiveRay(const Ray& ray, const CollisionPoint& hit);
	glm::vec3 getReflectiveVector(const glm::vec3 vec, const glm::vec3 axis);
	Ray getTransmissionRay(const Ray& ray, const CollisionPoint&hit);
	bool isTotalInternalReflection(double incidenceAngle, double outMediumCoefficient, double inMediumCoefficient);
};

