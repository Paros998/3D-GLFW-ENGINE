//
// Created by part4 on 29.01.2022.
//

template <typename T>
class Vector2  {
    T x,y;
public:
    Vector2()= default;
    Vector2(T x,T y){
        this->x = x;
        this->y = y;
    }

    T getX() const{
        return x;
    }

    void setX(T x){
        this->x = x;
    }

    T getY() const{
        return y;
    }

    void setY(T y){
        this->y = y;
    }
};