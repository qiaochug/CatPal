#include <SPI.h>
#include <WiFi101.h>
#define trigPin 6
#define echoPin 3



char ssid[] = "HP-setup-north"; //  your network SSID (name) 
char pass[] = "detkinlabnorth101999";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)


int status = WL_IDLE_STATUS;




//thingspeak server
char server[] = "api.thingspeak.com";


WiFiClient client;


//API key for the Thingspeak ThingHTTP
const String apiKey = "98P47Y7UPHNRQ6V9";


//the number the message should be sent to
const String sendNumber = "2678581272";
static boolean msg_sent = false;


void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  //set up WiFi:
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



void loop()
{
  static boolean lack = false;
  

  long duration, distance;
  digitalWrite(trigPin, LOW);  // start trig at 0
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //The rising edge of trig pulse
  delayMicroseconds(10); // decides duration of trig pulse
  digitalWrite(trigPin, LOW); //falling edge of the trig pulse
  // NOTE: echo pin reads HIGH till it receives the reflected signal
  duration = pulseIn(echoPin, HIGH);  // Reading the duration for which echoPin was HIGH gives        //you the time the sensor receives a reflected signal at the echo pin
  distance = (duration / 2) / 29.1;   //Calculate the distance of the reflecting surface in cm

  Serial.println("Distance reading");
  Serial.println(distance);
  if(distance > 21){ //If reading below this threshold, assume intruder detected
    lack = true;
  }else{
    lack = false;
    msg_sent = false; 
  }


  if(lack && !msg_sent){ //If intruder is detected and an alert message has not been sent yet, send a message
    
    //send the sms
    Serial.println("Sending SMS");
    
    //this function will send the sms
    //the first argument is the number to send to, formatted like this +12345678901
    //the second argument is the body of the text message, which must be within URLEncode()
    sendSMS(sendNumber, URLEncode("Feed me! -- Miao"));
    msg_sent = true;
  }
}



void sendSMS(String number,String message)
{
  // Make a TCP connection to remote host
  if (client.connect(server, 80))
  {


    //should look like this...
    //api.thingspeak.com/apps/thinghttp/send_request?api_key={api key}&number={send to number}&message={text body}


    client.print("GET /apps/thinghttp/send_request?api_key=");
    client.print(apiKey);
    client.print("&number=");
    client.print(number);
    client.print("&message=");
    client.print(message);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  }
  else
  {
    Serial.println(F("Connection failed"));
  } 


  // Check for a response from the server, and route it
  // out the serial port.
  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      Serial.print(c);
    }      
  }
  Serial.println();
  client.stop();
}

String URLEncode(const char* msg)
{
  const char *hex = "0123456789abcdef";
  String encodedMsg = "";


  while (*msg!='\0'){
    if( ('a' <= *msg && *msg <= 'z')
      || ('A' <= *msg && *msg <= 'Z')
      || ('0' <= *msg && *msg <= '9') ) {
      encodedMsg += *msg;
    } 
    else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 15];
    }
    msg++;
  }
  return encodedMsg;
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

