#include <SPI.h>
#include <WiFi101.h>



int joy1 = A0;
int joy2 = A1;
int modeButton = 7;
int mode = 0;

int button_val = 0;
int old_button_val = 0;

// Local Network Settings
char ssid[] = "HP-setup-north";  // your network SSID (name)
char pass[] = "detkinlabnorth101999";    // your network password
int keyIndex = 0;             // your network key Index number (needed only for WEP)


int status = WL_IDLE_STATUS;


WiFiServer server(80);


// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String APIKey = "0OGQ3MBK6DMYZLDB";             // enter your channel's Write API Key
const int updateThingSpeakInterval = 5*1000; // 5 second interval at which to update ThingSpeak


// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;


// Initialize Arduino Ethernet Client
WiFiClient client;


void setup() {
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);

   pinMode(modeButton, INPUT);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }


  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);


    // wait 10 seconds for connection:
    delay(1000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // read values from pins and store as strings


          button_val = digitalRead(modeButton);

          if ((button_val == HIGH) && (old_button_val == LOW)){
               mode = 1 - mode;
            }  

          old_button_val = button_val;
          
          int joyval1 = analogRead(joy1);//joystick
          int joyval2 = analogRead(joy2);
          
  String modeString = String(mode);
  String joy1String = String(joyval1);
  String joy2String = String(joyval2);// convert to strings

  
  // Print Update Response to Serial Monitor
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
//  // Disconnect from ThingSpeak
//  if (!client.connected() && lastConnected) {
//    Serial.println("...disconnected");
//    Serial.println();
//    client.stop();
//  }
  // Update ThingSpeak
  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval)) {
    updateThingSpeak("field1=" + modeString + "&field2=" + joy1String +"&field3=" + joy2String);
    Serial.println(modeString + " " + joy1String + " " + joy2String);


  }
  lastConnected = client.connected();
}


void updateThingSpeak(String tsData) {
  if (client.connect(thingSpeakAddress, 80)) {
    client.print("POST /update HTTP/1.1\n"); //Post HTTP request to ThingSpeak
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    lastConnectionTime = millis();


    if (client.connected()) {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
    }
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

