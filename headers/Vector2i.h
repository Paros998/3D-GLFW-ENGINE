//
// Created by part4 on 15.01.2022.
//

#ifndef GRAFIKA_VECTOR2I_H
#define GRAFIKA_VECTOR2I_H


class Vector2i {
    int x,y;
public:
    Vector2i();
    Vector2i(int x,int y);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);
};


#endif //GRAFIKA_VECTOR2I_H
