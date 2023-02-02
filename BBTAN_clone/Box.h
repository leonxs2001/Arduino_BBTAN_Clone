#ifndef BOX_H
#define BOX_H
#include "Sprite.h"
#include "Vector.h"
#include "Ball.h"

typedef struct SaveBox{
  byte x;
  byte y;
  SaveBox(byte x, byte y){this->x = x; this->y = y;}
} SaveBox;

class Box : public Sprite{
    private:
        Vector size;
        bool active;
    protected:
        float checkIntersection(Vector& start1, Vector& movement1, Vector& start2, Vector& movement2);
    public:
        Box(int width, int height, bool active) : Sprite() {size.x = width; size.y = height; this->active = active;}
        Box(int width, int height) : Box(width, height, 0){}
        ~Box(){this->active = 0;}
        Vector getSize(){return size;} 
        virtual char checkIntersection(Ball& ball,Vector& start, Vector& movement, TFT& TFTscreen) = 0;
        char checkIntersectionWithBox(Vector& start, Vector& movement);
        void setPositionByIndices(int x, int y);
        virtual void update(int ellapsedTime) = 0;
        virtual void draw(TFT& TFTscreen);
        void clearBox(TFT& TFTscreen);
        void deactivate(){this->active = 0;}
        bool getActive(){return this->active;}
        virtual char getType() = 0;
        static void drawFields(Box* fields[7][7], TFT& TFTscreen);
        static void addLineOfBoxes(int roundScore, Box* fields[7][7], TFT& TFTscreen);
        static bool nextStep(int roundScore, Box* fields[7][7], TFT& TFTscreen);
};

#endif
