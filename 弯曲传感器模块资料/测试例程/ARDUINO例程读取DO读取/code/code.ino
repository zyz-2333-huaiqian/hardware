/******************************************
 * 淘宝    ：https://ilovemcu.taobao.com
******************************************/

#define SENSOR 2
int KEY_NUM = 0;
int count = 0;
int state = 0;

void setup() 
{ 
  pinMode(SENSOR,INPUT);
  Serial.begin(9600); // setup serial 
}

void loop() 
{ 
  scanSensor();
  if(KEY_NUM == 1)
  {
    KEY_NUM = 0;
  	Serial.println("bend!");
  	Serial.print("count =");
  	Serial.println(count);
  	count++;

    
    if(state == 2)
      state = 0;
  }
  
}

void scanSensor()
{
  if(digitalRead(SENSOR) == HIGH)
  {
    delay(10);
    if(digitalRead(SENSOR) == HIGH) 
    {
      while(digitalRead(SENSOR) == HIGH);
      KEY_NUM = 1;
    } 
  }
}



