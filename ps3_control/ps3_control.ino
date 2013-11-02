#include <PS3BT.h>
#include <usbhub.h>

USB Usb;
USBHub Hub1(&Usb);
BTD Btd(&Usb);
PS3BT PS3(&Btd,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

boolean printAngle;

void setup() {
  Serial.begin(115200);
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
    if(PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: ")); 
      Serial.print(PS3.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: ")); 
      Serial.print(PS3.getAnalogHat(LeftHatY));
      if(!PS3.PS3NavigationConnected) {
        Serial.print(F("\tRightHatX: ")); 
        Serial.print(PS3.getAnalogHat(RightHatX));
        Serial.print(F("\tRightHatY: ")); 
        Serial.print(PS3.getAnalogHat(RightHatY));
      }      
    }

    if(PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      Serial.print(F("\r\nL2: ")); 
      Serial.print(PS3.getAnalogButton(L2));
      if(!PS3.PS3NavigationConnected) {
        Serial.print(F("\tR2: ")); 
        Serial.print(PS3.getAnalogButton(R2));
      }
    }
    if(PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    } 
    else {
      if(PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));              
        printAngle = !printAngle;
      }                           
    }
    if(printAngle) {
      Serial.print(F("\r\nPitch: "));               
      Serial.print(PS3.getAngle(Pitch));                  
      Serial.print(F("\tRoll: ")); 
      Serial.print(PS3.getAngle(Roll));
    }
  }
}
