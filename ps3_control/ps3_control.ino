#include <Servo.h> 
#include <PS3BT.h>
#include <usbhub.h>

USB Usb;
USBHub Hub1(&Usb);
BTD Btd(&Usb);
PS3BT PS3(&Btd,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

boolean defaultControls = true;

boolean forward = true;
int motorA = 2;
int motorB = 4;
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
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
void loop() {
  Usb.Task();
  
  if(PS3.PS3Connected || PS3.PS3NavigationConnected) {
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
      Serial.print(F("\r\nPitch: "));               
      Serial.print(PS3.getAngle(Pitch));                  
      Serial.print(F("\tRoll: ")); 
      Serial.print(PS3.getAngle(Roll));
    }
    
    if(PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    } 
    else {
      if(PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));              
        defaultControls = !defaultControls;
      }                           
    }
  }
  delay(10);
}
