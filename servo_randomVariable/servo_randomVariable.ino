#include <Servo.h>

Servo servo_x;
Servo servo_y;
Servo servo_feed;
int x;
int y;

void setup()
{
    servo_x.attach(9);
    servo_y.attach(10);
    servo_feed.attach(11);
    Serial.begin(9600);
  }

void loop()
{
    { 
            x = int(random(180));
            y = int(random(30, 90));
            servo_x.write(x);
            servo_y.write(y);
            Serial.println("x: "+ x);
            Serial.println("y: " + y);
            delay(500);
                        
    }     
  }
