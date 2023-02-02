#ifndef BALL_H
#define BALL_H
#include "Vector.h"
#include "Sprite.h"

//later defined in Box.h
class Box;

class Ball : public Sprite{
    private:
        Vector lastPosition;
        Vector movement;
        int damage;
    public:
        Ball(Vector& position, Vector& movement, int damage) : Sprite(position){setMovement(movement); this->damage=damage;this->lastPosition = this->position;}
        Ball(Vector& position, Vector& movement) : Sprite(position){setMovement(movement); damage=1;this->lastPosition = this->position;}
        Ball() : Sprite(){Vector mov = Vector(-1, -1); this->setMovement(mov); this->lastPosition = this->position;} 
        bool update(int ellapsedTime, Box* fields[7][7], TFT& TFTscreen);
        void draw(TFT& TFTscreen);
        Vector getIndicesByPosition();
        Vector getMovement(){ return this->movement;}
        void calculateNewMovement(char direction);
        void setMovement(Vector& movement){this->movement = movement.resizeTo(2.5);}
        int getDamage(){return damage;}
        void setDamage(int damage){this->damage = damage;}
        void drawDamage(TFT& TFTscreen);

};
#endif
