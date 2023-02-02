#include "Ball.h"
#include "Box.h"
#include <Arduino.h>
//calculates and set the new movemet
//the direction is how the ball touches the wall
//|<- or ->| = 'v' for vertical
//and the other 'h' for horizontal
void Ball::calculateNewMovement(char direction){
  if(direction == 'v'){
    this->movement.x *= -1;
  }else{
    this->movement.y *= -1;
  }
}

bool Ball::update(int ellapsedTime, Box* fields[7][7], TFT& TFTscreen){
  bool result = false;
  lastPosition = position;
  Vector newMovement = movement * (ellapsedTime * 0.05);
  position += newMovement;
  float relative = 0;
  //n means no
  char direction = 'n';
  //wall
  if(position.x < 0){
    relative = (position.x) / newMovement.x;
    position.x = 0;
    direction = 'v';
  }else if(position.x > 127){
    relative = (position.x - 127) / newMovement.x;
    position.x = 127;
    direction = 'v';
  }else if(position.y < 9){
    relative = (position.y - 9) / newMovement.y;
    position.y = 9;
    direction = 'h';
  }else if(position.y > 159){
    relative = (position.y - 159) / newMovement.y;
    position.y = 159;
    direction = 'h';
    result = true;
  }else{//could only collide with a box if it is not collded with a wall

    //boxes
    Vector indices = getIndicesByPosition();
    //if its -1 its outside the fieldpositions
    
    if(indices.x >= 0){

      Box* box = fields[(int)indices.x][(int)indices.y];
      if(box != 0 && box->getActive()){

        //cast the lastPosition and the newMovement(casted position - casted lastPosition) because the Positions on the screen are Integers
        Vector castedNewMovement = Vector(((int)position.x) - ((int)lastPosition.x),((int)position.y) - ((int)lastPosition.y) );
        Vector castedLastPosition = Vector((int)lastPosition.x, (int) lastPosition.y);
        char intersectionDirection = box->checkIntersection(*this, castedLastPosition, castedNewMovement, TFTscreen);
        
        if(intersectionDirection){
          Vector boxPosition = box->getPosition();
          Vector rightBottom = boxPosition + box->getSize() - Vector(1, 1);
          if(intersectionDirection == 't'){
            float newY = boxPosition.y - 1;
            relative = (position.y - newY) / newMovement.y;
            position.y = newY;
            direction = 'h';
          }else if(intersectionDirection == 'b'){
            float newY = rightBottom.y + 1;
            relative = (position.y - newY) / newMovement.y;
            position.y = newY;
            direction = 'h';
          }else if(intersectionDirection == 'l'){
            float newX = boxPosition.x - 1;
            relative = (position.x - newX) / newMovement.x;
            position.x = newX;
            direction = 'v';
          }else{
            float newX = rightBottom.x + 1;
            relative = (position.x - newX) / newMovement.x;
            position.x = newX;
            direction = 'v';
          }
        }
      }
    }
  }


  if(direction != 'n'){
    if(direction == 'v'){
      position.y -= relative * newMovement.y;
    }else if(direction == 'h'){
      position.x -= relative * newMovement.x;
    }
    calculateNewMovement(direction);
  }
  return result;
}

void Ball::draw(TFT& TFTscreen){
  //only draw the ball if there is a real change
  if(!position.nearlyEquals(lastPosition)){
    //draw new ball
    TFTscreen.stroke(255,255,255);
    TFTscreen.point(position.x, position.y);

    //delete old one
    TFTscreen.stroke(0,0,0);
    TFTscreen.point(lastPosition.x, lastPosition.y);
  }
 }

Vector Ball::getIndicesByPosition(){
  if(position.x >= 1 && position.x <= 126 && position.y >= 25 && position.y <= 150){
    return Vector((int)((position.x - 1) / 18), (int)((position.y - 25) / 18));
  }else{
    return Vector(-1, -1);
  }
}

void Ball::drawDamage(TFT& TFTscreen){
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.fill(0, 0, 0);
  TFTscreen.rect(66, 0, 127, 8);
  TFTscreen.noFill();
  TFTscreen.stroke(255, 255, 255);
  char str[20];
  sprintf(str, "Hits: %d", damage); // integer to string
  TFTscreen.text(str, 66, 0);
}
