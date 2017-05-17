/*
 * XBee RSSI Status Display
 * 
 * Stanford SSI Rockets IREC 2017
 * R.Wong
 * 
 * rev 1.0 2017-05-01
 * 
 * LCD Display Circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

int xBeeRX = 7; // Connect Arduino pin 8 to Xbee TX pin
int xBeeTX = 8; // Connect Arduino pin 9 to Xbee RX pin
int rssi = 9; // RSSI digitalPin
int ledPin = 13; //LED
int val = 0; // storage value
float alpha = 2.0/(1+15);

SoftwareSerial xBeeSerial(xBeeRX, xBeeTX);// Define SoftwareSerial xBeeSerial RX/TX pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long rssiDur;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  xBeeSerial.begin(115200);
  lcd.begin(16, 2);
  pinMode(rssi, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  rssiDur = pulseIn(rssi, LOW);
  val = (alpha * rssiDur) + ((1.0 - alpha) * val);
  Serial.print(" Strength: ");
  Serial.print(val);
  Serial.println();
  lcd.clear();
  lcd.print(" Strength: ");
  lcd.print(val);
  digitalWrite(ledPin, HIGH);
  delay(100);
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
