#pragma once
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void renderSphere(float radius = 0.25f);
void renderCone();
void renderTorus(float majorRadius = 0.5f, float minorRadius = 0.1f, unsigned int majorSegments = 64, unsigned int minorSegments = 32);

