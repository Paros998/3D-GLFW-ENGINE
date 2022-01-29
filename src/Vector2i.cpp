//
// Created by part4 on 15.01.2022.
//

#include "../headers/Vector2i.h"

int Vector2i::getX() const {
    return x;
}

void Vector2i::setX(int x) {
    Vector2i::x = x;
}

int Vector2i::getY() const {
    return y;
}

void Vector2i::setY(int y) {
    Vector2i::y = y;
}

Vector2i::Vector2i() {
    setX(0);
    setY(0);
}

Vector2i::Vector2i(int x, int y) {
    setX(x);
    setY(y);
}
