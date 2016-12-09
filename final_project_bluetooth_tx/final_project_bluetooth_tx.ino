#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); //TX RX
String send_val = "";
int joyxSensor = 0;
int joyySensor = 1;

int joyx;
int joyy;

int feedSensor = 9;
int modeSensor = 10;

int mode = 0;
int feed = 0;

int mode_val = 0;
int feed_val = 0;
int old_mode_val = 0;
int old_feed_val = 0;

void setup() {
  // put your setup code here, to run once:
  BTSerial.begin(9600);
  Serial.begin(9600);
  pinMode(feedSensor, OUTPUT);
  pinMode(modeSensor, OUTPUT);
}

void loop() {
  feed = 0;
  // put your main code here, to run repeatedly:
  int joyx = analogRead(joyxSensor);
  int joyy = analogRead(joyySensor);
  int feed_val = digitalRead(feedSensor);
  int mode_val = digitalRead(modeSensor);

  if((feed_val == HIGH)&&(old_feed_val == LOW)){
    feed = 1 - feed;
  }

  if((mode_val == HIGH)&&(old_mode_val == LOW)){
    mode = 1 - mode;
  }

  old_feed_val = feed_val;
  old_mode_val = mode_val;
  
  send_val = "X" + (String)joyx + "Y" + (String)joyy + "F" + (String)feed + "M" + (String)mode;
  BTSerial.println(send_val);
  Serial.println(send_val);
  delay(280);
}
