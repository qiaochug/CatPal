#include <SPI.h>
#include <WiFi101.h>


String send_val = "";

char ssid[] = "HP-setup-north";  // your network SSID (name)
char pass[] = "detkinlabnorth101999";    // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
                        
int status = WL_IDLE_STATUS;

int joy1 = A0;
int joy2 = A1;
int modeButton = 9;
int feedButton = 10;
int mode = 0;
int feed = 0;

int button_val = 0;
int old_button_val = 0;

int feed_val = 0;
int old_feed_val = 0;

WiFiServer server(80);


void setup() {
  //Initialize serial and wait for port to open:
  pinMode(modeButton, INPUT);
  pinMode(feedButton, INPUT);
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
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);


    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}




void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          Serial.println("Sending response");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          
          /*Takes the light reading and scales it from 0 to 100. 500 corresponds to the offset from 0.
           * You might need to play around with these values to scale it to suit your sensor
           */

          button_val = digitalRead(modeButton);
          feed_val = digitalRead(feedButton);

          if ((button_val == HIGH) && (old_button_val == LOW)){
               mode = 1 - mode;
            }

          if ((feed_val == HIGH) && (old_feed_val == LOW)){
               feed = 1 - feed;
            }

          old_button_val = button_val;
          old_feed_val = feed_val;
          
          int joyval1 = analogRead(joy1);//joystick
          int joyval2 = analogRead(joy2);
          //Forming a string of joystick values (L/R direction corresponds to X, and U/D direction corresponds to Y)
          send_val = "X"+(String)joyval1+"Y"+(String)joyval2;
          Serial.println(send_val);//Printing on the serial monitor // read light value
          
          //Sending data enclosed in different types of brackets to make it easier for the client to recognize
          client.print("[");
          client.print(mode);
          client.print("]");
          client.print("{");
          client.print(feed);
          client.print("}");
          client.print("*");
          client.print(joyval1);
          client.print("|");
          client.print("(");
          client.print(joyval2);
          client.print(")");
          client.print("<br />");
          
          client.println("</html>");
          feed = 0;
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
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