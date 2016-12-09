#include <Servo.h>

Servo servo_x;
Servo servo_y;
Servo servo_feed;
int x;
int y;
int led = 8;

void setup()
{
    servo_x.attach(9);
    servo_y.attach(10);
    servo_feed.attach(11);
    Serial.begin(9600);
    pinMode(led, OUTPUT);
  }

void loop()
{
    { 
            x = int(random(180));
            y = int(random(30, 90));
            digitalWrite(led, HIGH);
            servo_x.write(x);
            servo_y.write(y);
            Serial.println(x);
            Serial.println(y);
            delay(500);
                        
    }     
  }
