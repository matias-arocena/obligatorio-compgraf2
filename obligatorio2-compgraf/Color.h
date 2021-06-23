#pragma once

#include <glm/glm.hpp>

struct Color {
	glm::dvec3 rgb;
	glm::dvec3 ambient;
	glm::dvec3 diffuse;
	glm::dvec3 specular;
	double reflection;
	double transmission;
};
