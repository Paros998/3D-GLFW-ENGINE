//
// Created by part4 on 16.01.2022.
//

#include "../headers/Colors.h"

float Colors::getR() const {
    return r;
}

void Colors::setR(float r) {
    Colors::r = r;
}

float Colors::getG() const {
    return g;
}

void Colors::setG(float g) {
    Colors::g = g;
}

float Colors::getB() const {
    return b;
}

void Colors::setB(float b) {
    Colors::b = b;
}

float Colors::getA() const {
    return a;
}

void Colors::setA(float a) {
    Colors::a = a;
}

Colors::Colors() = default;

Colors::Colors(float r, float g, float b, float a) {
    setA(a);
    setB(b);
    setG(g);
    setR(r);
}
