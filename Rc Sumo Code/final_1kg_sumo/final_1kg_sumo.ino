#include<SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};
PacketData data;

//Right motor
int enableRightMotor=5; 
int rightMotorPin1=6;
int rightMotorPin2=7;

//Left motor
int enableLeftMotor=10;
int leftMotorPin1=9;
int leftMotorPin2=8;

unsigned long lastRecvTime = 0;

int botton = 0;
int value = 0;        

void setup()
{
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  rotateMotor(0, 0);
  
  btSerial.begin(38400);  
}

void loop()
{
  String dataString;
  if (btSerial.available())
  {
    dataString = btSerial.readStringUntil('\n');
    sscanf(dataString.c_str(), "%d,%d,%d,%d", &data.lxAxisValue, &data.lyAxisValue, &data.rxAxisValue, &data.ryAxisValue);
    int throttle = map(data.lyAxisValue, 254, 0, -255, 255); //Left stick  - y axis - forward/backward car movement
    int steering = map(data.rxAxisValue, 0, 254, -255, 255); //Right stick - x axis - left/right car movement 
    int motorDirection = 1;
    int botton = data.ryAxisValue;

    
    if (throttle < 0)       //Move car backward
    {
      motorDirection = -1;    
    }
  
    int rightMotorSpeed, leftMotorSpeed;
    rightMotorSpeed =  abs(throttle) - steering;
    leftMotorSpeed =  abs(throttle) + steering;
    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  
    rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection); 
    lastRecvTime = millis();      
  }
  else
  {
    unsigned long now = millis();
    if ( now - lastRecvTime > 1000 )       //Signal lost after 1 second. Reset the motor to stop
    {
      rotateMotor(0, 0);   
   }
 }      
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  
  if (rightMotorSpeed < 0 || botton == 1)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
                
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);    
    value = 1;  
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0 || botton == 1)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);     
    value = 2; 
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }  

  if (value == 1 && leftMotorSpeed == 0){
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH); 
    leftMotorSpeed = 255;
    value = 0;
  }
  if (value == 2 && rightMotorSpeed == 0){
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH); 
    rightMotorSpeed = 255;
    value = 0;
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}