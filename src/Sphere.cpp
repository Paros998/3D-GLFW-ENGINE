//
// Created by part4 on 29.01.2022.
//
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm/gtc/type_ptr.hpp"
#include "../headers/Texture.h"
#include "../headers/Shader.h"
#include <cmath>
#include "vector"
#include "../headers/Vector2i.h"

using namespace glm;

class Sphere{
private:
    int stackCount;
    int sectorsCount;
    float diameter;

    ///view
    mat4 view = mat4(1.f);
    GLint viewUniformLoc{};

    mat4 projection = mat4(1.f);
    GLint projectionUniformLoc{};

    mat4 model = mat4(1.f);
    GLint modelUniformLoc{};

    ///update of objects
    mat4 transform = mat4(1.f);
    GLint transformUniformLoc{};

    Shader sphereShader;
    Texture sphereTexture1;
    Texture sphereTexture2;
    Texture sphereTexture3;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    double sectorAngle;
    double stackAngle;

    vec3 position;

    vector<float> VerticesAndTexCoords;
    vector<int> Indices;
    unsigned int numberOfIndices;
public:
    void calculateVerticesAndIndices() {

        float sectorStep = 2 * M_PI / sectorsCount;
        float stackStep = M_PI / stackCount;
        sectorAngle = 2 * M_PI * (sectorStep/sectorsCount);
        stackAngle = (M_PI / 2) - (M_PI * (stackStep/stackCount));

        float sectorAngle, stackAngle;
        float x, y, z, xy;
        float s, t;

        for(int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = diameter / 2 * cosf(stackAngle);      // r * cos(u)
            z = diameter / 2 * sinf(stackAngle);       // r * sin(u)

            for(int j = 0; j <= sectorsCount; ++j)
            {
                sectorAngle = j * sectorStep;              // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                VerticesAndTexCoords.push_back(x + position.x);
                VerticesAndTexCoords.push_back(y + position.y);
                VerticesAndTexCoords.push_back(z + position.z);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorsCount;
                t = (float)i / stackCount;
                VerticesAndTexCoords.push_back(s);
                VerticesAndTexCoords.push_back(t);
            }
        }

        int k1, k2;
        for(int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorsCount + 1);     // beginning of current stack
            k2 = k1 + sectorsCount + 1;      // beginning of next stack

            for(int j = 0; j < sectorsCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    Indices.push_back(k1);
                    Indices.push_back(k2);
                    Indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount-1))
                {
                    Indices.push_back(k1 + 1);
                    Indices.push_back(k2);
                    Indices.push_back(k2 + 1);
                }

            }
        }
    }

    Sphere()= default;;

    Sphere(int stackCount, int sectorsCount, float diameter, const char* sphereTexture1File, const char* sphereTexture2File,
           const char* sphereTexture3File, const char* vertexFile, const char* fragFile,vec3 position){
        this->stackCount = stackCount;
        this->sectorsCount = sectorsCount;
        this->diameter = diameter;
        this->position = position;

        sphereShader = Shader(vertexFile,fragFile);
        sphereTexture1 = Texture(sphereTexture1File);
        sphereTexture2 = Texture(sphereTexture2File);
        sphereTexture3 = Texture(sphereTexture3File);

        calculateVerticesAndIndices();

        numberOfIndices = Indices.size();

        transformUniformLoc = glad_glGetUniformLocation(sphereShader.ID, "transform");
        projectionUniformLoc = glad_glGetUniformLocation(sphereShader.ID, "projection");
        modelUniformLoc = glad_glGetUniformLocation(sphereShader.ID, "model");
        viewUniformLoc = glad_glGetUniformLocation(sphereShader.ID, "view");

        glad_glGenVertexArrays(1,&VAO);
        glad_glGenBuffers(1,&VBO);
        glad_glGenBuffers(1,&EBO);


        float* vertices = new float[VerticesAndTexCoords.size()];
        int i = 0;
        for(float vertex : VerticesAndTexCoords){
            vertices[i++] = vertex;
        }

        int* indices = new int[Indices.size()];
        i = 0;
        for(int indic : Indices){
            indices[i++] = indic;
        }

        glad_glBindVertexArray(VAO);

        glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glad_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VerticesAndTexCoords.size(), VerticesAndTexCoords.data(), GL_STATIC_DRAW);

        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * Indices.size(), Indices.data(), GL_STATIC_DRAW);


        ///2-Setting vertex linking for openGL (call once in prepare)
        glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (0 * sizeof(float)));
        glad_glEnableVertexAttribArray(0);

        /// texture attribute
        glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glad_glEnableVertexAttribArray(1);

        glad_glBindVertexArray(0);

    }

    void updateAndDraw(bool usePerspectiveProjection,Vector2i ScreenSize,vec3 cameraPos,vec3 cameraFront,vec3 cameraUp){
        glad_glActiveTexture(GL_TEXTURE2);
        glad_glBindTexture(GL_TEXTURE_2D, sphereTexture1.getTexture());
        glad_glActiveTexture(GL_TEXTURE3);
        glad_glBindTexture(GL_TEXTURE_2D, sphereTexture2.getTexture());
        glad_glActiveTexture(GL_TEXTURE4);
        glad_glBindTexture(GL_TEXTURE_2D, sphereTexture3.getTexture());

        sphereShader.use();

        glad_glBindVertexArray(VAO);

        sphereShader.setInt("nightTexture", 2);
        sphereShader.setInt("dayTexture", 3);
        sphereShader.setInt("cloudsTexture", 4);

        transform = rotate(transform, radians(0.05f), vec3(0.f, 0.f, -0.3f) );
        glad_glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, value_ptr(transform));

        if (usePerspectiveProjection)
            projection = perspective(radians(90.f), (float) ScreenSize.getX() / (float) ScreenSize.getY(), 0.1f,10000.f);
        else
            projection = ortho(0.f, (float) ScreenSize.getX(), 0.f, (float) ScreenSize.getY(), 0.1f, 10000.f);

        glad_glUniformMatrix4fv(projectionUniformLoc, 1, GL_FALSE, value_ptr(projection));

        glad_glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, value_ptr(model));

        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glad_glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, value_ptr(view));

        glad_glDrawElements(GL_TRIANGLES,numberOfIndices,GL_UNSIGNED_INT, nullptr);

    }

};

