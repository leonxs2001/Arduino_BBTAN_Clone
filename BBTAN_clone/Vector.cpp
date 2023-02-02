#include "Vector.h"
/*checks if the vectors are nearly eaqual(converted to int)*/
bool Vector::nearlyEquals(Vector& vector){
    return (((int)x) == ((int)vector.x)) && (((int)y) == ((int)vector.y));
}

Vector Vector::resizeTo(float length){
  *this = (*this / sqrt(this->x * this->x + this->y * this->y)) * length;
}

const Vector& Vector::operator = (const Vector& vector){
    if(&vector != this){
        this->x = vector.x;
        this->y = vector.y;
    }

    return *this;
}

const Vector& Vector::operator += (const Vector& vector){
    this->x += vector.x;
    this->y += vector.y;
    
    return *this;
}

const Vector& Vector::operator -= (const Vector& vector){
    this->x -= vector.x;
    this->y -= vector.y;
    
    return *this;
}

Vector operator - (const Vector vector){
    Vector newVector;
    newVector.x = -vector.x;
    newVector.y = -vector.y;
    return newVector;
}

Vector operator - (const Vector& vector1, const Vector& vector2){
    Vector newVector;
    newVector.x = vector1.x - vector2.x;
    newVector.y = vector1.y - vector2.y;
    return newVector;
}

Vector operator + (const Vector& vector1, const Vector& vector2){
    Vector newVector;
    newVector.x = vector1.x + vector2.x;
    newVector.y = vector1.y + vector2.y;
    return newVector;
}

Vector operator * (const float& number, const Vector& vector){
    Vector newVector;
    newVector.x = number * vector.x;
    newVector.y = number * vector.y;
    return newVector;
}

Vector operator * (const Vector& vector, const float& number){
    return number * vector;
}

Vector operator / (const Vector& vector, const float& number){
    Vector newVector;
    newVector.x = vector.x / number;
    newVector.y = vector.y / number;
    return newVector;
}

bool operator == (const Vector& vector1, const Vector& vector2){
  return vector1.x == vector2.x && vector1.y == vector2.y;
}
