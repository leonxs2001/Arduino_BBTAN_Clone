#include "NumberBox.h"

char NumberBox::checkIntersection(Ball& ball,Vector& start, Vector& movement, TFT& TFTscreen){
    char intersectionDirection = checkIntersectionWithBox(start, movement);
    if(intersectionDirection){
        this->number -= ball.getDamage();
        
        if(this->number <= 0){
          this->deactivate();
          clearBox(TFTscreen);
        }else{
          redrawNumber(TFTscreen);
        }
    }
    return intersectionDirection;
}

void NumberBox::draw(TFT& TFTscreen){
  float ratio = (number * 1.0) / (startNumber * 1.0);
  if(ratio > 0.666){
    TFTscreen.stroke(0,0,255);
  } else if (ratio > 0.333){
    TFTscreen.stroke(0,255,255);
  } else {
    TFTscreen.stroke(0,255,0);
  }
  Box::draw(TFTscreen);

  drawNumber(TFTscreen);
}

void NumberBox::redrawNumber(TFT& TFTscreen){
  //clear last Number
  clearBox(TFTscreen);

  draw(TFTscreen);
  
  drawNumber(TFTscreen);
}

void NumberBox::drawNumber(TFT& TFTscreen){
  int number = this->number; 
  int countNumber = number;
  int width = 0;
  int length = 0;
  while(countNumber != 0){
    width += 1;
    countNumber /= 10;
  }
  length = (width * 4) - 1;
  float sideDistance = (16.0 - length)/2.0;
  while(number != 0){
    int digit = number % 10;
    Vector startPosition = Vector(position.x + sideDistance + (width-- - 1) * 4 ,position.y + 6);
    //draw specific digit
    switch(digit){
      case 0:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x, startPosition.y + 5);
        TFTscreen.line(startPosition.x + 2, startPosition.y, startPosition.x + 2, startPosition.y + 5);
        TFTscreen.point(startPosition.x + 1, startPosition.y);
        TFTscreen.point(startPosition.x + 1, startPosition.y + 4);
        break;
      case 1:
        TFTscreen.point(startPosition.x, startPosition.y + 1);
        TFTscreen.point(startPosition.x + 1, startPosition.y);
        TFTscreen.line(startPosition.x + 2, startPosition.y, startPosition.x + 2, startPosition.y + 5);
        break;
      case 2:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x , startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x, startPosition.y + 4, startPosition.x + 3, startPosition.y + 4);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 1);
        TFTscreen.point(startPosition.x, startPosition.y + 3);
        break;
       case 3:
        TFTscreen.line(startPosition.x + 2, startPosition.y, startPosition.x + 2, startPosition.y + 5);
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x, startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x, startPosition.y + 4,startPosition.x + 3, startPosition.y + 4);
        break;
       case 4:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x, startPosition.y + 3);
        TFTscreen.line(startPosition.x + 1, startPosition.y + 2, startPosition.x + 1, startPosition.y + 5);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 2);
        break;
       case 5:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x , startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x, startPosition.y + 4, startPosition.x + 3, startPosition.y + 4);
        TFTscreen.point(startPosition.x, startPosition.y + 1);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 3);
        break;
       case 6:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x, startPosition.y + 5);
        TFTscreen.line(startPosition.x + 1, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x + 1, startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x + 1, startPosition.y + 4, startPosition.x + 3, startPosition.y + 4);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 3);
        break;
       case 7:
        TFTscreen.point(startPosition.x, startPosition.y);
        TFTscreen.point(startPosition.x + 1, startPosition.y);
        TFTscreen.line(startPosition.x + 2, startPosition.y, startPosition.x + 2, startPosition.y + 5);
        break;
       case 8:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x , startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x, startPosition.y + 4, startPosition.x + 3, startPosition.y + 4);
        TFTscreen.point(startPosition.x, startPosition.y + 1);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 3);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 1);
        TFTscreen.point(startPosition.x, startPosition.y + 3);
        break;
       case 9:
        TFTscreen.line(startPosition.x, startPosition.y, startPosition.x + 3, startPosition.y);
        TFTscreen.line(startPosition.x , startPosition.y + 2,startPosition.x + 3, startPosition.y + 2);
        TFTscreen.line(startPosition.x, startPosition.y + 4, startPosition.x + 3, startPosition.y + 4);
        TFTscreen.point(startPosition.x, startPosition.y + 1);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 3);
        TFTscreen.point(startPosition.x + 2, startPosition.y + 1);
        break;
        
    }
    number /= 10;
  }
}
