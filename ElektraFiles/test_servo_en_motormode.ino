// Test motor joint mode

#include "DynamixelMotor.h"

#define DIR_PIN 2

// id of the motor
const uint8_t id=1;
// speed, between 0 and 1023
int16_t speed=512;
// communication baudrate
const long unsigned int baudrate = 1000000;

// hardware serial without tristate buffer
// see blink_led example, and adapt to your configuration
HardwareDynamixelInterface interface(Serial, DIR_PIN);

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor41(interface, 41);


void setup()
{ 
  interface.begin(baudrate);
  delay(100);
  
  motor41.enableTorque();
  motor41.wheelMode();
  motor1.enableTorque();  

  motor1.jointMode(204, 820);
  motor1.speed(speed);
}

void loop() 
{
  // go to middle position
  motor1.goalPosition(512);
  motor41.speed(speed);
  delay(500);

  // move 45° CCW
  motor1.goalPosition(666);
  delay(500);

  // go to middle position
  motor1.goalPosition(512);
  delay(500);

  // move 45° CW
  motor1.goalPosition(358);
    motor41.speed(0);
  delay(500);
}
