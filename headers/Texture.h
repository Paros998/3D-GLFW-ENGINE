//
// Created by part4 on 28.01.2022.
//

#ifndef GRAFIKA_TEXTURE_H
#define GRAFIKA_TEXTURE_H

#include "glad/glad.h"
#include "iostream"

using namespace std;

class Texture {
private:
    unsigned int texture{};

public:
    Texture();
    explicit Texture(const char* textureSource);
    [[nodiscard]] unsigned int getTexture() const;
};

#endif //GRAFIKA_TEXTURE_H
