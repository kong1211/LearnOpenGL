#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H
#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class ShapeRenderer {
public:
    ShapeRenderer() : sphereVAO(0), coneVAO(0), torusVAO(0), sphereIndexCount(0), coneIndexCount(0), torusIndexCount(0) {}
    ~ShapeRenderer() {
        glDeleteVertexArrays(1, &sphereVAO);
        glDeleteVertexArrays(1, &coneVAO);
        glDeleteVertexArrays(1, &torusVAO);
    }

    void renderSphere(float radius, unsigned int xSegments, unsigned int ySegments);
    void renderCone(float height, float baseRadius, unsigned int sectorCount, unsigned int stackCount);
    void renderTorus(float majorRadius, float minorRadius, unsigned int majorSegments, unsigned int minorSegments);

private:
    void initializeSphere(float radius, unsigned int xSegments, unsigned int ySegments);
    void initializeCone(float height, float baseRadius, unsigned int sectorCount, unsigned int stackCount);
    void initializeTorus(float majorRadius, float minorRadius, unsigned int majorSegments, unsigned int minorSegments);

    unsigned int sphereVAO;
    unsigned int coneVAO;
    unsigned int torusVAO;
    unsigned int sphereIndexCount;
    unsigned int coneIndexCount;
    unsigned int torusIndexCount;
};

#endif // SHAPERENDERER_H
