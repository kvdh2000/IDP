
//All methods
/*
 * setup
 * loop
 * readJoy
 * convertxy
 * turnOff
 * drive
 * tracks
 * voltMeter
 * executeSerial
 * sendBack
 */

//#include "DynamixelMotor.h"
#include "Arduino.h"

//Pin definitions
#define DIR_PIN 2
#define M1D 3
#define M2D 4
#define M3D 5
#define M4D 6
#define LED 13
#define M1A 22
#define M1B 23
#define M2A 24
#define M2B 25
#define M3A 26
#define M3B 27
#define M4A 28
#define M4B 29
#define vuMeter A3
#define X A5
#define Y A6

//Variables for Serial
const long unsigned int baudrate = 1000000;
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
  Serial.begin(9600);
  Serial.println("Arduino MEGA start");
  
  pinMode(LED, OUTPUT);
  pinMode(vuMeter, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(M1D, OUTPUT);
  pinMode(M1D, OUTPUT);
  pinMode(M1D, OUTPUT);
  pinMode(M1D, OUTPUT);
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(M3A, OUTPUT);
  pinMode(M3B, OUTPUT);
  pinMode(M4A, OUTPUT);
  pinMode(M4B, OUTPUT);

//  initServos()

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
  delay(500);
  Serial.println(angle);
  Serial.println(intensity);
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);

//  voltMeter();
  readJoy();

  if (dcinput != "")
  {
    drive();
    dcinput = "";
  }
//
//  if (Serial.available() && read_buffer.length() < buffer_size)
//  {
//    Serial.println("Start Serial");
//    char r_char = Serial.read(); // pakt een char van de serial en plakt hem aan de buffer
//    read_buffer += r_char;
//  }
//  else if ((read_buffer.length() >= buffer_size) || read_buffer.indexOf(cmd_sep) > 0)
//  {
//    int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
//    if (cmd_sep_idx > 0)
//    {
//      cmd = read_buffer.substring(0, cmd_sep_idx);
//      read_buffer = read_buffer.substring(cmd_sep_idx + 1);
//      executeSerial(cmd);
//    }
//    else if (cmd_sep_idx == 0)
//    {
//      read_buffer = read_buffer.substring(1);
//    }
//    else
//    {
//      read_buffer = "";
//    }
//  }
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

void convertxy()
{
  double x = dcinput.substring(0, 4).toInt() - 512;
  double y = dcinput.substring(4).toInt() - 512;
  angle = atan2(y, x); //in radians
  int bigPI = 157;
  int otherthing = (100 * abs(angle));
  double itmpangle = (otherthing % bigPI);
  double tmpangle = itmpangle / 100;
  if (tmpangle >= M_PI * 0.25) {
    tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
  }
  intensity = ((sqrt(pow(x, 2) + pow(y, 2))) * (512 / (512 / cos(tmpangle))));
}

void turnOff()
{
  analogWrite(M1D, 0);  
  analogWrite(M2D, 0);
  analogWrite(M3D, 0);
  analogWrite(M4D, 0);
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
  digitalWrite(M3A, LOW);
  digitalWrite(M3B, LOW);
  digitalWrite(M4A, LOW);
  digitalWrite(M4B, LOW);
}

void drive()
{
  convertxy();

  //Deadzone
  if (jX < 612 && jX > 412 && jY < 612 && jY > 412)
  {
    turnOff();
    return;
  }

  //Links
  if (angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
  {
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);

    if (angle <= M_PI * 0.5 && angle >= 0)
    {
      analogWrite(M1D, map(intensity, 0, 515, 0, 255));
      analogWrite(M2D, map(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.5)
    {
      analogWrite(M1D, intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(M2D, intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(M1D, intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(M2D, intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
    if (angle <= M_PI * -0.5)
    {
      analogWrite(M1D, map(intensity, 0, 515, 0, 255));
      analogWrite(M2D, map(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.75)
    {
      analogWrite(M1D, intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      analogWrite(M2D, intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(M1D, intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
      analogWrite(M2D, intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
    }
  }

  //Rechts
  if (angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
  {
    digitalWrite(M3A, LOW);
    digitalWrite(M3B, HIGH);
    digitalWrite(M4A, LOW);
    digitalWrite(M4B, HIGH);

    if (angle >= M_PI * -0.5 && angle <= 0)
    {
      analogWrite(M3D, map(intensity, 0, 515, 0, 255));
      analogWrite(M4D, map(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.5)
    {
      analogWrite(M3D, intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(M4D, intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(M3D, intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(M4D, intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(M3A, HIGH);
    digitalWrite(M3B, LOW);
    digitalWrite(M4A, HIGH);
    digitalWrite(M4B, LOW);
    
    if (angle >= M_PI * 0.5)
    {
      analogWrite(M3D, map(intensity, 0, 515, 0, 255));
      analogWrite(M4D, map(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.75)
    {
      analogWrite(M3D, intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(M4D, intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(M3D, intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(M4D, intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
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

//void executeSerial(String command)
//{
//  Serial.print("Execute command: ");
//  Serial.println(command);
//
//  //Command can be max 5 characters long
//  String str = "command: ";
//  if (command == "forw" || command == "back" || command == "left" || command == "right")
//  {
//    moveRobot(command);
//  }
//  else if (command == "dance")
//  {
//    dance();
//  }
//  else if (command == "dancl")
//  {
//    danceLine();
//  }
//  else if (command == "blink")
//  {
//    blinkLed();
//  }
//  else if (command == "marm")
//  {
//    moveArm();
//  }
//  else if (command == "look")
//  {
//    lookAround();
//  }
//  else
//  {
//    Serial.println(str + "unknown " + command);
//  }
//}
//
//void sendBack(String Text)
//{
//  Serial.print("Arduino sends: ");
//  Serial.println(Text);
//  Serial.flush();
//  readString = "";
//}
