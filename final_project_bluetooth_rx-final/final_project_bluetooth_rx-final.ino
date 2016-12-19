#include <Servo.h>
#include <SoftwareSerial.h>
#include "String.h"
SoftwareSerial BTSerial(2, 3); //TX RX
Servo servo_x;
Servo servo_y;
Servo servo_feed;
int laser = 13;
int x;
int y;
String inString_x = "";
String inString_y = "";
String inString_feed = "";
String inString_mode = "";
String inString_x_prev = "";
String inString_y_prev = "";
String inString_mode_prev = "";
String inString_feed_prev = "";
boolean stringComplete = false;
String totalString = "";
String string_to_use = "";

void setup() {
    BTSerial.begin(9600);
    Serial.begin(9600);
    servo_x.attach(9);
    servo_y.attach(10);
    servo_x.write(90);
    servo_y.write(90);
    delay(2000);
    servo_feed.attach(11);
    pinMode(laser, OUTPUT);
}

void loop() {
  static boolean xRead = false;
  static boolean yRead = false;
  static boolean feedRead = false;
  static boolean modeRead = false;
  static int x = 90;
  static int y = 90;
  boolean skip = false;
  boolean read_data = false;

    while(BTSerial.available())
    {
        char inChar = (char) BTSerial.read();
        totalString += inChar;
        if(inChar == '\n'){
          totalString += '\0';
          string_to_use = totalString;
          totalString  = "";
          BTSerial.flush();//clear bluetooth buffer!
          read_data = true;
         Serial.println(string_to_use);
          break;
        }
      }
      
      for(int i = 0; i < string_to_use.length(); i++){
          if(string_to_use.charAt(i) == 'X'){
            inString_x = "";
            xRead = true;
            yRead = false;
            feedRead = false;
            modeRead = false;
          }else if(string_to_use.charAt(i) == 'Y'){
            inString_y = "";
            yRead = true;
            xRead = false;
            feedRead = false;
            modeRead = false;
          }else if(string_to_use.charAt(i) == 'F'){
            inString_feed = "";
            yRead = false;
            xRead = false;
            feedRead = true;
            modeRead = false;
          }else if(string_to_use.charAt(i) == 'M'){
            inString_mode = "";
            yRead = false;
            xRead = false;
            feedRead = false;
            modeRead = true;
          }
          else if(xRead){
            inString_x += string_to_use.charAt(i);
          }
         
          else if(yRead){
            inString_y += string_to_use.charAt(i);
          }
         
          else if(feedRead){
            inString_feed += string_to_use.charAt(i);
          }
          else if(modeRead){
            inString_mode += string_to_use.charAt(i);
          }}
          
          inString_mode_prev = inString_mode;
          inString_mode = "";
          inString_feed_prev = inString_feed;
         
          inString_feed = "";
          inString_x_prev = inString_x;
          inString_x = "";
          inString_y_prev = inString_y;
          inString_x = "";

       
      
      if(read_data){
          if(inString_mode_prev.toInt() != 1){
            Serial.println("entered manual mode");
          x = inString_x_prev.toInt();
          y = inString_y_prev.toInt();
          //Serial.print("X");
          //Serial.println(x);
          //Serial.print("Y");
          //Serial.print(y);
          servo_x.write(map(x, 0, 1023, 0, 180));
          servo_y.write(map(y, 0, 1023, 0, 180));
          digitalWrite(laser, HIGH);
          delay(500);
          if(inString_feed_prev.toInt() == 1)
          {
            servo_feed.write(20);
            delay(2000);
          }else if (inString_feed_prev.toInt() == 0){
            servo_feed.write(88);
          }
         }
         
          else if(inString_mode_prev.toInt()==1){
            Random();
          }
      } else if(!read_data){
         digitalWrite(laser,LOW);
      }
      //totalString = "";
      //string_to_use = "";
      //inString_x = "";
      //inString_y = "";
      //inString_feed = "";
      //inString_feed = ""; 
      
}

void Random()
{
    servo_x.write(random(0,180));
    servo_y.write(random(80, 140));
    digitalWrite(laser, HIGH);
    int control = random(10000);
    delay(500);

    if(control<2)
    {
        servo_feed.write(20);
        delay(2000);
        servo_feed.write(88);
      }
  }
