#include <Servo.h>
Servo feed;
int pir1 = 5;
int motion;
void setup() {
  feed.attach(11);
  pinMode(pir1, INPUT);

}

void loop() {
  motion = digitalRead(pir1);
  delay(100);
  if(motion == HIGH)
  {
      feed.write(20);
    }
    else
    {
        feed.write(90);
      }

}
