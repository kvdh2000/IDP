
//All methods
/*
 * setup
 * loop
 * receiveEvent
 */

#include <Wire.h>

#define LED 13

int x;
const byte slaveID = 9;

void setup()
{
  Wire.begin(slaveID);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Arduino NANO2 start");
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop()
{
  Serial.println("-------------------------------------------------");
  Serial.println();
  Serial.println("Restart loop");
  Serial.println();

  if (x == 0)
  {
    digitalWrite(LED,LOW);
  }

  if (x == 1)
  {
    digitalWrite(LED, HIGH);
  }

  delay(100);
  Serial.println();
}

void receiveEvent(int bytes)
{
  Serial.println("Receiving");
  
  x = Wire.read();

  Serial.print("Read wire at: ");
  Serial.println(x);
}
