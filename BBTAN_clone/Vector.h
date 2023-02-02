#ifndef VECTOR_H
#define VECTOR_H
#include <Arduino.h>
typedef struct Vector
{
    float x;
    float y;
    Vector(){x = 0; y = 0;} 
    Vector(float x, float y){this->x = x; this->y = y;} 

    bool nearlyEquals(Vector& vector);
    Vector resizeTo(float length);
    const Vector& operator = (const Vector& vector);
    const Vector& operator += (const Vector& vector);
    const Vector& operator -= (const Vector& vector);

    friend Vector operator - (const Vector vector);
    friend Vector operator - (const Vector& vector1, const Vector& vector2);
    friend Vector operator + (const Vector& vector1, const Vector& vector2);
    friend Vector operator * (const float& number, const Vector& vector);
    friend Vector operator * (const Vector& vector, const float& number);
    friend Vector operator / (const Vector& vector, const float& number);
    friend bool operator == (const Vector& vector1, const Vector& vector2);
} Vector;

#endif
