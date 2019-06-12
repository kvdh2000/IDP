
//All methods
/*
 * setup
 * loop
 * readJoy
 * convertxy
 * turnOff
 * drive
 * armMovement
 * voltMeter
 * executeSerial
 * sendBack
 * dmap
 */

#include "DynamixelMotor.h"
#include "Arduino.h"
#include "JohnsSpecialEasyTransfer.h"

//Pin definitions
const int dcpins[12] = {22, 23, 3, 24, 25, 4, 26, 27, 5, 28, 29, 6};
#define DIR_PIN 2
#define LED 13
#define vuMeter A3
#define X A5
#define Y A6

//Variables for Serial
const long unsigned int baudrate = 1000000;
int reg = 0;
String readString;
String read_buffer = "";
String cmd = "";
const int bufferSize = 10;
const char cmd_sep = '|';

//Variables for bt
JohnsSpecialEasyTransfer bluetooth_conn;
int MotorXas;
int MotorYas;
int ArmXas;
int ArmYas;
int CurArmY = 825;
int CurArmX = 0;

//General Variables
const int deadzone_min = 485;
const int deadzone_max = 535;

//Variables for Servos
HardwareDynamixelInterface interface(Serial1, DIR_PIN);
DynamixelMotor motor1(interface, 1);
DynamixelMotor motor2(interface, 2);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor4(interface, 4);
DynamixelMotor motor5(interface, 5);
DynamixelMotor motor6(interface, 6);
//DynamixelMotor motor7(interface, 7);
//DynamixelMotor motor8(interface, 8);
//DynamixelMotor motor9(interface, 9);
//DynamixelMotor motor10(interface, 10);
//DynamixelMotor motor11(interface, 11);
//DynamixelMotor motor12(interface, 12);
//DynamixelMotor motor13(interface, 13);
//DynamixelMotor motor14(interface, 14);
//DynamixelMotor motor15(interface, 15);
//DynamixelMotor motor16(interface, 16);
DynamixelMotor motors(interface, BROADCAST_ID);
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

//Variables for DC motors
String dcinput = "05120512";
double angle;
int intensity;
int jX = 512;
int jY = 512;

//Variables for Volt Meter
#define AANTAL_METINGEN  10
float voltages[AANTAL_METINGEN];
float input_volt = 0.0;
float temp = 0.0;
float factor = 1.165;
uint8_t voltagesIndex = 0;
float som = 0;
boolean arrayGevuld = false;
float gemiddeldeVoltage = 0.0;

void setup()
{
  Serial.begin(38400);
  Serial1.begin(38400);
  Serial2.begin(38400);
  Serial.println("Arduino MEGA start");

  bluetooth_conn.begin(&Serial2);
  bluetooth_conn.add_recieve_int("Motor_Xas", 512);
  bluetooth_conn.add_recieve_int("Motor_Yas", 512);
  bluetooth_conn.add_recieve_int("Arm_Xas", 512);
  bluetooth_conn.add_recieve_int("Arm_Yas", 512);
  bluetooth_conn.add_recieve_int("Hand", 512);
  
  pinMode(LED, OUTPUT);
  pinMode(vuMeter, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);

  for(byte c = 0; c < 12; c++){
    pinMode(c, OUTPUT);
  }

  initServos();

  for (uint8_t i = 0; i < 10; i++)
  {
    voltages[i] = 0;
  }
}

void loop()
{
  Serial.println("-------------------------------------------------");
  Serial.println();
  Serial.println("Restart loop");
  Serial.println();

  bluetooth_conn.update();
  
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);

  voltMeter();
  readJoy();

  if (dcinput != "")
  {
    drive();
    dcinput = "";
  }

  armMovement();

  if (Serial.available() && read_buffer.length() < bufferSize)
  {
    Serial.println("Start Serial");
    char r_char = Serial.read(); // pakt een char van de serial en plakt hem aan de buffer
    read_buffer += r_char;
  }
  else if ((read_buffer.length() >= bufferSize) || read_buffer.indexOf(cmd_sep) > 0)
  {
    int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
    if (cmd_sep_idx > 0)
    {
      cmd = read_buffer.substring(0, cmd_sep_idx);
      read_buffer = read_buffer.substring(cmd_sep_idx + 1);
      executeSerial(cmd);
    }
    else if (cmd_sep_idx == 0)
    {
      read_buffer = read_buffer.substring(1);
    }
    else
    {
      read_buffer = "";
    }
  }
}

void readJoy()
{
  jX = analogRead(X);
  jY = analogRead(Y);
  String Xx = String(jX);
  while (Xx.length() < 4)
  {
    Xx = 0 + Xx;
  }
  String Yy = String(jY);
  while (Yy.length() < 4)
  {
    Yy = 0 + Yy;
  }
  Serial.println(Xx);
  Serial.println(Yy);
  dcinput = Xx + Yy;
}

void turnOff()
{
  for(byte c = 0; c < 12; c++){
    digitalWrite(c, 0);
  }
}

void drive()  //Everything from making joystick input usable to sending the right signals to the dc motors
{
  convertxy();  //Converting joystick input into usable variables

  //Deadzone within which the motors will not receive a signal
  if (deadzone_min < jX < deadzone_max && deadzone_min < jY < deadzone_max)
  {
    turnOff();
    return;
  }

  //Deciding on which signals to send to both left motors as well as sending them based on both the angle and the required speed
  if (angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
  {
    digitalWrite(dcpins[0], HIGH);
    digitalWrite(dcpins[1], LOW);
    digitalWrite(dcpins[3], HIGH);
    digitalWrite(dcpins[4], LOW);

    if (angle <= M_PI * 0.5 && angle >= 0)
    {
      analogWrite(dcpins[2], dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[5], dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.5)
    {
      analogWrite(dcpins[2], double(intensity) / 515 * dmap(M_PI * 0.75 - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[5], double(intensity) / 515 * dmap(M_PI * 0.75 - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[2], intensity / 515.0 * dmap(angle + 0.25 * M_PI, 0.0, 0.25 * M_PI, 0.0, 255));
      analogWrite(dcpins[5], intensity / 515.0 * dmap(angle + 0.25 * M_PI, 0.0, 0.25 * M_PI, 0.0, 255));
    }
  }
  else
  {
    digitalWrite(dcpins[0], LOW);
    digitalWrite(dcpins[1], HIGH);
    digitalWrite(dcpins[3], LOW);
    digitalWrite(dcpins[4], HIGH);
    if (angle <= M_PI * -0.5)
    {
      analogWrite(dcpins[2], dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[5], dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.75)
    {
      analogWrite(dcpins[2], double(intensity) / 515 * dmap(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[5], double(intensity) / 515 * dmap(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[2], double(intensity) / 515 * dmap(abs(angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
      analogWrite(dcpins[5], double(intensity) / 515 * dmap(abs(angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
    }
  }

  //Deciding on which signals to send to both right motors as well as sending them based on both the angle and the required speed
  if (angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
  {
    digitalWrite(dcpins[6], LOW);
    digitalWrite(dcpins[7], HIGH);
    digitalWrite(dcpins[9], LOW);
    digitalWrite(dcpins[10], HIGH);

    if (angle >= M_PI * -0.5 && angle <= 0)
    {
      analogWrite(dcpins[8], dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[11], dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.5)
    {
      analogWrite(dcpins[8], double(intensity) / 515 * dmap(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[11], double(intensity) / 515 * dmap(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[8], double(intensity) / 515 * dmap(abs(angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[11], double(intensity) / 515 * dmap(abs(angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(dcpins[6], HIGH);
    digitalWrite(dcpins[7], LOW);
    digitalWrite(dcpins[9], HIGH);
    digitalWrite(dcpins[10], LOW);
    
    if (angle >= M_PI * 0.5)
    {
      analogWrite(dcpins[8], dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[11], dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.75)
    {
      analogWrite(dcpins[8], double(intensity) / 515 * dmap(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[11], double(intensity) / 515 * dmap(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[8], double(intensity) / 515 * dmap(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[11], double(intensity) / 515 * dmap(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
    }
  }
}

void convertxy()  //Deciding the angle of the joystick, converting it to a circle input from a square input and deciding the factor for the speed by calculating the distance from the center of the joystick
{
  int x = MotorXas - 512;
  int y = MotorYas - 512;
  angle = -atan2(y, x);
  int bigPI = 157;
  int otherthing = (100 * abs(angle));
  double itmpangle = (otherthing % bigPI);
  double tmpangle = itmpangle / 100;
  if (tmpangle >= M_PI * 0.25) {
    tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
  }
  intensity = ((sqrt(pow(x, 2) + pow(y, 2))) * (512 / (512 / cos(tmpangle))));
}

double dmap(double input, double fromlow, double fromhigh, double tolow, double tohigh) //Improving the map function to work with doubles
{
  return (input - fromlow) / (fromhigh - fromlow) * (tohigh - tolow) + tolow;
}

void armMovement(){  
  motors.speed(500);
  
  ArmXas = bluetooth_conn.get_int("Arm_Xas");
  ArmYas = bluetooth_conn.get_int("Arm_Yas");
  int Hand = bluetooth_conn.get_int("Hand");

  if(Hand == 0){
    motor6.goalPosition(600);
  }
  else{
    motor6.goalPosition(50);    
  }
  if (ArmXas < deadzone_min) {
      CurArmY -= 20;     
  }
  if(ArmXas > deadzone_max)
  {
      CurArmY += 20;      
  }
  if(ArmYas > deadzone_max)
  {
      CurArmX += 2;         
  }
  if(ArmYas < deadzone_min)
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

void voltMeter()
{
  int analogvalue = analogRead(vuMeter);
  temp = (analogvalue * 5.0) / 1024.0;
  input_volt = temp / factor;
  som = som - voltages[voltagesIndex];
  voltages[voltagesIndex] = input_volt;

  som = som + voltages[voltagesIndex];

  if (arrayGevuld)
  {
    gemiddeldeVoltage = som / AANTAL_METINGEN;
  }

  else
  {
    gemiddeldeVoltage = som / (voltagesIndex + 1);
  }

  voltagesIndex = voltagesIndex + 1;

  if (voltagesIndex == AANTAL_METINGEN)
  {
    voltagesIndex = 0;
    arrayGevuld = true;
  }

  Serial.print("U = ");
  Serial.print(input_volt);
  Serial.println("V");
  Serial.print("Average U = ");
  Serial.print(gemiddeldeVoltage);
  Serial.println("V");
  Serial.println();
}

void executeSerial(String command)
{
  Serial.print("Execute command: ");
  Serial.println(command);

  //Command can be max 5 characters long
  String str = "command: ";
  if (command == "forw" || command == "back" || command == "left" || command == "right")
  {
    moveRobot(command);
  }
  else if (command == "dance")
  {
    dance();
  }
  else if (command == "dancl")
  {
    danceLine();
  }
  else if (command == "blink")
  {
    blinkLed();
  }
  else if (command == "marm")
  {
    moveArm();
  }
  else if (command == "look")
  {
    lookAround();
  }
  else
  {
    Serial.println(str + "unknown " + command);
  }
}

void sendBack(String Text)
{
  Serial.print("Arduino sends: ");
  Serial.println(Text);
  Serial.flush();
  readString = "";
}
