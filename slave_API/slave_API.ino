#include <SPI.h>
#include <WiFi101.h>
#include<TextFinder.h>


char ssid[] = "HP-setup-north"; //  your network SSID (name) 
char pass[] = "detkinlabnorth101999";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)


int status = WL_IDLE_STATUS;


char server[] = "api.thingspeak.com";
String readAPIKey = "GGPGMTAA387QAW2E";


WiFiClient client;
TextFinder  finder( client ); 


char anaRead1[10];
char anaRead2[10];
char anaRead3[10];




void setup() {
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
    int connFlag = 0;
    int fieldFlag1 = 0;
    int fieldFlag2 = 0;
    int fieldFlag3 = 0;


      if (client.connect(server, 80)) { 
    //If connection was made, issue HTTP Request 
    Serial.println("Connect to thing Speak..."); //
    client.println("GET /channels/190178/feed.xml?results=10 HTTP/1.1"); //Use your channel number instead of "127550". Check your url
    client.print("X-THINGSPEAKAPIKEY: "+readAPIKey+"\n");
    client.println("HOST: api.thingspeak.com ");
    client.println("Connection: close");
    client.println();
    Serial.println("Connected...");
    connFlag = 1;
    
  }


    while (client.connected() && connFlag == 1) {
    //Parsing the file that we have
    //String matching using the special library
   //adjust buffer size as needed 
   if ( (finder.getString("<field1>" ,"</field1>",anaRead1,5)!=0) ) {
     fieldFlag1 = fieldFlag1 + 1;
     if(fieldFlag1 > 1) { 
     Serial.println();
     Serial.print("mode:  ");
     Serial.println(anaRead1);
     }
   }  
   //adjust buffer size as needed 
   if ( (finder.getString("<field2>" ,"</field2>",anaRead2,5)!=0)) {
     fieldFlag2 = fieldFlag2 + 1;
     if(fieldFlag2 > 1) {
     Serial.println();
     Serial.print("Joy_x:  ");
     Serial.println(anaRead2);
     }
   }   
   if ( (finder.getString("<field3>" ,"</field3>",anaRead3,5)!=0)) {
     fieldFlag3 = fieldFlag3 + 1;
     if(fieldFlag3 > 1) {
     Serial.println();
     Serial.print("Joy_y:  ");
     Serial.println(anaRead3);
     }
   }    
  Serial.println();
  // END XML
  }
   Serial.print("Done");
   connFlag = 0; //formatting sake
   fieldFlag1 = 0; //used to format output - don't print field names - via the xml file to get an idea 
   fieldFlag2 = 0;
   fieldFlag3 = 0;


  client.stop();
  client.flush();
  delay(1000); //refresh rate should be moderated. Else tries refreshing at clock speed
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
