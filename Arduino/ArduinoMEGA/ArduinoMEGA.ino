
//Functions
/*
   setup
   loop
   getBTValues
   turnOff
   convertxy
   drive
   voltMeter
   locationUpdate
   executeSerial
   sendBack
   dmap
*/

#include "DynamixelMotor.h"
#include "Arduino.h"
#include "btLib.h"
#include "Math.h"

struct Motor
{
	int A;
	int B;
	int PWM;
};

//Pin definitions
#define DIR_PIN 2
#define LED 13
const Motor dcMotors[7] =
		{
				Motor{0, 0, 0},
				Motor{29, 33, 8}, //Motor 1 L voor
				Motor{24, 26, 3}, //Motor 2 L achter
				Motor{32, 30, 7}, //Motor 3 R voor
				Motor{25, 27, 4}, //Motor 4 R achter
};

#define volt A4
#define VUMeter 53

//Variables for Serial
const long unsigned int baudrate = 1000000;
int reg = 0;
String readString;
String read_buffer = "";
String cmd = "";
const int bufferSize = 20;
const char cmd_sep = '|';

//Variables for bt
btLib bluetooth_conn;
int stickOneXas;
int stickOneYas;
int stickTwoXas;
int stickTwoYas;
int CurArmY = 512;
int CurArmX1 = 512;
int CurArmX2 = 512;
int Hand = 0;
bool driveBool = true;
const int loc_default = 0;
int loc_update;
bool equalOn = true;
bool trackerOn = false;
bool danceOn = false;
int ms = 500;

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
DynamixelMotor motors(interface, BROADCAST_ID);
float A = 29;
float B = 0;
float C = 18;
float D = 8;
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
float voltages[10];
float input_volt = 0.0;
float temp = 0.0;
uint8_t voltageIndex = 0;
float sum = 0;
boolean arrayFilled = false;
float averageVoltage = 0.0;

void setup()
{
	Serial.println("Arduino MEGA start");

	Serial.begin(38400);
	Serial1.begin(38400);
	Serial2.begin(38400);

	randomSeed(analogRead(0));
	digitalWrite(VUMeter, HIGH);
	pinMode(VUMeter, OUTPUT);

	bluetooth_conn.begin(&Serial2);
	bluetooth_conn.add_recieve_int("1_Yas", js_neutral);
	bluetooth_conn.add_recieve_int("1_Xas", js_neutral);
	bluetooth_conn.add_recieve_int("2_Yas", js_neutral);
	bluetooth_conn.add_recieve_int("2_Xas", js_neutral);
	bluetooth_conn.add_recieve_int("Hand", js_neutral);
	bluetooth_conn.add_recieve_int("Drive", js_neutral);
	bluetooth_conn.add_recieve_int("Location", js_neutral);
	bluetooth_conn.add_recieve_int("Dance", js_neutral);
	bluetooth_conn.add_recieve_int("Tracker", js_neutral);

	pinMode(LED, OUTPUT);
	pinMode(volt, INPUT);
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
	//  Serial.println("-------------------------------------------------");
	//  Serial.println();
	//  Serial.println("Restart loop");
	//  Serial.println();

	bluetooth_conn.update();

	voltMeter();
	getBTValues();

	if (danceOn)
	{
		dance();
	}

	else if (trackerOn)
	{
		drive();
	}

	else if (driveBool)
	{
		convertxy();
		drive();
	}

	else
	{
		intensity = 0;
		armMovement();
	}

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

void getBTValues()
{
	stickOneXas = bluetooth_conn.get_int("1_Yas");
	stickOneYas = bluetooth_conn.get_int("1_Xas");
	stickTwoXas = bluetooth_conn.get_int("2_Yas");
	stickTwoYas = bluetooth_conn.get_int("2_Xas");
	driveBool = bluetooth_conn.get_int("Drive");
	Hand = bluetooth_conn.get_int("Hand");
	equalOn = bluetooth_conn.get_int("Equal");
	trackerOn = bluetooth_conn.get_int("Tracker");
	danceOn = bluetooth_conn.get_int("Dance");
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
	int x = stickOneXas - 512;
	int y = stickOneYas - 512;
	angle = -atan2(y, x);

	if (angle >= (-M_PI) * 0.5)
	{
		angle = angle - M_PI * 0.5;
	}

	else
	{
		angle = angle + (M_PI * 1.5);
	}

	int halfabigPI = 157;
	int otherthing = abs(int(100 * angle));
	double itmpangle = (otherthing % halfabigPI);
	double tmpangle = itmpangle / 100;

	if (tmpangle >= M_PI * 0.25)
	{
		tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
	}

	intensity = dmap(sqrt(pow(x, 2) + pow(y, 2)), 0, 512 / cos(tmpangle), 0, 512);
}

void drive() //Everything from making joystick input usable to sending the right signals to the dc motors
{
	//Deadzone
	if (intensity < 50)
	{
		turnOff();
		return;
	}

	//pim's if else labyrint
	//Deciding on which signals to send to both left motors as well as sending them based on both the angle and the required speed
	//links
	if (angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
	{
		digitalWrite(dcMotors[1].A, LOW);
		digitalWrite(dcMotors[1].B, HIGH);
		digitalWrite(dcMotors[2].A, LOW);
		digitalWrite(dcMotors[2].B, HIGH);

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
		digitalWrite(dcMotors[1].A, HIGH);
		digitalWrite(dcMotors[1].B, LOW);
		digitalWrite(dcMotors[2].A, HIGH);
		digitalWrite(dcMotors[2].B, LOW);

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
			analogWrite(dcMotors[1].PWM, intensity / 515 * dmap((0 - angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
			analogWrite(dcMotors[2].PWM, intensity / 515 * dmap((0 - angle), 0.25 * M_PI, 0.5 * M_PI, 0, 255));
		}
	}

	//Deciding on which signals to send to both right motors as well as sending them based on both the angle and the required speed
	//rechts
	if (angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
	{
		digitalWrite(dcMotors[3].A, HIGH);
		digitalWrite(dcMotors[3].B, LOW);
		digitalWrite(dcMotors[4].A, HIGH);
		digitalWrite(dcMotors[4].B, LOW);

		if (angle >= M_PI * -0.5 && angle <= 0)
		{
			analogWrite(dcMotors[3].PWM, dmap(intensity, 0, 515, 0, 255));
			analogWrite(dcMotors[4].PWM, dmap(intensity, 0, 515, 0, 255));
		}

		else if (angle <= M_PI * -0.5)
		{
			analogWrite(dcMotors[3].PWM, intensity / 515 * dmap((0 - ((M_PI * -0.75) - angle)), 0, 0.25 * M_PI, 0, 255));
			analogWrite(dcMotors[4].PWM, intensity / 515 * dmap((0 - ((M_PI * -0.75) - angle)), 0, 0.25 * M_PI, 0, 255));
		}

		else
		{
			analogWrite(dcMotors[3].PWM, intensity / 515 * dmap(0 - (angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
			analogWrite(dcMotors[4].PWM, intensity / 515 * dmap(0 - (angle - 0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
		}
	}

	else
	{
		digitalWrite(dcMotors[3].A, LOW);
		digitalWrite(dcMotors[3].B, HIGH);
		digitalWrite(dcMotors[4].A, LOW);
		digitalWrite(dcMotors[4].B, HIGH);

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

void voltMeter()
{
	float factor = 1.165;
	int analogValue = analogRead(volt);

	temp = (analogValue * 5.0) / 1024.0;
	input_volt = temp / factor;
	sum = sum - voltages[voltageIndex];
	voltages[voltageIndex] = input_volt;

	sum = sum + voltages[voltageIndex];

	if (arrayFilled)
	{
		averageVoltage = sum / 10;
	}

	else
	{
		averageVoltage = sum / (voltageIndex + 1);
	}

	voltageIndex++;

	if (voltageIndex == 10)
	{
		voltageIndex = 0;
		arrayFilled = true;
	}

	if (averageVoltage <= 4.08)
	{
		Serial.println("VU:10.25");
	}

	//  Serial.print("U = ");
	//  Serial.print(input_volt);
	//  Serial.println("V");
	//  Serial.print("Average U = ");
	//  Serial.print(averageVoltage);
	//  Serial.println("V");
	//  Serial.println();

	if (averageVoltage <= 4.08)
	{
		Serial.println("VU:12.25");
	}
}

void locationUpdate()
{
	loc_update = bluetooth_conn.get_int("Location");
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

	else if (command == "dancl")
	{
		danceLine();
	}

	else if (command == "blink")
	{
		blinkLed();
	}

	else if (command.indexOf("bpm") != -1)
	{
		int j = command.indexOf("m");
		ms = command.substring(j + 1).toInt();
	}

	else if (command.indexOf("marm") != -1)
	{
		int j = command.indexOf("z");
		float dist = command.substring(j + 1).toFloat();
		moveArm(dist);
	}

	else if (command.indexOf("move") != -1)
	{
		int j = command.indexOf("x");
		String dir = command.substring(j + 1);
		turn(dir);
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
