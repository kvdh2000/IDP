
//All methods
/*
 * setup
 * loop
 * readJoy
 * convertxy
 * drive
 * tracks
 * voltMeter
 * executeSerial
 * sendBack
 */

#include "DynamixelMotor.h"
#include "Arduino.h"

#define LED 13
#define DIR_PIN 2
#define vuMeter A3
#define X A5
#define Y A6

const long unsigned int baudrate = 1000000;

//Variables for Serial
int reg = 0;
String readString;
String read_buffer = "";
String cmd = "";
const int buffer_size = 10;
const char cmd_sep = '|';

//Variables for DC motors
String dcinput = "05120512";
double angle;
int intensity;
int dcpins[18] = {4, 26, 27, 5, 28, 29, 2, 22, 23, 3, 24, 25, 6, 30, 31, 7, 32, 33};
int jX = 512;
int jY = 512;

//Variables for Servos
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

//Variables for Volt Meter
#define AANTAL_METINGEN  10
float input_volt = 0.0;
float temp=0.0;
float factor= 1.165;
float voltages[AANTAL_METINGEN];
uint8_t voltagesIndex = 0;
float som = 0; 
boolean arrayGevuld = false;
float gemiddeldeVoltage = 0.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Arduino MEGA start");
  
  pinMode(LED, OUTPUT);
  pinMode(vuMeter, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);

  motors.enableTorque();
  motors.speed(speed2);
  motor1.jointMode(0, 1023);
  motor2.jointMode(124, 897);
  motor3.jointMode(33, 680);
  motor4.jointMode(19, 615);
  motor5.jointMode(0, 595);
  motor6.jointMode(28, 684);
  //  motor7.jointMode(0, 1023);
  //  motor8.jointMode(0, 1023);
  //  motor9.jointMode(0, 1023);
  //  motor10.jointMode(0, 1023);
  //  motor11.jointMode(0, 1023);
  //  motor12.jointMode(0, 1023);
  //  motor13.jointMode(0, 1023);
  //  motor14.jointMode(0, 1023);
  //  motor15.jointMode(0, 1023);
  //  motor16.jointMode(0, 1023);
  
  //digitalWrite(DIR_PIN, HIGH);
  motor1.goalPosition(825);
  motor2.goalPosition(512);
  motor3.goalPosition(512);
  motor4.goalPosition(512);
  motor5.goalPosition(512);
  motor6.goalPosition(350);
  //  motor7.goalPosition(900);
  //  motor8.goalPosition(200);
  //  motor9.goalPosition(200);
  //  motor10.goalPosition(350);
  //  motor11.goalPosition(900);
  //  motor12.goalPosition(200);
  //  motor13.goalPosition(200);
  //  motor14.goalPosition(350);

  for(byte c = 0; c < 18; c++)
  {
    pinMode(dcpins[c], OUTPUT);
  }
  
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

  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);

  voltMeter();
  readJoy();

  if(dcinput != "")
  {
    drive();
    dcinput = "";
  }

  if (Serial.available() && read_buffer.length() < buffer_size)
  {
    Serial.println("Start Serial");
    char r_char = Serial.read(); // pakt een char van de serial en plakt hem aan de buffer
    read_buffer += r_char;
    //Serial.print(r_char);
  }
  else if ((read_buffer.length() >= buffer_size) || read_buffer.indexOf(cmd_sep) > 0)
  {
    Serial.println("start");
    int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
    if (cmd_sep_idx > 0)
    {
      Serial.println("1");
      // pakt commando en sliced de buffer
      cmd = read_buffer.substring(0, cmd_sep_idx);
      read_buffer = read_buffer.substring(cmd_sep_idx + 1);
      executeSerial(cmd);
    }
    else if (cmd_sep_idx == 0)
    {
      Serial.println("1.1");
      read_buffer = read_buffer.substring(1);
    }
    else
    {
      Serial.println("1.2");
      Serial.println("recieved garbage clearing buffer");
      read_buffer = "";
    }
  }
  
  delay(500);
  Serial.println();
}

void readJoy()
{
  jX = analogRead(X);
  jY = analogRead(Y);
  String Xx = String(jX);
  while(Xx.length() < 4)
  {
    Xx = 0 + Xx;
  }
  String Yy = String(jY);
  while(Yy.length() < 4)
  {
    Yy = 0 + Yy;
  }
  Serial.println(Xx);
  Serial.println(Yy);
  dcinput = Xx + Yy;
}

void convertxy()
{
  double x = dcinput.substring(0,4).toInt() - 512;
  double y = dcinput.substring(4).toInt() - 512;
  angle = atan2(y, x); //in radians
  int bigPI = 157;
  int otherthing = (100 * abs(angle));
  double itmpangle = (otherthing % bigPI);
  double tmpangle = itmpangle / 100;
  if(tmpangle >= M_PI * 0.25){
    tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
  }
  intensity = ((sqrt(pow(x,2) + pow(y,2))) * (512 / (512 / cos(tmpangle))));
}

void drive()
{
  convertxy();
  
  //Links
  if(angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
  {
    digitalWrite(dcpins[1], HIGH);
    digitalWrite(dcpins[2], LOW);
    digitalWrite(dcpins[4], HIGH);
    digitalWrite(dcpins[5], LOW);
    
    if(angle <= M_PI * 0.5 && angle >= 0)
    {
      analogWrite(dcpins[0], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 515, 0, 255));
    }
    else if(angle >= M_PI * 0.5)
    {
      analogWrite(dcpins[0], intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[3], intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[0], intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[3], intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(dcpins[1], LOW);
    digitalWrite(dcpins[2], HIGH);
    digitalWrite(dcpins[4], LOW);
    digitalWrite(dcpins[5], HIGH);
    if(angle <= M_PI * -0.5)
    {
      analogWrite(dcpins[0], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 515, 0, 255));
    }
    else if(angle >= M_PI * 0.75)
    {
      analogWrite(dcpins[0], intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[3], intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[0], intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[3], intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
    }
  }
  
  //Rechts
  if(angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
  {
    digitalWrite(dcpins[7], LOW);
    digitalWrite(dcpins[8], HIGH);
    digitalWrite(dcpins[10], LOW);
    digitalWrite(dcpins[11], HIGH);
    
    if(angle >= M_PI * -0.5 && angle <= 0)
    {
      analogWrite(dcpins[6], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 515, 0, 255));
    }
    else if(angle <= M_PI * -0.5)
    {
      analogWrite(dcpins[6], intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[9], intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[6], intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[9], intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(dcpins[7], HIGH);
    digitalWrite(dcpins[8], LOW);
    digitalWrite(dcpins[10], HIGH);
    digitalWrite(dcpins[11], LOW);
    if(angle >= M_PI * 0.5)
    {
      analogWrite(dcpins[6], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 515, 0, 255));
    }
    else if(angle <= M_PI * -0.75)
    {
      analogWrite(dcpins[6], intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[9], intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcpins[6], intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcpins[9], intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
    }
  }
}

void tracks()
{
  
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
  Serial.println("in execute command");

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
    MoveArm();
  } 
  else if (command == "look")
  {
    LookAround();
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
  Serial.print("\n");
  Serial.flush();
  readString = "";
}
