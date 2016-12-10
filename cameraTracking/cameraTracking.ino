#include <Servo.h>
Servo servo;
int pir1 = 10;
int pir2 = 11;
int pir3 = 12;
int dr1;
int dr2;
int dr3;
boolean d1 = false;
boolean d3 = false;
int d = -1;//direction to turn(angle to turn)

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(90);
}

void loop() {
  dr1 = digitalRead(pir1);
  //dr2 = digitalRead(pir2);
  dr3 = digitalRead(pir3);
  Serial.print("1:");
 Serial.println(dr1);
 //Serial.print("2:");
 //Serial.println(dr2);
 Serial.print("3:");
Serial.println(dr3);  
  
  if(dr1 == 1 && d1 == false){
    d1 = true;
    d3 = false;
    d = 30;
  }

  if(dr3 == 1  && d3 == false){
    d1 = false;
    d3 = true;
    d = 150;
  }
  
  Serial.println(d);
  if(d != -1){
  servo.write(d);
  delay(500);}
  
}
