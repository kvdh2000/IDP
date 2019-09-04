#include "DynamixelMotor.h"
#include "Arduino.h"
#include "AX12A.h"

#define treu TRUE
#define DIR_PIN 2

int reg = 0;

const uint8_t servoid1 = 1;
const uint8_t servoid2 = 2;
const uint8_t servoid3 = 3;
const uint8_t servoid4 = 4;
const uint8_t servoid5 = 5;
const uint8_t servoid6 = 6;
const uint8_t servoid7 = 7;
const uint8_t servoid8 = 8;
const uint8_t servoid9 = 9;
const uint8_t servoid10 = 10;
const uint8_t servoid11 = 11;
const uint8_t servoid12 = 12;
const uint8_t servoid13 = 13;
const uint8_t servoid14 = 14;
const uint8_t servoid15 = 15;
const uint8_t servoid16 = 16;

int servo7 = 0;
int servo8 = 0;
int servo9 = 0;
int servo10 = 0;
int servo11 = 0;
int servo12 = 0;
int servo15 = 0;
int servo16 = 0;

int16_t speed1 = 1023;
int16_t speed2 = 512;

const long unsigned int baudrate = 1000000;

HardwareDynamixelInterface interface(Serial, DIR_PIN);

//arm
DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
//rechtsvoor
DynamixelMotor motor7(interface, 7);
DynamixelMotor motor8(interface, 8);
//linksvoor
DynamixelMotor motor9(interface, 9);
DynamixelMotor motor10(interface, 10);
//rechtsvoor
DynamixelMotor motor11(interface, 11);
DynamixelMotor motor12(interface, 12);
//nvt
DynamixelMotor motor13(interface, 13);
//extra
DynamixelMotor motor14(interface, 14);
//linksachter
DynamixelMotor motor15(interface, 15);
DynamixelMotor motor16(interface, 16);
//naar alle servos
DynamixelMotor motors(interface, BROADCAST_ID);

void setup() {
  //Serial.begin(9600);
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
  motor7.jointMode(0, 1023);
  motor8.jointMode(0, 1023);
  motor9.jointMode(0, 1023);
  motor10.jointMode(0, 1023);
  motor11.jointMode(0, 1023);
  motor12.jointMode(0, 1023);
  motor13.jointMode(0, 1023);
  motor14.jointMode(0, 1023);
  motor15.jointMode(0, 1023);
  motor16.jointMode(0, 1023);


}

void loop() {
  servo7=servo9=servo11=servo15=900;
  servo8=servo10=servo12=servo16=200;
  
  motor7.goalPosition(servo7);
  delay(1);
  motor8.goalPosition(servo8);
  motor9.goalPosition(servo9);
  motor10.goalPosition(servo10);
  motor11.goalPosition(servo11);
  motor12.goalPosition(servo12);
  motor15.goalPosition(servo15);
  motor16.goalPosition(servo16);

  servo7=servo9=servo11=servo15=200;
  servo8=servo10=servo12=servo16=900;
  
  motor7.goalPosition(servo7);
  motor8.goalPosition(servo8);
  motor9.goalPosition(servo9);
  motor10.goalPosition(servo10);
  motor11.goalPosition(servo11);
  motor12.goalPosition(servo12);
  motor15.goalPosition(servo15);
  motor16.goalPosition(servo16);
}
