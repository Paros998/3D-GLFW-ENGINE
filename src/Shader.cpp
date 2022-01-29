//
// Created by part4 on 28.01.2022.
//

#include "../headers/Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        // open files
        vShaderFile = ifstream(vertexPath,ios::in);
        fShaderFile = ifstream(fragmentPath,ios::in);
        stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure &e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();


    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glad_glCreateShader(GL_VERTEX_SHADER);
    glad_glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glad_glCompileShader(vertex);
    // print compile errors if any
    glad_glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glad_glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glad_glCompileShader(fragment);

    glad_glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glad_glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // shader Program
    ID = glad_glCreateProgram();
    glad_glAttachShader(ID, vertex);
    glad_glAttachShader(ID, fragment);
    glad_glLinkProgram(ID);

    // print linking errors if any
    glad_glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glad_glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glad_glDeleteShader(vertex);
    glad_glDeleteShader(fragment);

}

void Shader::use() const {
    glad_glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glad_glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glad_glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glad_glUniform1f(glad_glGetUniformLocation(ID, name.c_str()), value);
}

Shader::Shader() = default;
