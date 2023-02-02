#ifndef SPRITE_H
#define SPRITE_H
#include <TFT.h>  
#include <SPI.h>
#include "Vector.h"

class Sprite{
    protected:
        Vector position;
    public:
        Sprite(Vector& position){this->position = position;};
        Sprite(){this->position.x = 0; this->position.y = 0;};
        Vector getPosition(){ return this->position;}
        void setPosition(Vector& position){this->position = position;}
        virtual void draw(TFT& TFTscreen) = 0;
};
#endif
