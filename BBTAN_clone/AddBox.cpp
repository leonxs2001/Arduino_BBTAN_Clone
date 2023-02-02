#include "AddBox.h"
char AddBox::checkIntersection(Ball& ball,Vector& start, Vector& movement, TFT& TFTscreen){
    char intersectionDirection = checkIntersectionWithBox(start, movement);
   
    if(intersectionDirection){
        ball.setDamage(ball.getDamage() + 1);
        ball.drawDamage(TFTscreen);
        clearBox(TFTscreen);
        this->deactivate();
    }
    return intersectionDirection;
}

void AddBox::draw(TFT& TFTscreen){
  TFTscreen.stroke(0,255,255);
  Box::draw(TFTscreen);

  //draw cross +
  TFTscreen.fill(0,255,0); 
  TFTscreen.rect(position.x + 2,position.y + 4,6,2);
  TFTscreen.rect(position.x + 4,position.y + 2,2,6);
  TFTscreen.noFill(); 
}
