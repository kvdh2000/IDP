
//All methods
/*
 * setup
 * loop
 * WireSend
 */
 
#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Arduino UNO 1 start");
}

void loop()
{
  Serial.println("-------------------------------------------------");
  Serial.println();
  Serial.println("Restart loop");
  Serial.println();

  WireSend(8, 0);
  delay(500);
  WireSend(8, 1);
  delay(500);

  Serial.println();
}

void WireSend(int slaveID, int var)
{
  println("Wire send command");
  Wire.beginTransmission(slaveID);
  Wire.write(var);
  Wire.endTransmission();
}
