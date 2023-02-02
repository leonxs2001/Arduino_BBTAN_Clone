#include "Box.h"
#include "AddBox.h"
#include "NumberBox.h"

void Box::setPositionByIndices(int x, int y){
    position.x = (1 + 18 * x) + (18 - size.x)/2;
    position.y = (25 + 18 * y) + (18 - size.y)/2;
}

void Box::draw(TFT& TFTscreen){
  TFTscreen.rect(position.x, position.y, size.x, size.y);
}
//returns the first intersection as direction t = top, b = bottom, l = left, r = right
char Box::checkIntersectionWithBox(Vector& start, Vector& movement){
  Vector rightBottom = position + size - Vector(1, 1);

  Vector rightMovement = Vector(size.x - 1, 0);
  Vector downMovement = Vector(0, size.y - 1);
  Vector upMovement = Vector(0, -(size.y - 1));
  Vector leftMovement = Vector(-(size.x - 1), 0);

  float rTop,rLeft,rRight,rBottom;
  rTop = checkIntersection(start, movement, position, rightMovement);
  rLeft = checkIntersection(start, movement, position, downMovement);
  rRight = checkIntersection(start, movement, rightBottom, upMovement);
  rBottom = checkIntersection(start, movement, rightBottom, leftMovement);
  
  /*Serial.print(rTop);
  Serial.print(" ");
  Serial.print(rLeft);
  Serial.print(" ");
  Serial.print(rRight);
  Serial.print(" ");
  Serial.print(rBottom);
  Serial.print("\n");*/
  //first part is to check, if this direction is even possible with this direction
  if(movement.y >= 0 && rTop != -1 && (rLeft == -1 || rTop <= rLeft) && (rRight == -1 || rTop <= rRight) && (rBottom == -1 || rTop <= rBottom)){//first intersection on the top
    return 't';
  }else if(movement.x >= 0 && rLeft != -1 && (rTop == -1 || rLeft <= rTop) && (rRight == -1 || rLeft <= rRight) && (rBottom == -1 || rLeft <= rBottom)){//first intersection on the left
    return 'l';
  }else if(movement.x <= 0 && rRight != -1 && (rTop == -1 || rRight <= rTop) && (rLeft == -1 || rRight <= rLeft) && (rBottom == -1 || rRight <= rBottom)){//first intersection on the right
    return 'r';
  }else if(movement.y <= 0 && rBottom != -1 && (rTop == -1 || rBottom <= rTop) && (rLeft == -1 || rBottom <= rLeft) && (rRight == -1 || rBottom <= rRight)){//first intersection on the bottom
    return 'b';
  }else{
    return 0;
  }
  
}
/*returns r of first given and -1 if not intersects*/
float Box::checkIntersection(Vector& start1, Vector& movement1, Vector& start2, Vector& movement2){
  float r = (movement2.x * (start2.y - start1.y) - movement2.y * (start2.x - start1.x)) / (movement2.x * movement1.y - movement2.y * movement1.x);
  float s = (movement1.x * (start2.y - start1.y) - movement1.y * (start2.x - start1.x)) / (movement2.x * movement1.y - movement2.y * movement1.x);
  /*Serial.print(r);
  Serial.print(" ");
  Serial.print(s);
  Serial.print("\n");*/
  if(r >= 0 && r <= 1 && s >= 0 && s <= 1){
    Vector point1 = start1 + r * movement1;
    Vector point2 = start2 + s * movement2;
    if(point1 == point2){
      return r;
    }else{
      return -1;
    }
  }else{
    return -1;
  }
}

void Box::clearBox(TFT& TFTscreen){
  TFTscreen.stroke(0,0,0);
  TFTscreen.fill(0,0,0);
  TFTscreen.rect(position.x, position.y, size.x, size.y);
  TFTscreen.noFill();
}

void Box::drawFields(Box* fields[7][7], TFT& TFTscreen){
  for(int i = 0; i < 7; i++){
    for(int j = 0; j < 7; j++){
      if(fields[i][j] != 0 && fields[i][j]->getActive()){
        fields[i][j]->draw(TFTscreen);
      } 
    }
  }
}

void Box::addLineOfBoxes(int roundScore, Box* fields[7][7], TFT& TFTscreen){
  int xs[7] = {0, 1, 2, 3, 4, 5, 6};

  int number = random(1, 4);
  int start = 0;
  for(int i=0; i <= number;i++){
    int index = random(number, 7);
    int x = xs[index];
    xs[index] = xs[start];
    start++;
    if(i == 0){
      fields[x][0] = new AddBox();
    }else{
      int number = roundScore + 1;
      if(number % 10 == 0){
          number *= 2;
      }
      fields[x][0] = new NumberBox(number);
    }
    fields[x][0]->setPositionByIndices(x, 0);
    fields[x][0]->draw(TFTscreen);
  }
}

bool Box::nextStep(int roundScore, Box* fields[7][7], TFT& TFTscreen){
  for(int y = 6; y >= 0; y--){
    for(int x = 0; x < 7; x++){
      if(y == 6){
        if(fields[x][y] != 0 && fields[x][y]->getActive()){
          return false;
        }
      }else{
        if(fields[x][y] != 0 && fields[x][y]->getActive()){
          fields[x][y]->clearBox(TFTscreen);
          fields[x][y + 1] = fields[x][y];
          fields[x][y + 1]->setPositionByIndices(x,y+1);
          fields[x][y + 1]->draw(TFTscreen);
          fields[x][y] = 0;
        }
      }
    }
  }

  Box::addLineOfBoxes(roundScore, fields, TFTscreen);

  return true;
}
