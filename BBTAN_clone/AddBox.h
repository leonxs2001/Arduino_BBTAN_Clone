#ifndef ADDBOX_H
#define ADDBOX_H

#include "Box.h"

typedef struct SaveAddBox : SaveBox{
  SaveAddBox(byte x, byte y) : SaveBox(x, y) {}
} SaveAddBox;

class AddBox : public Box{

    public: 
        AddBox(bool active) : Box(10, 10, active){}
        AddBox(): Box(10,10, 1){}
        char checkIntersection(Ball& ball, Vector& start, Vector& movement, TFT& TFTscreen);
        virtual void update(int ellapsedTime){}
        virtual void draw(TFT& TFTscreen);
        virtual char getType() {return 'a';/*a for add*/};
};

#endif
