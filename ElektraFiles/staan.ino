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
  // put your setup code here, to run once:
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
  motor8.jointMode(411, 1023);
  motor9.jointMode(0, 1023);
  motor10.jointMode(411, 1023);
  motor11.jointMode(0, 1023);
  motor12.jointMode(411, 1023);
  motor13.jointMode(0, 1023);
  motor14.jointMode(0, 1023);
  motor15.jointMode(411, 1023);
  motor16.jointMode(0, 1023);

  motor7.goalPosition(493);
  motor8.goalPosition(506);
  motor9.goalPosition(506);
  motor10.goalPosition(493);
  motor11.goalPosition(530);
  motor12.goalPosition(506);
  motor15.goalPosition(493);
  motor16.goalPosition(506);
  motor7.goalPosition(493);
  motor8.goalPosition(506);
  motor9.goalPosition(506);
  motor10.goalPosition(493);
  motor11.goalPosition(530);
  motor12.goalPosition(506);
  motor15.goalPosition(493);
  motor16.goalPosition(506);
  delay(1000);

  motor7.goalPosition(128);
  motor8.goalPosition(450);
  motor9.goalPosition(128);
  motor10.goalPosition(450);
  motor11.goalPosition(880);
  motor12.goalPosition(450);
  motor15.goalPosition(450);
  motor16.goalPosition(128);
  motor7.goalPosition(128);
  motor8.goalPosition(450);
  motor9.goalPosition(128);
  motor10.goalPosition(450);
  motor11.goalPosition(880);
  motor12.goalPosition(450);
  motor15.goalPosition(450);
  motor16.goalPosition(128);
  delay(1000);

  motor7.goalPosition(308);
  motor8.goalPosition(622);
  motor9.goalPosition(308);
  motor10.goalPosition(622);
  motor11.goalPosition(703);
  motor12.goalPosition(308);
  motor15.goalPosition(612);
  motor16.goalPosition(308);
  motor7.goalPosition(308);
  motor8.goalPosition(622);
  motor9.goalPosition(308);
  motor10.goalPosition(622);
  motor11.goalPosition(703);
  motor12.goalPosition(308);
  motor15.goalPosition(612);
  motor16.goalPosition(308);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
//  motor7.goalPosition(308);
//  motor8.goalPosition(622);
//  motor9.goalPosition(308);
//  motor10.goalPosition(622);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  motor7.goalPosition(308);
//  motor8.goalPosition(622);
//  motor9.goalPosition(308);
//  motor10.goalPosition(622);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  delay(1000);
//
//  motor7.goalPosition(423);
//  motor8.goalPosition(748);
//  motor9.goalPosition(423);
//  motor10.goalPosition(738);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  motor7.goalPosition(423);
//  motor8.goalPosition(448);
//  motor9.goalPosition(423);
//  motor10.goalPosition(738);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  delay(1000);
//
//  motor7.goalPosition(423);
//  motor8.goalPosition(726);
//  motor9.goalPosition(703);
//  motor10.goalPosition(723);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  motor7.goalPosition(423);
//  motor8.goalPosition(726);
//  motor9.goalPosition(703);
//  motor10.goalPosition(723);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  delay(1000);
//
//  motor7.goalPosition(413);
//  motor8.goalPosition(707);
//  motor9.goalPosition(408);
//  motor10.goalPosition(712);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  motor7.goalPosition(413);
//  motor8.goalPosition(707);
//  motor9.goalPosition(308);
//  motor10.goalPosition(712);
//  motor11.goalPosition(703);
//  motor12.goalPosition(308);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  delay(1000);
//
//  motor7.goalPosition(412);
//  motor8.goalPosition(698);
//  motor9.goalPosition(452);
//  motor10.goalPosition(736);
//  motor11.goalPosition(520);
//  motor12.goalPosition(423);
//  motor15.goalPosition(612);
//  motor16.goalPosition(308);
//  motor7.goalPosition(412);
//  motor8.goalPosition(698);
//  motor9.goalPosition(452);
//  motor10.goalPosition(736);
//  motor11.goalPosition(520);
//  motor12.goalPosition(423);
//  motor15.goalPosition(850);
//  motor16.goalPosition(535);
//  delay(1000);
//
//  motor7.goalPosition(293);
//  motor8.goalPosition(602);
//  motor9.goalPosition(590);
//  motor10.goalPosition(875);
//  motor11.goalPosition(512);
//  motor12.goalPosition(423);
//  motor15.goalPosition(850);
//  motor16.goalPosition(543);
//  motor7.goalPosition(293);
//  motor8.goalPosition(602);
//  motor9.goalPosition(590);
//  motor10.goalPosition(875);
//  motor11.goalPosition(512);
//  motor12.goalPosition(423);
//  motor15.goalPosition(850);
//  motor16.goalPosition(543);
//  delay(1000);
}
