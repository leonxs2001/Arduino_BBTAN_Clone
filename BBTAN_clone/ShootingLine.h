#ifndef SHOOTINGLINE_H
#define SHOOTINGLINE_H

#include "Vector.h"
#include <TFT.h>  
#include <SPI.h>
#include <Arduino.h>
class Box;
class ShootingLine{
    private:
        int leftPin;
        int rightPin;
        Vector start;
        Vector stop;
        Vector lastStop;
        int ellapsedTime;
        bool currentlyClicked;
        
    public:
        ShootingLine(){}
        ShootingLine(Vector& start, int leftPin, int rightPin) : ShootingLine(leftPin, rightPin){this->setStart(start);}
        ShootingLine(int leftPin, int rightPin){ellapsedTime = 0;this->leftPin = leftPin; this->rightPin = rightPin; currentlyClicked = false;}
        void setStart(Vector& start);
        Vector getStart(){return start;};
        Vector getStop(){return stop;};
        void update(int ellapsedTime, Box* fields[7][7], TFT& TFTscreen);
        void clear(Box* fields[7][7],TFT& TFTscreen);
        void draw(Box* fields[7][7], TFT& TFTscreen);
};
#endif
