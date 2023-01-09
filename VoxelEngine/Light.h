#pragma once

#include <glm/vec3.hpp>

struct Light {
	glm::vec3 color;
	GLuint isEnabled;
	glm::vec3 ambient;
	GLuint isLocal;
	glm::vec3 position;
	GLuint isSpot;
	glm::vec3 halfVector;
	float spotCosCutoff;
	glm::vec3 coneDirection;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float padding;
};
