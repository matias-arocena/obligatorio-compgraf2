#include "Scene.h"

#include <iostream>
#include <algorithm>   

#include <SDL.h>
#include <pugixml.hpp>
#include <glm/glm.hpp>

#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"

Scene::Scene()
{
}

void Scene::loadSceneFromFile()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("../escena.xml");

	projectionCenter = glm::vec3(
		doc.child("projectionCenter").attribute("x").as_int(),
		doc.child("projectionCenter").attribute("y").as_int(),
		doc.child("projectionCenter").attribute("z").as_int()
	);

	Sphere* testSphere = new Sphere(.5f, glm::vec3(0.0, 1.0, 2.f));
	testSphere->diffuse = Color();
	testSphere->diffuse.rgb = glm::vec3(1, 0, 0);
	testSphere->ambient = Color();
	testSphere->ambient.rgb = glm::vec3(1, 0, 0);
	testSphere->specular = Color();
	testSphere->specular.rgb = glm::vec3(1, 0, 0);
	testSphere->shininess = 2.f;
	testSphere->reflectionCoefficient = 2.f;
	objects.push_back(testSphere);

	Sphere* testSphere2 = new Sphere(.5f, glm::vec3(-1.5, 1.0, 2.f));
	testSphere2->diffuse = Color();
	testSphere2->diffuse.rgb = glm::vec3(0, 0, 0);
	testSphere2->ambient = Color();
	testSphere2->ambient.rgb = glm::vec3(0, 0, 0);
	testSphere2->specular = Color();
	testSphere2->specular.rgb = glm::vec3(0, 0, 0);
	testSphere2->shininess = 2.f;
	testSphere2->reflectionCoefficient = 5.f;


	objects.push_back(testSphere2);

	Sphere* world = new Sphere(105.f, glm::vec3(0.0, 110.0, 2.f));
	world->diffuse = Color();
	world->diffuse.rgb = glm::vec3(0, 1, 0);
	world->ambient = Color();
	world->ambient.rgb = glm::vec3(0, 1, 0);
	world->specular = Color();
	world->specular.rgb = glm::vec3(1, 0, 0);
	world->shininess = 0.f;
	world->reflectionCoefficient = 0.f;

	objects.push_back(world);

	Color white;
	white.rgb = glm::vec3(1, 1, 1);
	ambientLight = new Light(white, 0.1f);
	
	//obtener luz de xml
	PositionLight* l = new PositionLight(glm::vec3(0, -3
		, -1), white, 1);
	lights.push_back(l);

	//obtener bk del xml
	backgroudColor.rgb = glm::vec3(0, 0, 0);
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;

	//TODO: obtener camara de xml
	camera = new Camera(glm::vec3(0, 0, 0) , 1.f, 2.f);

	maxDepth = 2;
}

void Scene::render(SDL_Renderer* renderer) {
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		double v = (double)y / SCREEN_HEIGHT;
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			double u = (double)x / SCREEN_WIDTH;


			Ray ray(
				camera->getPosition(),
				camera->getDirectionToViewport(u, v)
			);

			Color pixel(rayTrace(ray, 1));

			// std::cout << "(" << x << "," << y << ") (" << u << "," << v << ") (" << pixel.rgb.r << "," << pixel.rgb.g << "," << pixel.rgb.b << ")" << std::endl;
			SDL_SetRenderDrawColor(renderer, 255 * pixel.rgb.r, 255 * pixel.rgb.g, 255 * pixel.rgb.b, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}

Color Scene::rayTrace(const Ray& ray, int depth)
{
	std::vector<CollisionPoint*> intersections;
	for (auto &obj : objects) {
		CollisionPoint* hit = obj->intersects(ray);
		if (hit != nullptr) {
			intersections.push_back(hit);
		}
	}
	CollisionPoint* collisionPoint = getClosestObject(ray, intersections);
	if (collisionPoint != nullptr) {
		return shadow(collisionPoint, ray, depth);
	}
	else {

		glm::vec3 rgb = backgroudColor.rgb;
		Color color;
		color.rgb = rgb;
		return color;
	}
}

Color Scene::shadow(const CollisionPoint* hit, const Ray& ray, int depth)
{
	Color diffuse;
	diffuse.rgb = glm::vec3(0, 0, 0);
	Color specular;
	specular.rgb = glm::vec3(0, 0, 0);
	/*
	Color transparency;
	transparency.rgb = glm::vec3(0, 0, 0);

	float ambientK = 0;
	float diffuseK = 0;
	float specularK = 0;

	double reflectionCoefficient = 1.0;
	double transmissionCoefficient = 1.0;
*/
	float brightness = 0;
	for (PositionLight* light : lights) {


		Ray rayToLight(hit->position, light->getPosition() - hit->position);

		if (glm::dot(hit->normal, rayToLight.getDirection()) > 0) {
			bool intersects = false;
			for (auto& obj : objects) {
				CollisionPoint* otherHit = obj->intersects(rayToLight);
				if (otherHit != nullptr && otherHit->distanceFromOrigin < glm::length(light->getPosition() - otherHit->position)) {
					if (otherHit->object != hit->object) {
						intersects = true;
					}
					intersects = true;
					break;
				}
			}
			if (!intersects) {
				float angle = std::asin(glm::dot(hit->normal, rayToLight.getDirection()));
				if (angle > 0) {
					diffuse.rgb += angle * light->getIntensity() * hit->object->diffuse.rgb;
					/*if (hit->object->getReflectionCoefficient() == 0) brightness += angle * light->getIntensity(); //TODO: diffuse coef?*/

					glm::vec3 reflected_light = glm::normalize(getReflectiveVector(rayToLight.getDirection(), hit->normal));
					specular.rgb +=  hit->object->getReflectionCoefficient() * 
									std::pow(std::max(0.f, glm::dot(reflected_light, (-hit->hitDir))), hit->object->getShininess()) *
									light->getIntensity() * 0.3f * 
									glm::vec3(1,1,1);
					/*if (hit->object->shininess > 0)
					{
						std::cout << "dot " << glm::dot(reflected_light, (-hit->hitDir)) << std::endl;
						std::cout << "ref coef " << hit->object->getReflectionCoefficient() << std::endl;
						std::cout << "pow " << std::pow(std::max(0.f, glm::dot(reflected_light, (-hit->hitDir))), hit->object->getShininess()) << std::endl;
						std::cout << "light " << light->getIntensity() * 0.3f << std::endl;
						std::cout << "value " << hit->object->getReflectionCoefficient() * std::max(0.f, std::pow(glm::dot(reflected_light, (-hit->hitDir)), hit->object->getShininess())) * light->getIntensity() * 0.3f << std::endl;
						std::cout << "r " << specular.rgb.r << std::endl;
					}*/
				}
			}
		}
	}
	Color result;
	result.rgb = glm::vec3(0, 0, 0);
	if (depth < maxDepth && hit->object->getReflectionCoefficient() > 0) {
		float kr;
		fresnel(hit->hitDir, hit->normal, hit->object->getReflectionCoefficient(), kr);

		Color reflectiveColor;
		if (hit->object->getReflectionCoefficient() > 0) {
			Ray reflectiveRay = getReflectiveRay(ray, *hit);
			reflectiveColor = rayTrace(reflectiveRay, depth + 1);
		}

		result.rgb = reflectiveColor.rgb * kr;

	}
	//diffuse.rgb = brightness * hit->object->diffuse.rgb;

	result.rgb += hit->object->ambient.rgb * ambientLight->getIntensity() + diffuse.rgb + specular.rgb;
	result.rgb.r = result.rgb.r * (result.rgb.r < 1.f) + (result.rgb.r >= 1);
	result.rgb.g = result.rgb.g * (result.rgb.g < 1.f) + (result.rgb.g >= 1);
	result.rgb.b = result.rgb.b * (result.rgb.b < 1.f) + (result.rgb.b >= 1);

	
	//std::cout << diffuse.rgb.r << "," << diffuse.rgb.g << ", " << diffuse.rgb.g << std::endl;
	return result;
			/*
			if (!intersects) {
				diffuseK += light->getIntensity() * std::max(0.f, glm::dot(hit->normal, -rayToLight.getDir()));

				//Ver en nuestro caso el valor de n
				int n = 10;
				glm::vec3 V = glm::normalize(camera->getPosition() -hit->position);
				glm::vec3 R = glm::normalize(2 * glm::dot(hit->normal, rayToLight.getDir()) * hit->normal - rayToLight.getDir());
				
				specularK += std::max(0.f, (float)pow(glm::dot(V, R), n));

			}

		}
	}
	if (depth < maxDepth) {
		if (hit->object->getReflectionCoefficient() > 0) {
			Ray reflectiveRay = getReflectiveRay(ray, *hit);
			Color reflectiveColor = rayTrace(reflectiveRay, depth + 1);
				
			specular.rgb += reflectiveColor.rgb * hit->object->getReflectionCoefficient();
		}
		
		if (hit->object->getTransmissionCoefficient() > 0) {
			//TODO: Ver si necesitamos considerar 2 objetos intersectandose 
			if (!isTotalInternalReflection(hit->angle, AIR, hit->object->getMedium())) {
				Ray transmissionRay = getTransmissionRay(ray, *hit);
				Color transmissionColor = rayTrace(transmissionRay, depth + 1);
				//TODO: Donde va la transparencia?
				transparency.rgb += transmissionColor.rgb * hit->object->getReflectionCoefficient();
			}
		}
	}

	float red = ambient.rgb[0] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[0] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[0] * specularK / (ambientK + diffuseK + specularK);
	float green = ambient.rgb[1] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[1] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[1] * specularK / (ambientK + diffuseK + specularK);
	float blue = ambient.rgb[2] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[2] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[2] * specularK / (ambientK + diffuseK + specularK);


	Color result;
	//result.rgb = glm::vec3(red, green, blue);
	result.rgb = glm::vec3(0, 0, 0);
	result.reflection = hit->object->getReflectionCoefficient();
	result.transmission = hit->object->getTransmissionCoefficient();
	return result;
	*/

}

void Scene::fresnel(const glm::vec3& I, const glm::vec3& N, const float& ior, float& kr)
{
	float cosi = glm::clamp(-1.f, 1.f, glm::dot(I, N));
	float etai = 1, etat = ior;
	if (cosi > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}


CollisionPoint* Scene::getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions)
{
	CollisionPoint* curClosest = nullptr;
	for (int i = 0; i < collisions.size(); i++)
	{
		if (curClosest == nullptr || collisions[i]->position.z < curClosest->position.z)
		{
			curClosest = collisions[i];
		}
	}
	return curClosest;
}

Ray Scene::getReflectiveRay(const Ray& ray, const CollisionPoint& hit)
{
	return Ray(hit.position, hit.hitDir - 2 * glm::dot(hit.hitDir, hit.normal) * hit.normal);
//	return Ray(glm::vec3(std::cos(hit.angle) / hit.normal.x, std::cos(hit.angle) / hit.normal.y, std::cos(hit.angle) / hit.normal.z) + hit.position, hit.position);
}

glm::vec3 Scene::getReflectiveVector(const glm::vec3 vec, const glm::vec3 axis)
{
	return 2 * (glm::dot(vec, axis)) * axis - vec;
}

// TODO
Ray Scene::getTransmissionRay(const Ray& ray, const CollisionPoint& hit)
{
	float angle = std::asin((AIR * std::sin(hit.angle)) / hit.object->getMedium());
	return Ray(-glm::vec3(std::cos(angle) / hit.normal.x, std::cos(angle) / hit.normal.y, std::cos(angle) / hit.normal.z) + hit.position, hit.position);
}

bool Scene::isTotalInternalReflection(double incidenceAngle, double outMediumCoefficient, double inMediumCoefficient)
{
	if (outMediumCoefficient < inMediumCoefficient)
		return false;
	return incidenceAngle > std::asin(inMediumCoefficient/outMediumCoefficient);
}

bool distanceComparator(const CollisionPoint& a, const CollisionPoint& b) {
	return a.distanceFromOrigin < b.distanceFromOrigin;
}
