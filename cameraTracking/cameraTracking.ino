#include <Servo.h>
Servo servo;
int pir1 = 0;
int pir2 = 1;
int pir3 = 2;
int dr1;
int dr2;
int dr3;
boolean d1 = false;
boolean d2 = false;
boolean d3 = false;
int d = -1;//direction to turn(angle to turn)

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(90);
}

void loop() {
  dr1 = analogRead(pir1);
  dr2 = analogRead(pir2);
  dr3 = analogRead(pir3);
  Serial.print("1:");
 Serial.println(dr1);
 Serial.print("2:");
 Serial.println(dr2);
 Serial.print("3:");
Serial.println(dr3);  
  
  if(dr1 > 500 && d1 == false){
    d1 = true;
    d2 = false;
    d3 = false;
    d = 30;
  }
  if(dr2 > 500 && d2 == false){
    d1 = false;
    d2 = true;
    d3 = false;
    d = 90;
  }
  if(dr3 > 500 && d3 == false){
    d1 = false;
    d2 = false;
    d3 = true;
    d = 150;
  }
  Serial.println(d);
  if(d != -1){
  servo.write(d);}
  delay(2000);
}
