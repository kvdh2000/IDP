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


int16_t speed1 = 1023;
int16_t speed2 = 512;

const long unsigned int baudrate = 1000000;

HardwareDynamixelInterface interface(Serial, DIR_PIN);

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
DynamixelMotor motor7(interface, 7);
DynamixelMotor motor8(interface, 8);
DynamixelMotor motor9(interface, 9);
DynamixelMotor motor10(interface, 10);
DynamixelMotor motor11(interface, 11);
DynamixelMotor motor12(interface, 12);
DynamixelMotor motor13(interface, 13);
DynamixelMotor motor14(interface, 14);
DynamixelMotor motor13(interface, 15);
DynamixelMotor motor14(interface, 16);
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


  //digitalWrite(DIR_PIN, HIGH);
  motor1.goalPosition(900);
  delay(100);
  motor2.goalPosition(512);
  delay(100);
  motor3.goalPosition(512);
  delay(100);
  motor4.goalPosition(512);
  delay(100);
  motor5.goalPosition(512);
  delay(100);
  motor6.goalPosition(350);
  delay(100);
  motor7.goalPosition(900);
  delay(100);
  motor8.goalPosition(200);
  delay(100);
  motor9.goalPosition(200);
  delay(100);
  motor10.goalPosition(350);
  delay(100);
  motor11.goalPosition(900);
  delay(100);
  motor12.goalPosition(200);
  delay(100);
  motor13.goalPosition(200);
  delay(100);
  motor14.goalPosition(350);
  delay(100);

  delay(1000);

  motor1.goalPosition(900);
  delay(100);
  motor4.goalPosition(200);
  delay(100);
  motor5.goalPosition(512);
  delay(100);
  motor6.goalPosition(350);
  delay(100);
  motor7.goalPosition(900);
  delay(100);
  motor8.goalPosition(200);
  delay(100);
  motor9.goalPosition(200);
  delay(100);
  motor10.goalPosition(350);
  delay(100);
  motor11.goalPosition(900);
  delay(100);
  motor12.goalPosition(200);
  delay(100);
  motor13.goalPosition(200);
  delay(100);
  motor14.goalPosition(350);
  delay(100);

}

void loop() {
  //  put your main code here, to run repeatedly:
  //digitalWrite(DIR_PIN, HIGH);
  motor1.goalPosition(900);
  delay(100);
  motor2.goalPosition(512);
  delay(100);
  motor3.goalPosition(512);
  delay(100);
  motor4.goalPosition(512);
  delay(100);
  motor5.goalPosition(512);
  delay(100);
  motor6.goalPosition(350);
  delay(100);
  motor7.goalPosition(900);
  delay(100);
  motor8.goalPosition(200);
  delay(100);
  motor9.goalPosition(200);
  delay(100);
  motor10.goalPosition(350);
  delay(100);
  motor11.goalPosition(900);
  delay(100);
  motor12.goalPosition(200);
  delay(100);
  motor13.goalPosition(200);
  delay(100);
  motor14.goalPosition(350);
  delay(100);

  //digitalWrite(DIR_PIN, HIGH);
  motor1.goalPosition(900);
  delay(100);
  motor4.goalPosition(200);
  delay(100);
  motor5.goalPosition(200);
  delay(100);
  motor6.goalPosition(350);
  delay(100);
  motor7.goalPosition(100);
  delay(100);
  motor8.goalPosition(700);
  delay(100);
  motor9.goalPosition(800);
  delay(100);
  motor10.goalPosition(750);
  delay(100);
  motor11.goalPosition(900);
  delay(100);
  motor12.goalPosition(900);
  delay(100);
  motor13.goalPosition(900);
  delay(100);
  motor14.goalPosition(950);
  delay(100);

  //Serial.println("dit is een testje");

}
