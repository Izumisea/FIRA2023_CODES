#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial btSerial(2, 3); // RX | TX
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};
PacketData data;

void setup()
{
  btSerial.begin(38400);   
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

 
}

//This function is used to map 0-1023 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick.
//Jotstick values range from 0-1023. But its center value is not always 511. It is little different.
//So we need to add some deadband to center value. in our case 500-530. Any value in this deadband range is mapped to center 127.
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);
  }
  else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void loop()
{
   lcd.setCursor(2,0);   //26
  lcd.print("PK-SLAYER 2.0");
  lcd.setCursor(2,1);  
  lcd.print("ANIMO SPEAR");
  data.lxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.lyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A1), false);
  data.rxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);
  data.ryAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A3), false);

  String dataString;
  dataString = dataString 
               + data.lxAxisValue + "," 
               + data.lyAxisValue + ","  
               + data.rxAxisValue + ","  
               + data.ryAxisValue + "\n";
  
  btSerial.print(dataString);
  delay(10);
}