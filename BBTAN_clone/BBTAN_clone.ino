 // include TFT and SPI libraries
#include <SPI.h>
#include <TFT.h>
#include<stdio.h>
#include "EEPROM.h"

#include "AddBox.h"
#include "NumberBox.h"
#include "Vector.h"
#include "Ball.h"
#include "ShootingLine.h"

// pin definition for Arduino UNO
#define cs   8 
#define dc   10 //=rs
#define rst  9
//define Inputpins
#define LEFTBUTTONPIN 4
#define MIDDLEBUTTONPIN 2
#define RIGHTBUTTONPIN 3


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

Box* fields[7][7];
Ball ball;
ShootingLine shootingLine;
int roundScore, highScore;
int lastTime;
int noticeEllapsedTime = 0;
//0 = load saved, 1 = shootingMode, 2 = runningMode, 3 = Gameover, 5 = save screen
int gameState;

void loadGame();
void loadGameFromMemory();
void loadNewGame();
void loadStuff(int positionInMemory, byte* pointer, int length);
void redrawTopScreen();
void drawGameOverScreen();
void drawLoadScreen();
void drawSaveScreen();
void redrawRoundScoreText();
void deactivateAllFields();
void saveGame();
void saveStuff(int positionInMemory, byte* pointer, int length);
void saveHighScore(int positionInMemory);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  /*char test = 0;
  char * test_p = &test;
  byte* test_b_p = (byte*)test_p;
  *test_b_p = EEPROM.read(9);
  Serial.println("Test:");
  Serial.println((int)test);*/
  
  //initialize the library
  TFTscreen.begin();
  //rotate screen
  TFTscreen.setRotation(180);
  TFTscreen.setTextSize(1);

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  //set Pins as Input
  pinMode(LEFTBUTTONPIN, INPUT);
  pinMode(RIGHTBUTTONPIN, INPUT);
  pinMode(MIDDLEBUTTONPIN, INPUT);

  shootingLine = ShootingLine(LEFTBUTTONPIN, RIGHTBUTTONPIN);
  ball = Ball();
  loadGame();
}

void loop() {
  //time measuring
  int time = millis();
  int ellapsedTime = time - lastTime;
  lastTime = time;
  
  //update and draw ----------------------
  if(gameState == 2){
    if(ball.update(ellapsedTime, fields, TFTscreen)){
      //set new shootingLine params
      Vector newStart = ball.getPosition();
      shootingLine.setStart(newStart);
      if(Box::nextStep(roundScore, fields, TFTscreen)){
        //update roundScore
        roundScore++;
        if(roundScore > highScore){
          highScore = roundScore;
        }

        redrawRoundScoreText();
        shootingLine.draw(fields, TFTscreen);
        gameState = 1;
      }else{
        saveHighScore(sizeof(byte));
        drawGameOverScreen();
        gameState = 3;
      }
    }else{
      ball.draw(TFTscreen);
    }
   
  }else if(gameState == 1){
    if(digitalRead(LEFTBUTTONPIN)== HIGH && digitalRead(RIGHTBUTTONPIN)== HIGH){
      drawSaveScreen();
      noticeEllapsedTime = 0;
      gameState = 5;
    }else{
      if(digitalRead(MIDDLEBUTTONPIN)== HIGH){
        //change game state to runningMode
        Vector newMovement = shootingLine.getStop() - shootingLine.getStart();
        ball.setMovement(newMovement);
        gameState = 2;
        shootingLine.clear(fields, TFTscreen);
      }else{
        shootingLine.update(ellapsedTime, fields, TFTscreen);
      }
    }
  }else if(gameState == 3){
    if(digitalRead(MIDDLEBUTTONPIN)== HIGH){
        TFTscreen.background(0, 0, 0);
        deactivateAllFields();
        loadNewGame();
    }
  }else if(gameState == 0){
    if(digitalRead(LEFTBUTTONPIN)== HIGH){
        TFTscreen.background(0, 0, 0);
        loadGameFromMemory();
    }else if(digitalRead(RIGHTBUTTONPIN)== HIGH){
        TFTscreen.background(0, 0, 0);
        loadNewGame();
    }
  }else{
    noticeEllapsedTime += ellapsedTime;
    if(noticeEllapsedTime > 1000){
      int leftSide = digitalRead(LEFTBUTTONPIN);
      int rightSide = digitalRead(RIGHTBUTTONPIN);
      if(leftSide == HIGH || rightSide == HIGH){
        if(leftSide == HIGH){
          saveGame();
          Serial.println("saved");
        }
        TFTscreen.background(0, 0, 0);
        redrawTopScreen();
        shootingLine.draw(fields, TFTscreen);
        gameState = 1;
      }
    }
  }
}

void saveGame(){
  //order: 
  //char checkValue, unsigned short Highscore, unsigned short score, unsigned short hits/damage, unsigned char x-pos-line, unsigned char number of addblocks,unsigned char number of numberblocks, 
  //(addblocks:[usigned char x-pos, usigned char y-pos],....),(numberblocks:[usigned char x-pos, usigned char y-pos, unsigned short number, unsigned short startNumber], ....)
  
  //set checkValue to 42 (saved)
  EEPROM.update(0, (byte)42);
  int positionInMemory = sizeof(byte);

  //Todo something with HIGHSCORE
  saveHighScore(positionInMemory);
  positionInMemory += sizeof(unsigned short);

  //Handle score
  unsigned short saveRoundScore = (unsigned short)roundScore;  
  saveStuff(positionInMemory, (byte*)&saveRoundScore, sizeof(unsigned short));
  positionInMemory += sizeof(unsigned short);

  //Handle hits/damage
  unsigned short saveDamage = (unsigned short)ball.getDamage();  
  saveStuff(positionInMemory, (byte*)&saveDamage, sizeof(unsigned short));
  positionInMemory += sizeof(unsigned short);

  //Handle x-pos of Line  
  unsigned short saveShootingLineX = (unsigned short) shootingLine.getStart().x;
  Serial.println("hier:");
  Serial.println(saveShootingLineX);
  saveStuff(positionInMemory, (byte*)&saveShootingLineX, sizeof(unsigned short));
  positionInMemory += sizeof(unsigned short);
  //go through all fields and convert to a struct
  SaveAddBox* addBoxes[7];
  SaveNumberBox* numberBoxes[32];
  byte numberOfAddBlocks = 0;
  byte numberOfNumberBlocks = 0;

  for(int x = 0; x < 7; x++){
    for(int y = 0; y < 7; y++){
      if(fields[x][y] != 0 && fields[x][y]->getActive()){
        //is a addblock
        if(fields[x][y]->getType() == 'a'){
          addBoxes[numberOfAddBlocks] = new SaveAddBox((byte)x, (byte)y);
          numberOfAddBlocks += 1;             
        }else{
          //convert to numberBox for the Number and startNumber
          NumberBox* numberBox = (NumberBox*)fields[x][y];
          numberBoxes[numberOfNumberBlocks] = new SaveNumberBox((byte)x, (byte)y, (unsigned short)numberBox->getNumber(), (unsigned short)numberBox->getStartNumber());
          numberOfNumberBlocks += 1;   
        }
      }
    }
  }

  //Handle number Of addBlocks
  saveStuff(positionInMemory, (byte*)&numberOfAddBlocks, sizeof(byte));
  positionInMemory += sizeof(byte);
  
  //Handle number Of numberBlocks
  saveStuff(positionInMemory, (byte*)&numberOfNumberBlocks, sizeof(byte));
  positionInMemory += sizeof(byte);
  //Handle add Blocks
  for(int i = 0; i < numberOfAddBlocks; i++){
    //Handle x-position
    saveStuff(positionInMemory, (byte*)&addBoxes[i]->x, sizeof(byte));
    positionInMemory += sizeof(byte);

    //Handle y-position
    saveStuff(positionInMemory, (byte*)&addBoxes[i]->y, sizeof(byte));
    positionInMemory += sizeof(byte);
  }

  //Handle add Blocks
  for(int j = 0; j < numberOfNumberBlocks; j++){
    //Handle x-position
    saveStuff(positionInMemory, (byte*)&numberBoxes[j]->x, sizeof(byte));
    positionInMemory += sizeof(byte);

    //Handle y-position
    saveStuff(positionInMemory, (byte*)&numberBoxes[j]->y, sizeof(byte));
    positionInMemory += sizeof(byte);

    //Handle number
    saveStuff(positionInMemory, (byte*)&numberBoxes[j]->number, sizeof(unsigned short));
    positionInMemory += sizeof(unsigned short);
 
    //Handle startNumber
    saveStuff(positionInMemory, (byte*)&numberBoxes[j]->startNumber, sizeof(unsigned short));
    positionInMemory += sizeof(unsigned short);
  }

}

void saveStuff(int positionInMemory, byte* pointer, int length){
  for(int i = positionInMemory; i < positionInMemory + length; i++){
    EEPROM.update(i, *(pointer++));
  }
}

void saveHighScore(int positionInMemory){
  //only save highscore, if is higher than the last one
  unsigned short newHighScore;
  loadStuff(positionInMemory, (byte*)&newHighScore, sizeof(unsigned short));
  if(newHighScore < highScore){
    newHighScore = (unsigned short)highScore;
    saveStuff(positionInMemory, (byte*)&newHighScore, sizeof(unsigned short));
  }
}

void loadGame(){
  //read checkValue from position 0
  byte checkValue = EEPROM.read(0);
  //if the value is not 42, we dont have to load something(nothing in memory)
  if(checkValue != 42){
    loadNewGame();
    loadHighScore(sizeof(byte));
    if(highScore == 0){
      highScore = 1;
    }
  }else{
    loadHighScore(sizeof(byte));
    gameState = 0;
    drawLoadScreen();
    //loadGameFromMemory();
  }
}

void loadNewGame(){
  gameState = 1;
  roundScore = 1;
  //initial with 0 
  Box::addLineOfBoxes(0, fields, TFTscreen);
  
  Vector pos = Vector(64, 159);
  shootingLine.setStart(pos);
  shootingLine.draw(fields, TFTscreen);
  
  ball.setDamage(1);
  ball.setPosition(pos);

  redrawTopScreen();

  //reset lastTime
  lastTime = millis();
}

void loadGameFromMemory(){
  gameState = 1;
  //start after the checkValue 
  int positionInMemory = sizeof(byte);

  //load HighScore
  positionInMemory += sizeof(unsigned short);

  //load score
  unsigned short savedRoundScore;
  loadStuff(positionInMemory, (byte*)&savedRoundScore, sizeof(unsigned short));
  roundScore = savedRoundScore;
  positionInMemory += sizeof(unsigned short);

  //load damage/hits
  unsigned short savedDamage;
  loadStuff(positionInMemory, (byte*)&savedDamage, sizeof(unsigned short));
  ball.setDamage(savedDamage);

  positionInMemory += sizeof(unsigned short);

  //load ShootingLineX and startPosition of ball
  unsigned short shootingLineX;
  loadStuff(positionInMemory, (byte*)&shootingLineX, sizeof(unsigned short));
  Vector start = Vector(shootingLineX, 159);
  shootingLine.setStart(start);
  shootingLine.draw(fields, TFTscreen);
  ball.setPosition(start);
  positionInMemory += sizeof(unsigned short);
  
  //load number of addblocks
  byte numberOfAddBlocks;
  loadStuff(positionInMemory, (byte*)&numberOfAddBlocks, sizeof(byte));
  positionInMemory += sizeof(byte);

  //load number of numberblocks
  byte numberOfNumberBlocks;
  loadStuff(positionInMemory, (byte*)&numberOfNumberBlocks, sizeof(byte));
  positionInMemory += sizeof(byte);
  //load all addBlocks
  for(int i = 0; i < numberOfAddBlocks + numberOfNumberBlocks; i++){
    //load x
    byte x;
    loadStuff(positionInMemory, (byte*)&x, sizeof(byte));
    
    positionInMemory += sizeof(byte);
    //load y
    byte y;
    loadStuff(positionInMemory, (byte*)&y, sizeof(byte));
    
    positionInMemory += sizeof(byte);
    if(i < numberOfAddBlocks){
      fields[x][y] = new AddBox();
    }else{
      //load number
      unsigned short savedNumber;
      loadStuff(positionInMemory, (byte*)&savedNumber, sizeof(unsigned short));
      
      positionInMemory += sizeof(unsigned short);
      //load startnumber
      unsigned short savedStartNumber;
      loadStuff(positionInMemory, (byte*)&savedStartNumber, sizeof(unsigned short));
      
      positionInMemory += sizeof(unsigned short);

      fields[x][y] = new NumberBox(savedNumber, savedStartNumber);

    }
    fields[x][y]->setPositionByIndices(x,y);
    fields[x][y]->draw(TFTscreen);
  }

  redrawTopScreen();

  //reset lastTime
  lastTime = millis();
}

void loadStuff(int positionInMemory, byte* pointer, int length){
  for(int i = positionInMemory; i < positionInMemory + length; i++){
    *(pointer++) = EEPROM.read(i);
  }
}

void loadHighScore(int positionInMemory){
  unsigned short savedHighScore;
  loadStuff(positionInMemory, (byte*)&savedHighScore, sizeof(unsigned short));
  highScore = savedHighScore;
}

void redrawTopScreen(){
  //draw the top line and Score text
  TFTscreen.stroke(255,255,255);
  TFTscreen.line(0, 8, 127, 8);
  redrawRoundScoreText();
  ball.drawDamage(TFTscreen);
}

void redrawRoundScoreText(){
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.fill(0, 0, 0);
  TFTscreen.rect(0, 0, 65, 8);
  TFTscreen.noFill();
  TFTscreen.stroke(255, 255, 255);
  char str[20];
  sprintf(str, "Runde %d", roundScore); // integer to string
  TFTscreen.text(str, 0, 0);
}

void drawGameOverScreen(){
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(20, 34, 87, 50);
    TFTscreen.noFill();
    TFTscreen.text("Game Over!", 35, 40);
    TFTscreen.text("Druecke die", 29, 55);
    TFTscreen.text("Mittel-Taste!", 25, 65);
}

void drawLoadScreen(){
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(20, 34, 87, 50);
    TFTscreen.noFill();
    TFTscreen.text("Stand laden?", 28, 40);
    TFTscreen.text("<-      ->", 29, 55);
    TFTscreen.text("ja     nein", 27, 65);
}

void drawSaveScreen(){
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(12, 34, 103, 70);
    TFTscreen.noFill();
    TFTscreen.text("Speichern?", 33, 60);
    TFTscreen.text("<-      ->", 32, 75);
    TFTscreen.text("ja     nein", 30, 90);

    byte counter = 1;
    int newHighScore = highScore;
    while(newHighScore/10 != 0){
      newHighScore = newHighScore / 10;
      counter++;
    }
    char str[20];
    sprintf(str, "HighScore: %d", highScore); // integer to string
    TFTscreen.text(str, 20 + (4 - counter) * 2.5, 45);
}

void deactivateAllFields(){
  for(int i = 0; i < 7;i++){
    for(int j = 0; j < 7;j++){
      if(fields[i][j]->getActive() && fields[i][j] != 0){
        fields[i][j]->deactivate();
      }
    }
  }
}
