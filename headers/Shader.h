//
// Created by part4 on 28.01.2022.
//

#ifndef GRAFIKA_SHADER_H
#define GRAFIKA_SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
    public:
        // the program ID
        unsigned int ID{};
        Shader();
        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use() const;
        // utility uniform functions
        void setBool(const string &name, bool value) const;
        void setInt(const string &name, int value) const;
        void setFloat(const string &name, float value) const;
};


#endif //GRAFIKA_SHADER_H
