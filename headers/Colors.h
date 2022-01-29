//
// Created by part4 on 16.01.2022.
//

#ifndef GRAFIKA_COLORS_H
#define GRAFIKA_COLORS_H


class Colors {
    float r = 0, g = 0, b = 0, a = 0;
public:
    Colors();
    Colors(float r,float g,float b, float a);

    [[nodiscard]] float getR() const;

    void setR(float r);

    [[nodiscard]] float getG() const;

    void setG(float g);

    [[nodiscard]] float getB() const;

    void setB(float b);

    [[nodiscard]] float getA() const;

    void setA(float a);
};


#endif //GRAFIKA_COLORS_H
