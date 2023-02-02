#ifndef NUMBERBOX_H
#define NUMBERBOX_H
#include "Box.h"

typedef struct SaveNumberBox : SaveBox{
  unsigned short number;
  unsigned short startNumber;
  SaveNumberBox(byte x, byte y, unsigned short number, unsigned short startNumber) : SaveBox(x, y) {this->number = number; this->startNumber = startNumber;}
} SaveNumberBox;

class NumberBox : public Box{
    private:
        int startNumber;
        int number;
        void drawNumber(TFT& TFTscreen);
        void redrawNumber(TFT& TFTscreen);
    public: 
        NumberBox(int number, int startNumber, bool active) : Box(16, 16, active){this->number = number; this->startNumber = startNumber;}
        NumberBox(int number, int startNumber) : NumberBox(number, startNumber, 1){}
        NumberBox(int startNumber) : NumberBox(startNumber, startNumber){}
        char checkIntersection(Ball& ball,Vector& start, Vector& movement, TFT& TFTscreen);
        virtual void update(int ellapsedTime){}
        virtual void draw(TFT& TFTscreen);
        virtual char getType() {return 'n';/*a for number*/};
        int getNumber(){return this->number;}
        void setNumber(int number){this->number = number;}
        int getStartNumber(){return this->startNumber;}
};
#endif
