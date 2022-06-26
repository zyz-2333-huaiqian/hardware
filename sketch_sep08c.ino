#include <Wire.h>
#include <JY901.h>
#include <Servo.h> 
Servo myservo; 
int servo_pin = 8;
//JY901 sensor ;
int16_t ax, ay, az ;
int16_t gx, gy, gz ;
void setup ( )
{
myservo.attach ( servo_pin );
JY901.StartIIC();
Wire.begin ( );
Serial.begin (9600);
//Serial.println (sensor.testConnection ( ) ? "Successfully Connected" : "Connection failed");
delay (1000);
Serial.println ( "Taking Values from the sensor" );
delay (1000);
}
void loop ( ) 
{
JY901.GetAngle();
ax = (float)JY901.stcAngle.Angle[1]/32768*180;
ax = map (ax, -90, 90, 0, 180);
Serial.println (ax);
myservo.write (ax);
delay (200);
}
//JY901.GetAcc();
  //Serial.print("Acc:");Serial.print((float)JY901.stcAcc.a[0]/32768*16);Serial.print(" ");Serial.print((float)JY901.stcAcc.a[1]/32768*16);Serial.print(" ");Serial.println((float)JY901.stcAcc.a[2]/32768*16);
  
