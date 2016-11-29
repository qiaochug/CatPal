#include <Servo.h>
#include <SPI.h>
#include <WiFi101.h>


char ssid[] = "HP-setup-south"; //  your network SSID (name) 
char pass[] = "detkinlabsouth101999";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)


int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//Check the IP address of the server and fill it out:


byte server[] = {123,123,123,123};


// Initialize the WiFiclient client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


String inString_mode;
String inString_feed;// string for incoming serial data
String inString_joyx;
String inString_joyy;
int stringPos_mode = 0; // string index counter
int stringPos_feed = 0;
int stringPos_joyx = 0;
int stringPos_joyy = 0;
boolean startRead_mode = false; // is reading?
boolean startRead_feed = false;
boolean startRead_joyx = false;
boolean startRead_joyy = false;
Servo servo_x;
Servo servo_y;
Servo servo_feed;
float xval;
float yval;

String location = "/ HTTP/1.1\n"; //To send an HTTP request


void setup() {
  servo_x.attach(9);
  servo_y.attach(10);
  servo_feed.attach(11);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  
  printWifiStatus();
}


  


void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  
  connectAndRead();


  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }
}


void connectAndRead(){


    Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET ");
    client.print(location);
    client.print("Host: 192.168.43.91\n");
    client.print("Connection: close\n");
    client.print("\n\n");
  }


    stringPos_mode = 0;
    stringPos_feed = 0;
    inString_mode = "";
    inString_feed = "";
    inString_joyx = "";
    inString_joyy = "";
    boolean firstComma = true;

    while(true) {
    if(client.available()) {
      char c = client.read();
      if(c == '['){ //Light reading value within square brackets
        startRead_mode = true;
      }else if(startRead_mode){
        if(c !=','){
          inString_mode += c;
        }else{
          startRead_mode = false;
        }
      }
      
      if(c == ',' && firstComma){ //Light reading value within square brackets
        startRead_feed = true;
      }else if(startRead_feed){
        if(c !=']'){
          inString_feed += c;
        }else{
          startRead_feed = false;
          firstComma = false;
        }
      }

      if(c == '{'){ //Temperature reading value within curly brackets
        startRead_joyx = true;
      }else if(startRead_joyx){
        if(c != ','){
        inString_joyx += c;
      }else{
        startRead_joyx = false;
      }
    }

    if(c == ','&& !firstComma){ //Temperature reading value within curly brackets
        startRead_joyy = true;
      }else if(startRead_joyy){
        if(c != ','){
        inString_joyy += c;
      }else{
        startRead_joyy = false;
        break;
      }
    }

    
    }
    }


    while(client.available()){ //Read the remaining HTML bytes
      char d = client.read();
    }


    Serial.print("Mode Reading:");
    Serial.println(inString_mode);
    Serial.print("Feed Reading:");
    Serial.println(inString_feed);
    Serial.print("Joyx Reading:");
    Serial.println(inString_joyx);
    Serial.print("Joyy Reading:");
    Serial.println(inString_joyy);

    if(inString_mode.toInt() == 1)
        {
            servo_x.write(random(180));
            servo_y.write(random(30, 90));}
    else{
      xval = inString_joyx.toInt()/1023*180;
      servo_x.write((int)xval);
      yval = inString_joyy.toInt()/1023*60 + 30;
      servo_y.write((int)yval);
    }

    if(inString_feed.toInt() == 1){
      servo_feed.write(30);
      servo_feed.write(0);
    }


}




void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());


  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

