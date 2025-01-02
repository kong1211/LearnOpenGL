#include "ShapeRenderer.h"

void ShapeRenderer::renderSphere(float radius, unsigned int xSegments, unsigned int ySegments) {
    if (sphereVAO == 0) {
        initializeSphere(radius, xSegments, ySegments);
    }
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, 0);
}

void ShapeRenderer::renderCone(float height, float baseRadius, unsigned int sectorCount, unsigned int stackCount) {
    if (coneVAO == 0) {
        initializeCone(height, baseRadius, sectorCount, stackCount);
    }
    glBindVertexArray(coneVAO);
    glDrawElements(GL_TRIANGLES, coneIndexCount, GL_UNSIGNED_INT, 0);
}

void ShapeRenderer::renderTorus(float majorRadius, float minorRadius, unsigned int majorSegments, unsigned int minorSegments) {
    if (torusVAO == 0) {
        initializeTorus(majorRadius, minorRadius, majorSegments, minorSegments);
    }
    glBindVertexArray(torusVAO);
    glDrawElements(GL_TRIANGLES, torusIndexCount, GL_UNSIGNED_INT, 0);
}

void ShapeRenderer::initializeSphere(float radius, unsigned int xSegments, unsigned int ySegments) {
    glGenVertexArrays(1, &sphereVAO);
    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;
    for (unsigned int x = 0; x <= xSegments; ++x) {
        for (unsigned int y = 0; y <= ySegments; ++y) {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;
            float xPos = radius * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = radius * std::cos(ySegment * PI);
            float zPos = radius * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos / radius, yPos / radius, zPos / radius));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < ySegments; ++y) {
        if (!oddRow) {
            for (unsigned int x = 0; x <= xSegments; ++x) {
                indices.push_back(y * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x);
            }
        }
        else {
            for (int x = xSegments; x >= 0; --x) {
                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y * (xSegments + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    sphereIndexCount = static_cast<unsigned int>(indices.size());

    std::vector<float> data;
    for (unsigned int i = 0; i < positions.size(); ++i) {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (!normals.empty()) {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
        if (!uv.empty()) {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }

    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    unsigned int stride = (3 + 3 + 2) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
}

void ShapeRenderer::initializeCone(float height, float baseRadius, unsigned int sectorCount, unsigned int stackCount) {
    glGenVertexArrays(1, &coneVAO);

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    const float PI = 3.14159265359f;

    std::vector<float> data;
    std::vector<unsigned int> indices;

    // Generate side vertices and indices
    for (unsigned int i = 0; i <= stackCount; ++i) {
        float z = -(height * 0.5f) + (float)i / stackCount * height;
        float radius = baseRadius * (1.0f - (float)i / stackCount);
        float t = 1.0f - (float)i / stackCount;

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = (float)j / sectorCount * 2.0f * PI;
            float x = cos(sectorAngle);
            float y = sin(sectorAngle);

            // Position
            data.push_back(x * radius);
            data.push_back(y * radius);
            data.push_back(z);

            // Normal
            data.push_back(x);
            data.push_back(y);
            data.push_back(baseRadius / height);

            // UV
            data.push_back((float)j / sectorCount);
            data.push_back(t);

            if (i < stackCount && j < sectorCount) {
                unsigned int k1 = i * (sectorCount + 1) + j;
                unsigned int k2 = k1 + sectorCount + 1;

                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);

                indices.push_back(k2);
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
            }
        }
    }

    // Generate base vertices and indices
    unsigned int baseIndex = data.size() / 8; // Each vertex has 8 attributes (3 position + 3 normal + 2 UV)
    float z = -height * 0.5f;

    // Center vertex
    data.push_back(0.0f);
    data.push_back(0.0f);
    data.push_back(z);
    data.push_back(0.0f);
    data.push_back(0.0f);
    data.push_back(-1.0f);
    data.push_back(0.5f);
    data.push_back(0.5f);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
        float sectorAngle = (float)j / sectorCount * 2.0f * PI;
        float x = cos(sectorAngle);
        float y = sin(sectorAngle);

        // Edge vertices
        data.push_back(x * baseRadius);
        data.push_back(y * baseRadius);
        data.push_back(z);
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(-1.0f);
        data.push_back(-x * 0.5f + 0.5f);
        data.push_back(-y * 0.5f + 0.5f);

        if (j < sectorCount) {
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + j + 2);
            indices.push_back(baseIndex + j + 1);
        }
    }

    coneIndexCount = indices.size();

    // Upload data to GPU
    glBindVertexArray(coneVAO);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    unsigned int stride = (3 + 3 + 2) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
}


void ShapeRenderer::initializeTorus(float majorRadius, float minorRadius, unsigned int majorSegments, unsigned int minorSegments) {
    if (torusVAO == 0) {
        glGenVertexArrays(1, &torusVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        const float PI = 3.14159265359f;
        std::vector<float> data;
        std::vector<unsigned int> indices;

        // Generate vertices and indices
        for (unsigned int i = 0; i <= majorSegments; ++i) {
            float majorAngle = (float)i / majorSegments * 2.0f * PI;
            float cosMajor = cos(majorAngle);
            float sinMajor = sin(majorAngle);

            for (unsigned int j = 0; j <= minorSegments; ++j) {
                float minorAngle = (float)j / minorSegments * 2.0f * PI;
                float cosMinor = cos(minorAngle);
                float sinMinor = sin(minorAngle);

                // Position
                float x = (majorRadius + minorRadius * cosMinor) * cosMajor;
                float y = (majorRadius + minorRadius * cosMinor) * sinMajor;
                float z = minorRadius * sinMinor;
                data.insert(data.end(), { x, y, z });

                // Normal
                float nx = cosMinor * cosMajor;
                float ny = cosMinor * sinMajor;
                float nz = sinMinor;
                data.insert(data.end(), { nx, ny, nz });

                // UV
                float u = (float)i / majorSegments;
                float v = (float)j / minorSegments;
                data.insert(data.end(), { u, v });

                // Indices
                if (i < majorSegments && j < minorSegments) {
                    unsigned int k1 = i * (minorSegments + 1) + j;
                    unsigned int k2 = k1 + minorSegments + 1;

                    indices.insert(indices.end(), { k1, k2, k1 + 1, k2, k2 + 1, k1 + 1 });
                }
            }
        }

        torusIndexCount = indices.size();

        // Upload data to GPU
        glBindVertexArray(torusVAO);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        unsigned int stride = (3 + 3 + 2) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }
}

