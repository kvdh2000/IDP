//idp code
//motoraansturing en bluetooth ontvanger

//adres van de slave: 98d3:31:fd1781

//liberarys
#include "JohnsSpecialEasyTransfer.h"
#include <DynamixelMotor.h>

JohnsSpecialEasyTransfer bluetooth_conn;


//info
#define motor_out 3
#define motor_linksom 6
#define motor_rechtsom 7

#define TRIGGER_PIN  4
#define ECHO_PIN     5
#define MAX_DISTANCE 400
#define DIR_PIN 2

const long unsigned int baudrate = 1000000;
const int deadzone_min = 485;
const int deadzone_max = 535;
const int js_neutral = 512;


int ARMXas;
int ARMYas;
int CurArmY = 825;
int CurArmX = 0;
int VEHXas;
int VEHYas;
int totaal;
float snelheid = 0;
int richting;               //0 dan is het achteruit en 1 dan is vooruit
int vooruitschaal;

float A = 17;
float B = 0;
float C = 14;
float D = 6.7;
float afstand;
float alpha;
float beta;
float gamma;
float alphar;
float betar;
float gammar;

int16_t speed1 = 1023;
int16_t speed2 = 512;

HardwareDynamixelInterface interface(Serial1, DIR_PIN);
DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
DynamixelMotor motors(interface, BROADCAST_ID);

void setup() {
    Serial.begin(38400);
    Serial1.begin(38400);
    Serial2.begin(38400);
    
    interface.begin(baudrate);  
    motors.enableTorque();
    motors.speed(speed2);
    motor1.jointMode(0, 1023);
    motor2.jointMode(124, 897);
    motor3.jointMode(33, 680);
    motor4.jointMode(19, 615);
    motor5.jointMode(0, 595);
    motor6.jointMode(28, 684);
    
    
    motor1.goalPosition(825);
    motor2.goalPosition(512);
    motor3.goalPosition(512);
    motor4.goalPosition(512);
    motor5.goalPosition(512);
    motor6.goalPosition(350);

    bluetooth_conn.begin(&Serial2);
    bluetooth_conn.add_recieve_int("VEH_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("VEH_Yas", js_neutral);
    bluetooth_conn.add_recieve_int("ARM_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("ARM_Yas", js_neutral);
    bluetooth_conn.add_recieve_int("Klauw", js_neutral);

    pinMode(3, OUTPUT);
    pinMode(motor_linksom, OUTPUT);
    pinMode(motor_rechtsom, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {  
  bluetooth_conn.update();
//  vehicleMovement();
  armMovement();
  
}

void armMovement(){  
  motors.speed(500);
  
  ARMXas = bluetooth_conn.get_int("ARM_Xas");
  ARMYas = bluetooth_conn.get_int("ARM_Yas");
  int Klauw = bluetooth_conn.get_int("Klauw");

  if(Klauw == 0){
    motor6.goalPosition(600);
  }
  else{
    motor6.goalPosition(50);    
  }
  if (ARMXas < deadzone_min) {
      CurArmY -= 20;     
  }
  if(ARMXas > deadzone_max)
  {
      CurArmY += 20;      
  }
  if(ARMYas > deadzone_max)
  {
      CurArmX += 2;         
  }
  if(ARMYas < deadzone_min)
  {
      CurArmX -= 2;        
  }

  CurArmX = constrain(CurArmX, 1, 28);
  
  B=sqrt(CurArmX*CurArmX+D*D);
  if(B < 26 and B != 0){ 
      alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
      betar = acos((A * A - B * B + C * C) / (2 * A * C));
      gammar = acos((A * A + B * B - C * C) / (2 * A * B));
      alpha = alphar * 180 / M_PI+60;
      beta = betar * 180 / M_PI-20;
      gamma = gammar * 180 / M_PI;
      float servohoek1 = map(alpha, 0, 360, 100, 1023);
      float servohoek2 = map(beta, 0, 360, 50, 1023);
      motor2.goalPosition(servohoek1);
      motor4.goalPosition(servohoek2); 
  }
   
  CurArmY = constrain(CurArmY, 0, 1023);
    
  motor1.goalPosition(CurArmY);
}

void vehicleMovement(){
  VEHXas = bluetooth_conn.get_int("VEH_Xas");
  VEHYas = bluetooth_conn.get_int("VEH_Yas");

    Serial2.print("Xas");
    Serial2.println(VEHXas);
    Serial2.print("Yas");
    Serial2.println(VEHYas);

    if (VEHYas > deadzone_min && VEHYas < deadzone_max) {
        snelheid = 0;
        //Serial.println("neutraal");
    }
    if (VEHYas > deadzone_max) {
        richting = 1;
        vooruitschaal = VEHYas - 512;
        snelheid = vooruitschaal;
    }

    if (VEHYas < deadzone_min) {
        richting = 0;
        vooruitschaal = (VEHYas - 512) * (-1);
        snelheid = vooruitschaal;
    }

    Serial.println("------------------------");
    snelheid = map(snelheid, 0, 511, 0, 255);
    
    analogWrite(motor_out, snelheid);

    switch (richting) {
    case false:
        digitalWrite(motor_linksom, 1);
        digitalWrite(motor_rechtsom, 0);
        break;

    case true:
        digitalWrite(motor_linksom, 0);
        digitalWrite(motor_rechtsom, 1);
        break;

    default:
        break;
    }
}
