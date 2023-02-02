#include "ShootingLine.h"
#include "Box.h"

void ShootingLine::setStart(Vector& start){
    this->start = start;
    //set to the top of the whole field
    this->stop = start - Vector(0, 150);
    //is a value, that the clearing process in the beginning dont destroys something
    this->lastStop = Vector(0, 155);
}

void ShootingLine::update(int ellapsedTime, Box* fields[7][7], TFT& TFTscreen){
    int leftPinRead = digitalRead(leftPin);
    int rightPinRead = digitalRead(rightPin);
    byte speed = 1;
    if(leftPinRead == HIGH || rightPinRead == HIGH){
      this->ellapsedTime += ellapsedTime;
      if(!this->currentlyClicked || this->ellapsedTime > 1000){

        if(this->ellapsedTime > 4000){
          speed = 9;
        }else if(this->ellapsedTime > 3000){
          speed = 7;
        }else if(this->ellapsedTime > 2000){
          speed = 5;
        }else if(this->ellapsedTime > 1500){
          speed = 3;
        }
        Serial.println(speed);
        this->lastStop = this->stop;
        if(leftPinRead == HIGH){
          if(stop.x > 0 && stop.x < 127){
            this->stop.x -= speed;
          }else if(stop.x <= 0){
            this->stop.y += speed;
          }else{
            if(stop.y >= 10){
              this->stop.y -= speed;
            }else{
              stop.x -= speed;
            }
          } 
        }else{
          if(stop.x > 0 && stop.x < 127){
            this->stop.x += speed;
          }else if(stop.x <= 0){
            if(stop.y >= 10){
              this->stop.y -= speed;
            }else{
              this->stop.x += speed;
            }
          }else{
            this->stop.y += speed;
          } 
        }
        this->currentlyClicked = true;

        if(stop.x < 0){
          stop.y = stop.x * -1 + 9;
          stop.x = 0;
        }else if(stop.x > 127){
          stop.y = stop.x - 127 + 9;
          stop.x = 127;
        }

        if(stop.y < 9){
          stop.y = 9;
        }else if(stop.y > 158){
          stop.y = 158;
        }

        draw(fields, TFTscreen);
      }
    }else{
      this->ellapsedTime = 0;
      this->currentlyClicked = false;
    }
   
}
void ShootingLine::clear(Box* fields[7][7], TFT& TFTscreen){
  TFTscreen.stroke(0,0,0);
  TFTscreen.line(this->start.x, this->start.y, this->stop.x, this->stop.y);
  Box::drawFields(fields, TFTscreen);
}

void ShootingLine::draw(Box* fields[7][7], TFT& TFTscreen){
  TFTscreen.stroke(0,0,0);
  TFTscreen.line(this->start.x, this->start.y, this->lastStop.x, this->lastStop.y);
  Box::drawFields(fields, TFTscreen);
  //draw new one
  TFTscreen.stroke(255,255,255);
  TFTscreen.line(this->start.x, this->start.y, this->stop.x, this->stop.y);
}
