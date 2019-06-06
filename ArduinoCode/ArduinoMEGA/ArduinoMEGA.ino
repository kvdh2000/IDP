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

int dcpins[18] = {3, 22, 23, 4, 24, 25, 5, 26, 27, 6, 28, 29, 7, 30, 31, 8, 32, 33};

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


//Variables for DC motors
String dcinput = "05120512";
double angle;
int intensity;
int jX = 512;
int jY = 512;

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

//  voltMeter();
  readJoy();

  if (dcinput != "")
  {
    drive();
    delay(500);
    dcinput = "";
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
  digitalWrite(M1D, 0);  
  digitalWrite(M2D, 0);
  digitalWrite(M3D, 0);
  digitalWrite(M4D, 0);
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
      double output = map(intensity, 0, 515, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else if (angle >= M_PI * 0.5)
    {
      double output = intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else
    {
      double output = intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
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
      double output = map(intensity, 0, 515, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else if (angle >= M_PI * 0.75)
    {
      double output = intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else
    {
      double output = intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255);
      analogWrite(M1D, output);
      analogWrite(M2D, output);
      Serial.print("pwm = ");
      Serial.println(output);
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
      double output = map(intensity, 0, 515, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else if (angle <= M_PI * -0.5)
    {
      double output = intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else
    {
      double output = intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
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
      double output = map(intensity, 0, 515, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else if (angle <= M_PI * -0.75)
    {
      double output = intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
    else
    {
      double output = intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255);
      analogWrite(M3D, output);
      analogWrite(M4D, output);
      Serial.print("pwm = ");
      Serial.println(output);
    }
  }
}

void tracks()
{

}
