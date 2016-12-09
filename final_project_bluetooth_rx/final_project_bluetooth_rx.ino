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
    servo_x.attach(9);
    servo_y.attach(10);
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
          read_data = true;
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
          }
          else if(xRead){
            inString_x += string_to_use.charAt(i);
          }
          else if(string_to_use.charAt(i) == 'Y'){
            inString_y = "";
            yRead = true;
            xRead = false;
            feedRead = false;
            modeRead = false;
          }
          else if(yRead){
            inString_y += string_to_use.charAt(i);
          }
          else if(string_to_use.charAt(i) == 'F'){
            inString_feed = "";
            yRead = false;
            xRead = false;
            feedRead = true;
            modeRead = false;
          }
          else if(feedRead){
            inString_feed += string_to_use.charAt(i);
          }
          else if(string_to_use.charAt(i) == 'M'){
            inString_mode = "";
            yRead = false;
            xRead = false;
            feedRead = false;
            modeRead = true;
          }
          else if(modeRead){
            inString_mode += string_to_use.charAt(i);
          }
         }

       
      
      if(read_data){
          if(inString_mode.toInt() != 1){
               x = inString_x.toInt();
          y = inString_y.toInt();
          servo_x.write(map(x, 0, 1023, 0, 180));
          servo_y.write(map(y, 0, 1023, 30, 90));
          digitalWrite(laser, HIGH);
          delay(500);
          if(inString_feed.toInt() == 1)
          {
            servo_feed.write(20);
            delay(2000);
          }
         }
         
          else if(inString_mode.toInt()==1){
            Random();
          }
      } else if(!read_data){
         digitalWrite(laser,LOW):
      }
      totalString = "";
      string_to_use = "";
      inString_x = "";
      inString_y = "";
      inString_feed = "";
      inString_feed = "";
      
}

void Random()
{
    servo_x.write(random(0,180));
    servo_y.write(random(30,90));
    digitalWrite(laser, HIGH);
    int control = random(1000);
    delay(500);

    if(control<2)
    {
        servo_feed.write(20);
        delay(2000);
      }
  }
