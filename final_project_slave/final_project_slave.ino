#include <Servo.h>
#include <WiFi101.h>
#include<SPI.h>

//servo program

char ssid[] = "HP-setup-north";
char pass[] = "detkinlabnorth101999";

int keyIndex = 0;
int status = WL_IDLE_STATUS;

byte server[] = {192,168,1,146};

WiFiClient client;

String inString_x;
String inString_y; //incoming serial data for joystick
String inString_mode; //incoming serial data for button auto/man toggle
String inString_feed;
int stringPos_x = 0;
int stringPos_y = 0;
int stringPos_mode = 0; //string index count
int pos_x = 90;
int pos_y = 90; //servo position
boolean startRead_x = false;
boolean startRead_y = false;
boolean startRead_feed = false;
boolean startRead_button = false; //reading?
int x = 90;
int y = 90;
int control = 0; // a random variable that will be used to control the feeding when in auto mode

Servo servo_x;
Servo servo_y;
Servo servo_feed;

String location = "/ HTTP/1.1\n";

void setup()
{
    servo_x.attach(9);
    servo_y.attach(10);
    servo_feed.attach(11);

    Serial.begin(9600);
    while (!Serial)
    {
      ; //wait for serial port to connect
      }

     //check the presence of shilde:
     if(WiFi.status() == WL_NO_SHIELD)
     {
        Serial.println("WiFi shield not present");
        while(true);
      }

      //attempt to connect to Wifi network
      while (status != WL_CONNECTED)
      {
          Serial.print("Attempting to connect to SSID: ");
          Serial.println(ssid);
          status = WiFi.begin(ssid, pass);
          delay(10000);
        }
        Serial.println("Connected to wifi");

        printWifiStatus();
        
  }

  void loop()
  {
      //if there are incoming bytes available from the server, read them and print them:

      connectAndRead();

      //if the server disconnected, stop client
      if(!client.connected())
      {
          Serial.println();
          Serial.println("Disconnecting from server.");
          client.stop();
        }
    }

void connectAndRead()
{
    Serial.println("\nStarting connection to server...");

    //if you get a connection, report back via serial:
    if(client.connect(server, 80))
    {
        Serial.println("connected to server");
        client.print("GET ");
        client.print(location);
        client.print("Host: 192.168.43.91\n");
        client.print("Connection: clost\n");
        client.print("\n\n");
      }

      stringPos_x = 0;
      stringPos_y = 0;
      stringPos_mode = 0;
      inString_x = "";
      inString_y = "";
      inString_mode = "";
      inString_feed = "";

      while(true)
      {
          if(client.available())
          {
              //Serial.println("I am in in this if statement\n");
              char c = client.read();
            //  Serial.println("client avai");
              if(c == '[') //mode reading
                {
                  startRead_button = true;
        
                }
              else if(startRead_button)
              {
                  if(c != ']')
                  {
                      inString_mode += c;
                    }
                   else
                   {  
                      startRead_button = false;
                     // Serial.println(inString_mode);
                    }
                }
               if(c == '{') //joystick pos 
               {
                  startRead_feed = true;
                }
               else if(startRead_feed)
               {
                  if(c != '}')
                   { inString_feed += c;}
                  else
                   {
                      startRead_feed = false;
                     // Serial.println(inString_feed);
                    }
                }
                if(c == '*') //add a * after ,
                {
                    startRead_x = true; 
                 }
                else if(startRead_x)
                {
                  if(c != '|')
                  {inString_x += c;}
                  else
                  {
                      startRead_x = false;
                     // Serial.println(inString_x);
                    } 
                 }

                 if(c == '(') //add a * after ,
                {
                    startRead_y = true; 
                 }
                else if(startRead_y)
                {
                  if(c != ')')
                  {inString_y += c;}
                  else
                  {
                      startRead_y = false;
                      //Serial.println(inString_y);
                      break;
                    } 
                 }
                 
            }
        }

        while(client.available())//read the remaining HTML bytes
        {
            char d = client.read();
            //Serial.println(d);
          }
        //Serial.print("mode: ");
        Serial.println(inString_mode);
        //Serial.print("feed: ");
        Serial.println(inString_feed);
        //Serial.print("x: ");
        Serial.println(inString_x);
        //Serial.print("y: ");
        Serial.println(inString_y);

        /*if(inString_mode.toInt() == 1)
        {
            servo_x.write(random(180));
            servo_y.write(random(30, 90));
           // control = random(0,6000);
            delay(100);
            
         /*   if(control < 2) //feeder is controlled using a random variable, this will feed approx every 5 minutes
            {
                servo_feed.write(30);
                delay(1000);
              }
          }

        if(inString_mode.toInt() == 0)
        {


              pos_x = map(inString_x.toInt(), 0, 1023, 0, 180);
              servo_x.write(pos_x);
              pos_y = map(inString_y.toInt(), 0, 1023, 30, 60);
              servo_y.write(pos_y);

              delay(100);
          }*/
          delay(500);
  }

void printWifiStatus()
{
    //print the SSID of the network you are attached to 
    //Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    //print your WiFi shield's IP address
    IPAddress ip = WiFi.localIP();
    //Serial.print("IP Address: ");
    //Serial.println(ip);

    //print the received signal strength:
    long rssi = WiFi.RSSI();
    //Serial.print("Signal strength (RSSI): ");
    //Serial.print(rssi);
    //Serial.println(" dBm");
  }




