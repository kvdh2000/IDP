
//All methods
/*
 * setup
 * loop
 * receiveEvent
 */

#include <Wire.h>

#define LED 12

int x;
const byte slaveID = 8;

void setup()
{
  Wire.begin(slaveID);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Arduino NANO1 start");
  
  pinMode(LED, OUTPUT);
}

void loop()
{
  Serial.println("-------------------------------------------------");
  Serial.println();
  Serial.println("Restart loop");
  Serial.println();

  if (x == 0)
  {
    digitalWrite(13,LOW);
  }

  if (x == 1)
  {
    digitalWrite(13, HIGH);
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
