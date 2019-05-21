
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

	//WireSend(1, 0);
	Wire.beginTransmission(1);
  Wire.write(0);
  Wire.endTransmission();
  delay(500);
	//WireSend(1, 1);
  Wire.beginTransmission(1);
  Wire.write(1);
  Wire.endTransmission();
	delay(500);

	Serial.println();
}

void WireSend(int slaveID, int var)
{
	Wire.beginTransmission(slaveID);
	Wire.write(var);
	Wire.endTransmission();
}
