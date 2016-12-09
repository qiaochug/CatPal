#include <SoftwareSerial.h>
SoftwareSerial BluetoothSerial(2, 3); // 2- TX, 3 - RX

void setup(void)
{
Serial.begin(38400);

BluetoothSerial.begin(38400); /* The AT mode runs at 38400 */

Serial.println("Ready.");
}

void loop(void)
{
while(BluetoothSerial.available()){
Serial.write(BluetoothSerial.read());}

while(Serial.available())
BluetoothSerial.write(Serial.read());
}

