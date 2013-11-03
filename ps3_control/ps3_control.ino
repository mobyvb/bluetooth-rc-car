#include <Servo.h> 
#include <PS3BT.h>
#include <usbhub.h>

USB Usb;
USBHub Hub1(&Usb);
BTD Btd(&Usb);
PS3BT PS3(&Btd);

boolean defaultControls = true;

int success = 31;
int waiting = 35;
int error = 39;

boolean forward = true;
int motorA = 2;
int motorB = 4;
int motorPWM = 6;
int motorSpeed;

Servo steering;
int rightBound = 50;
int leftBound = 110;
int turnAngle = (rightBound+leftBound)/2;

void setup() {
  Serial.begin(115200);
  
  steering.attach(3);
  
  while (!Serial);
  if (Usb.Init() == -1) {
    digitalWrite(error, HIGH);
    digitalWrite(success, LOW);
    digitalWrite(waiting, LOW);
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  digitalWrite(error, LOW);
  digitalWrite(success, LOW);
  digitalWrite(waiting, HIGH);
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
void loop() {
  Usb.Task();
  
  if(PS3.PS3Connected || PS3.PS3NavigationConnected) {
    digitalWrite(error, LOW);
    digitalWrite(success, HIGH);
    digitalWrite(waiting, LOW);

    if(defaultControls) {
      if(PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117) {
        turnAngle = map(PS3.getAnalogHat(LeftHatX), 0, 255, leftBound, rightBound);   
      }
      else {
        turnAngle = (rightBound+leftBound)/2;
      }
  
      if(PS3.getAnalogButton(L2)) {
        motorSpeed = PS3.getAnalogButton(L2);
        forward = false;
      }
      else if(PS3.getAnalogButton(R2)) {
        motorSpeed = PS3.getAnalogButton(R2);
        forward = true;
      }
      else {
        motorSpeed = 0;
      }
    }
    
    else {
      int pitch = PS3.getAngle(Pitch);
      if(pitch > 185) {
        if(pitch > 225) pitch = 230;
        motorSpeed = map(pitch, 185, 230, 0, 255);
        forward = true;
      }
      else if(pitch < 175) {
        if(pitch < 130) pitch = 130;
        motorSpeed = map(pitch, 175, 130, 0, 255);
        forward = false;
      }
      else {
        motorSpeed = 0;
      }
      
      int roll = PS3.getAngle(Roll);
      if(roll < 125) roll = 125;
      else if(roll > 225) roll = 225;
      turnAngle = map(roll, 125, 225, leftBound, rightBound);
    }
    
    if(PS3.getButtonClick(PS)) {
      PS3.disconnect();
      digitalWrite(error, LOW);
      digitalWrite(success, LOW);
      digitalWrite(waiting, LOW);
    } 
    else {
      if(PS3.getButtonClick(START)) {            
        defaultControls = !defaultControls;
      }                           
    }
  }
  
  if(motorSpeed > 200) motorSpeed = 200;
  
  if(forward) {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, HIGH);
  }
  else {
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
  }
  analogWrite(motorPWM, motorSpeed);
  
  steering.write(turnAngle);
  
  Serial.print(F("\t\nForward: "));
  Serial.print(forward);
  Serial.print(F("\tMotor speed: "));
  Serial.print(motorSpeed);
  Serial.print(F("\tTurn angle: "));
  Serial.print(turnAngle);
  delay(10);
}
