#include <SoftwareSerial.h>
#include "enes100.h"
#define SensorPin 0


SoftwareSerial mySerial(8, 9); //RX is pin 8, TX is pin 9
enes100::RfClient<SoftwareSerial> rf(&mySerial); 
enes100::Marker marker;
int markerNumber=100;

const int mot1 = 1;
const int mot2 = 2;
const int mot3 = 3;
const int mot4 = 4;
const int leftDir = 12;
const int rightDir = 13;
const int finalX = 2200;
const int finalY = 1400;
const int coordMargin = 20;
const float thetaMargin = .1;
float thetaDesired;
unsigned long int avgValue;
float b;
int buf[10],temp;

int nextX;
int nextY;
int mode;

void setup() 
{
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(0,INPUT);
  pinMode(8, INPUT); //RX for RF
  pinMode(9, OUTPUT); //TX for RF
  rf.resetServer();
  rf.sendMessage("\nTeam Drop the Base is Connected\n"); //sent to mission control
  Serial.println("Team Drop the Base is Connected"); //sent to student's serial 
  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  pinMode(mot3, OUTPUT);
  pinMode(mot4, OUTPUT);
  pinMode(leftDir, OUTPUT);
  pinMode(rightDir, OUTPUT);
  mode = 1;
  thetaDesired = 0;
  
  calculateInitialRoute();
}

void loop()
{
  int something = mode;
  switch(something)
  {
    case 1:
      goToInitialPoint();
      break;
    case 2:
      break;
      
  }
}

void calculateInitialRoute()
{
  nextX = marker.x;
  nextY = finalY;
}

void goToInitialPoint()
{
  if(marker.theta - thetaDesired < thetaMargin)
      {
        turnLeft();
      }
      else if(marker.theta - thetaDesired > thetaMargin)
      {
        turnRight();
      }
      else if(marker.y - nextY < coordMargin)
      {
        moveForward;
      }
      else if(marker.y - nextY > coordMargin)
      {
        moveBackward;
      }
      else
      {
        mode = 2;
      }
}

void turnLeft()
{
  digitalWrite(leftDir, LOW);
  digitalWrite(rightDir, HIGH);
}

void turnRight()
{
  digitalWrite(leftDir, HIGH);
  digitalWrite(rightDir, LOW);
}

void moveForward()
{
  digitalWrite(leftDir, HIGH);
  digitalWrite(rightDir, HIGH);
}

void moveBackward()
{
  digitalWrite(leftDir, LOW);
  digitalWrite(rightDir, LOW);
}

void stopLocomotion()
{
  analogWrite(mot1, 0);
  analogWrite(mot2, 0);
  analogWrite(mot3, 0);
  analogWrite(mot4, 0);
}

void getPH(){
  for(int i=0; i<10; i++){
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for(int i= 0; i<9; i++){
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue = 0;
  for(int i=2; i<8; i++)
    avgValue+=buf[i];
  float phValue = (float)avgValue*5.0/1024/6;
  phValue = 3.5*phValue;
  Serial.print("    ph:");
  Serial.print(phValue,2);
  Serial.print(" ");
  delay(500);
}

