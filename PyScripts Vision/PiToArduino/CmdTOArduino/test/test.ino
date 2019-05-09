#include "DynamixelMotor.h"

#define DIR_PIN 2

const uint8_t servoid1 = 1;
const uint8_t servoid2 = 2;
const uint8_t servoid3 = 3;
const uint8_t servoid4 = 4;
const uint8_t servoid5 = 5;
const uint8_t servoid6 = 6;

int16_t speed1 = 1023;
int16_t speed2 = 512;

const long unsigned int baudrate = 1000000;

HardwareDynamixelInterface interface(Serial1, DIR_PIN);

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
DynamixelMotor motors(interface, BROADCAST_ID);



void setup() {
  interface.begin(baudrate);
  delay(100);

  motors.enableTorque();
  motors.speed(speed2);
  motor1.jointMode(0, 1023);
  motor2.jointMode(124, 897);
  motor3.jointMode(33, 680);
  motor4.jointMode(19, 615);
  motor5.jointMode(0, 595);
  motor6.jointMode(28, 684);

  motor1.goalPosition(512);
  motor2.goalPosition(552);
  motor3.goalPosition(362);
  motor4.goalPosition(359);
  motor5.goalPosition(350);
  motor6.goalPosition(350);
  delay(1000);

}

void loop() {
  //  put your main code here, to run repeatedly:
    motor1.goalPosition(900);
    motor4.goalPosition(200);
    motor5.goalPosition(200);
    motor6.goalPosition(350);
    delay(10000);

    motor1.goalPosition(900);
    motor4.goalPosition(670);
    motor5.goalPosition(595);
    motor6.goalPosition(350);
    delay(10000);
}