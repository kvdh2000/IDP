
struct Motor {
  int A;
  int B;
  int PWM;
};

//All methods
/*
   setup
   loop
   readJoy
   turnOff
   convertxy
   drive
   armMovement
   voltMeter
   locationUpdate
   executeSerial
   sendBack
   dmap
*/

#include "DynamixelMotor.h"
#include "Arduino.h"
#include "btLib.h"

//Pin definitions
#define DIR_PIN 2
const Motor dcMotors[5] =
{
  Motor{0, 0, 0},
  Motor{22, 23, 3},
  Motor{24, 25, 4},
  Motor{26, 27, 5},
  Motor{28, 29, 6}
};
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
btLib bluetooth_conn;
int MotorXas;
int MotorYas;
int ArmXas;
int ArmYas;
int CurArmY = 825;
int CurArmX = 0;
const int loc_default = 0;
int loc_update;

//General Variables
const int deadzone_min = 485;
const int deadzone_max = 535;
const int js_neutral = 512;

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
double angle;
double intensity;
int jX = 512;
int jY = 512;

//Variables for Volt Meter
#define AANTAL_METINGEN 10
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
  bluetooth_conn.add_recieve_int("Motor_Xas", js_neutral);
  bluetooth_conn.add_recieve_int("Motor_Yas", js_neutral);
  bluetooth_conn.add_recieve_int("Arm_Xas", js_neutral);
  bluetooth_conn.add_recieve_int("Arm_Yas", js_neutral);
  bluetooth_conn.add_recieve_int("Hand", js_neutral);
  bluetooth_conn.add_recieve_int("Location", loc_default);

  pinMode(LED, OUTPUT);
  pinMode(vuMeter, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);

  for (byte c = 1; c < 5; c++)
  {
    pinMode(dcMotors[c].A, OUTPUT);
    pinMode(dcMotors[c].B, OUTPUT);
    pinMode(dcMotors[c].PWM, OUTPUT);
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
  drive();
  armMovement();
  locationUpdate();

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
  MotorXas = analogRead(X);
  MotorYas = analogRead(Y);

  Serial.println(MotorXas);
  Serial.println(MotorYas);
}

void turnOff()
{
  for (byte c = 1; c < 5; c++)
  {
    digitalWrite(dcMotors[c].A, 0);
    digitalWrite(dcMotors[c].B, 0);
    digitalWrite(dcMotors[c].PWM, 0);
  }
}

void convertxy() //Deciding the angle of the joystick, converting it to a circle input from a square input and deciding the factor for the speed by calculating the distance from the center of the joystick
{
  int x = MotorXas - 512;
  int y = MotorYas - 512;
  angle = -atan2(y, x);
  int halfabigPI = 157;
  int otherthing = abs(int(100 * angle));
  double itmpangle = (otherthing % halfabigPI);
  double tmpangle = itmpangle / 100;
  if (tmpangle >= M_PI * 0.25) {
    tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
  }
  intensity = dmap(sqrt(pow(x, 2) + pow(y, 2)), 0, 512 / cos(tmpangle), 0, 512);
}

void drive() //Everything from making joystick input usable to sending the right signals to the dc motors
{
  convertxy(); //Converting joystick input into usable variables

  //Deadzone
  if (intensity < 50)
  {
    turnOff();
    return;
  }

  //Deciding on which signals to send to both left motors as well as sending them based on both the angle and the required speed
  if (angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
  {
    digitalWrite(dcMotors[1].A, HIGH);
    digitalWrite(dcMotors[1].B, LOW);
    digitalWrite(dcMotors[2].A, HIGH);
    digitalWrite(dcMotors[2].B, LOW);

    if (angle <= M_PI * 0.5 && angle >= 0)
    {
      analogWrite(dcMotors[1].PWM, dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcMotors[2].PWM, dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.5)
    {
      analogWrite(dcMotors[1].PWM, intensity / 515 * dmap(M_PI * 0.75 - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[2].PWM, intensity / 515 * dmap(M_PI * 0.75 - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcMotors[1].PWM, intensity / 515 * dmap(angle + 0.25 * M_PI, 0.0, 0.25 * M_PI, 0.0, 255));
      analogWrite(dcMotors[2].PWM, intensity / 515 * dmap(angle + 0.25 * M_PI, 0.0, 0.25 * M_PI, 0.0, 255));
    }
  }
  else
  {
    digitalWrite(dcMotors[1].A, LOW);
    digitalWrite(dcMotors[1].B, HIGH);
    digitalWrite(dcMotors[2].A, LOW);
    digitalWrite(dcMotors[2].B, HIGH);

    if (angle <= M_PI * -0.5)
    {
      analogWrite(dcMotors[1].PWM, dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcMotors[2].PWM, dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle >= M_PI * 0.75)
    {
      analogWrite(dcMotors[1].PWM, intensity / 515 * dmap(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[2].PWM, intensity / 515 * dmap(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcMotors[1].PWM, intensity / 515 * dmap(abs(angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
      analogWrite(dcMotors[2].PWM, intensity / 515 * dmap(abs(angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
    }
  }

  //Deciding on which signals to send to both right motors as well as sending them based on both the angle and the required speed
  if (angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
  {
    digitalWrite(dcMotors[3].A, LOW);
    digitalWrite(dcMotors[3].B, HIGH);
    digitalWrite(dcMotors[4].A, LOW);
    digitalWrite(dcMotors[4].B, HIGH);

    if (angle >= M_PI * -0.5 && angle <= 0)
    {
      analogWrite(dcMotors[3].PWM, dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcMotors[4].PWM, dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.5)
    {
      analogWrite(dcMotors[3].PWM, intensity / 515 * dmap(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[4].PWM, intensity / 515 * dmap(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcMotors[3].PWM, intensity / 515 * dmap(abs(angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[4].PWM, intensity / 515 * dmap(abs(angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
    }
  }
  else
  {
    digitalWrite(dcMotors[3].A, HIGH);
    digitalWrite(dcMotors[3].B, LOW);
    digitalWrite(dcMotors[4].A, HIGH);
    digitalWrite(dcMotors[4].B, LOW);

    if (angle >= M_PI * 0.5)
    {
      analogWrite(dcMotors[3].PWM, dmap(intensity, 0, 515, 0, 255));
      analogWrite(dcMotors[4].PWM, dmap(intensity, 0, 515, 0, 255));
    }
    else if (angle <= M_PI * -0.75)
    {
      analogWrite(dcMotors[3].PWM, intensity / 515 * dmap(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[4].PWM, intensity / 515 * dmap(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
    }
    else
    {
      analogWrite(dcMotors[3].PWM, intensity / 515 * dmap(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      analogWrite(dcMotors[4].PWM, intensity / 515 * dmap(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
    }
  }
}

void armMovement()
{
  motors.speed(500);

  ArmXas = bluetooth_conn.get_int("Arm_Xas");
  ArmYas = bluetooth_conn.get_int("Arm_Yas");
  int Hand = bluetooth_conn.get_int("Hand");

  if (Hand == 0)
  {
    motor6.goalPosition(600);
  }
  else
  {
    motor6.goalPosition(50);
  }
  if (ArmXas < deadzone_min)
  {
    CurArmY -= 20;
  }
  if (ArmXas > deadzone_max)
  {
    CurArmY += 20;
  }
  if (ArmYas > deadzone_max)
  {
    CurArmX += 2;
  }
  if (ArmYas < deadzone_min)
  {
    CurArmX -= 2;
  }

  CurArmX = constrain(CurArmX, 1, 28);

  B = sqrt(CurArmX * CurArmX + D * D);
  if (B < 26 and B != 0)
  {
    alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
    betar = acos((A * A - B * B + C * C) / (2 * A * C));
    gammar = acos((A * A + B * B - C * C) / (2 * A * B));
    alpha = alphar * 180 / M_PI + 60;
    beta = betar * 180 / M_PI - 20;
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

void locationUpdate() {
  loc_update = bluetooth_conn.get_int("Location");
  Serial.print("Loc:");
  Serial.println(loc_update);
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

double dmap(double input, double fromlow, double fromhigh, double tolow, double tohigh) //Improving the map function to work with doubles
{
  return (input - fromlow) / (fromhigh - fromlow) * (tohigh - tolow) + tolow;
}
