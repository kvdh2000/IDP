#include "DynamixelMotor.h"
#include "Arduino.h"
#include "AX12A.h"

#define treu TRUE
#define DIR_PIN 2
//Define variables to be used
int reg = 0;
String readString;
String Text = "No text";

#define DIR_PIN 2

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

String read_buffer = "";
const int buffer_size = 10;
const char cmd_sep = '|';

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

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

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
  motor2.goalPosition(512);
  motor3.goalPosition(512);
  motor4.goalPosition(512);
  motor5.goalPosition(512);
  motor6.goalPosition(350);
  motor7.goalPosition(900);
  motor8.goalPosition(200);
  motor9.goalPosition(200);
  motor10.goalPosition(350);
  motor11.goalPosition(900);
  motor12.goalPosition(200);
  motor13.goalPosition(200);
  motor14.goalPosition(350);

  delay(1000);
}

void loop()
{
  while (!Serial.available())
  {
  } //Wait for data to arrive
  //Serial read section
  while (Serial.available())
  {
    delay(500);
    if (Serial.available() > 0)
    {
      char c = Serial.read(); //Gets one byte from serial buffer
      readString += c;        //Makes the string readString
    }
  }

  if (readString.length() > 0)
  {
    Serial.print("Arduino received: ");
    Serial.println(readString); //See what was received
  }
  getCommand(readString);
  SendBack();
}

void loop()
{
  //Wait for data to arrive
  //Serial read section
  if (Serial.available() && read_buffer.length() < buffer_size)
  {
    char r_char = Serial.read(); // pakt een char van de serial en plakt hem aan de buffer
    read_buffer += r_char;
    //Serial.print(r_char);
  }
  else if ((read_buffer.length() >= buffer_size) || read_buffer.indexOf(cmd_sep) > 0)
  {
    int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
    if (cmd_sep_idx > 0)
    {
      // pakt commando en slicet de buffer
      String cmd = read_buffer.substring(0, cmd_sep_idx);
      read_buffer = read_buffer.substring(cmd_sep_idx + 1);
      execute_command(cmd);
    }
    else if (cmd_sep_idx == 0)
    {
      read_buffer = read_buffer.substring(1);
    }
    else
    {
      Serial.println("recieved garbage clearing buffer");
      read_buffer = "";
    }
  }
  SendBack();
  delay(1); // 1ms delay zodat ie 'm niet flipt
}

void execute_command(String command)
{
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
  else
  {
    Serial.println(str + "unknown " + command);
  }
}

void SendBack()
{
  Serial.print("Arduino sends: ");
  Serial.println(Text);
  Serial.print("\n");
  Serial.flush();
  readString = "";
}
