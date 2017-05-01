#include <SoftwareSerial.h>
//#include <XBee.h>

int xBeeRX = 2; // Connect Arduino pin 8 to Xbee TX pin
int xBeeTX = 3; // Connect Arduino pin 9 to Xbee RX pin
int rssi = 12; // RSSI digitalPin
int ledPin = 13; //LED
SoftwareSerial xBeeSerial(xBeeRX, xBeeTX);// Define SoftwareSerial xBeeSerial RX/TX pins

char incomingByte, checkByte;
unsigned long rssiDur;

//XBee xbee=XBee();

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  xBeeSerial.begin(115200);
//  xbee.setSerial(xBeeSerial);
  pinMode(rssi, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  rssiDur = pulseIn(rssi, LOW);
  Serial.print(" Strength: ");
  Serial.print(rssiDur);
  Serial.println();
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
}

//  
//  if (xBeeSerial.available()) {
//      incomingByte = xBeeSerial.read();
//    }
//
//    if (incomingByte != checkByte){
////      Serial.print(" Byte: ");
////      Serial.print(incomingByte, HEX);
//      Serial.print(" Strength: ");
//      Serial.print(rssiDur);
//      Serial.println();
//      checkByte = incomingByte;
//      delay(200);
//    }
//////    digitalWrite(ledPin, LOW);
