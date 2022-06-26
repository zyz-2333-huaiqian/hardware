#include <Wire.h>
#include <JY901.h>

void setup() 
{
  Serial.begin(9600);
  JY901.StartIIC();
} 

void loop() 
{
  //print received data. Data was received in serialEvent;
  JY901.GetAngle();
  Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  Serial.println("");
  delay(20);
}