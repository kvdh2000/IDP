
//All methods
/*
 * setup
 * loop
 * receiveEvent
 */

#include <Wire.h>

const byte slaveID = 2;

void setup()
{
	Wire.begin(slaveID);
	Wire.onReceive(receiveEvent);
	Serial.begin(9600);
	Serial.println("Arduino NANO2 start");
}

void loop()
{
	Serial.println("-------------------------------------------------");
	Serial.println();
	Serial.println("Restart loop");
	Serial.println();

	Serial.println();
}

void receiveEvent(int bytes)
{
	x = Wire.read();

	Serial.print("Read wire at: ");
	Serial.print(x);
}
