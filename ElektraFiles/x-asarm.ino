#include <arduino.h>
#include <DynamixelMotor.h>
#include <NewPing.h>
#include <math.h>

#define TRIGGER_PIN  4
#define ECHO_PIN     5
#define MAX_DISTANCE 400
#define DIR_PIN 2

const uint8_t servoid1 = 1;
const uint8_t servoid2 = 2;
const uint8_t servoid3 = 3;
const uint8_t servoid4 = 4;
const uint8_t servoid5 = 5;
const uint8_t servoid6 = 6;

float afstand;
float alpha;
float beta;
float beta1;
float gamma;
float alphar;
float betar;
float gammar;
float tot;

float servohoek1;
float servohoek2;

float A = 17;
float B = 0;
float C = 14;
float D = 6.7;

int16_t distance = 0;
int16_t speed = 100;

const long unsigned int baudrate = 1000000;

HardwareDynamixelInterface interface(Serial1, DIR_PIN);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
DynamixelMotor motors(interface, BROADCAST_ID);

void setup() {
  Serial.begin(115200);
  interface.begin(baudrate);

  motors.enableTorque();
  motors.speed(speed);
  motor1.jointMode(0, 1023);
  motor2.jointMode(0, 1023);
  motor3.jointMode(0, 1023);
  motor4.jointMode(0, 1023);
  motor5.jointMode(0, 595);
  motor6.jointMode(28, 684);

  motor1.goalPosition(512);
  motor2.goalPosition(512);
  motor3.goalPosition(512);
  motor4.goalPosition(512);
  motor5.goalPosition(350);
  motor6.goalPosition(350);
  delay(4000);

  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
  afstand = 25;
  //afstand = sonar.ping_cm()+3;
  B=sqrt(afstand*afstand+D*D);

   if (B < 30 and B != 0) {
    alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
    betar = acos((A * A - B * B + C * C) / (2 * A * C));
    gammar = acos((A * A + B * B - C * C) / (2 * A * B));
    alpha = alphar * 180 / M_PI+50;
    beta = betar * 180 / M_PI-20;
    gamma = gammar * 180 / M_PI;
    servohoek1 = map(alpha, 0, 360, 1023, 0);
    servohoek2 = map(beta, 0, 360, 1023, 0);
    Serial.println(alpha);
    Serial.println(beta);
    Serial.println(gamma);
    motor2.goalPosition(servohoek1);
    motor4.goalPosition(servohoek2);
    delay(200);;
  }
  else {
    motor1.goalPosition(512);
    motor2.goalPosition(512);
    motor3.goalPosition(512);
    motor4.goalPosition(512);
    motor5.goalPosition(350);
    motor6.goalPosition(350);
  }
}

void loop() {
//  Serial.print("Ping: ");
//  Serial.print(sonar.ping_cm());
//  Serial.println("cm");
//  B = sonar.ping_cm() + 4;
//
//  if (B < 30 and B != 0) {
//    alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
//    betar = acos((A * A - B * B + C * C) / (2 * A * C));
//    gammar = acos((A * A + B * B - C * C) / (2 * A * B));
//    alpha = alphar * 180 / M_PI;
//    beta = betar * 180 / M_PI;
//    gamma = gammar * 180 / M_PI;
//    beta1=beta/2;
//    servohoek1 = map(alpha, 0, 300, 1230, 0);
//    servohoek2 = map(beta1, 0, 300, 0, 1023);
//    Serial.println(alpha);
//    Serial.println(beta);
//    Serial.println(gamma);
//    //if (0 < alpha < 90 and 90 < beta < 180) {
//    motor2.goalPosition(servohoek1);
//    motor3.goalPosition(servohoek2);
//    motor4.goalPosition(servohoek2);
//    delay(200);
//    //      motor2.goalPosition(512);
//    //      motor3.goalPosition(362);
//  }
//  else {
//    motor1.goalPosition(512);
//    motor2.goalPosition(512);
//    motor3.goalPosition(512);
//    motor4.goalPosition(512);
//    motor5.goalPosition(350);
//    motor6.goalPosition(350);
//  }

}
