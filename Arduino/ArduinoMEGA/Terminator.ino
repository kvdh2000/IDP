
int16_t speed1 = 1023;
int16_t speed2 = 512;

typedef uint8_t uint8;

const uint8 NUM_LEDS = 70;

struct
{
	int beat_delta_ms = 0;
	int bpm = 0;
	unsigned long timer_start = millis();
	int step = 0;
} led_vars;

void initServos()
{
	interface.begin(baudrate);
	motors.enableTorque();
	motors.speed(speed2);

	motor1.jointMode(0, 1023);
	motor2.jointMode(150, 900);
	motor3.jointMode(150, 900);
	motor4.jointMode(20, 1023);
	motor5.jointMode(0, 1023);
	motor6.jointMode(28, 684);

	//  digitalWrite(DIR_PIN, HIGH);

	motor1.goalPosition(512);
	motor2.goalPosition(512);
	motor3.goalPosition(getMotor3Value(512));
	motor4.goalPosition(512);
	motor5.goalPosition(512);
	motor6.goalPosition(460);
}

int getMotor3Value(int mtr2Val)
{
	return 1023 - mtr2Val;
}

void armMovement()
{
	motors.speed(300);

	if (Hand == 0)
	{
		motor6.goalPosition(462);
	}

	else
	{
		motor6.goalPosition(0);
	}

	if (stickOneXas < deadzone_min)
	{
		CurArmX1 -= 20;
	}

	if (stickOneXas > deadzone_max)
	{
		CurArmX1 += 20;
	}

	if (stickOneYas > deadzone_max)
	{
		CurArmY += 20;
	}

	if (stickOneYas < deadzone_min)
	{
		CurArmY -= 20;
	}

	if (stickTwoXas > deadzone_max)
	{
		CurArmX2 -= 20;
	}

	if (stickTwoXas < deadzone_min)
	{
		CurArmX2 += 20;
	}

	CurArmX1 = constrain(CurArmX1, 260, 512);
	CurArmX2 = constrain(CurArmX2, 50, 900);
	CurArmY = constrain(CurArmY, 300, 750);

	//  motor1.goalPosition(CurArmY);
	motor2.goalPosition(CurArmX1);
	motor3.goalPosition(getMotor3Value(CurArmX1));
	motor4.goalPosition(CurArmX2);
	motor1.goalPosition(512);

	//grond tot 1 = 18cm
	//1 tot 2 = 18cm
	//2 tot 3 = 35cm
	//    CurArmX1 = constrain(CurArmX1, 1, 44);
	//
	//    B = sqrt(CurArmX1 * CurArmX1 + D * D);
	//    Serial.println("b    =   " + String(B));
	//    if (B < 24 and B != 0) // 28 normaal
	//    {
	//      alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
	//      betar = acos((A * A - B * B + C * C) / (2 * A * C));
	//      gammar = acos((A * A + B * B - C * C) / (2 * A * B));
	//      alpha = alphar * 180 / M_PI + 60;
	//      beta = betar * 180 / M_PI - 20;
	//      gamma = gammar * 180 / M_PI;
	//      float servohoek1 = map(alpha, 0, 360, 100, 1023);
	//      float servohoek2 = map(beta, 0, 360, 50, 1023);
	//      motor2.goalPosition(servohoek1);
	//      motor3.goalPosition(getMotor3Value(servohoek1));
	//      motor4.goalPosition(servohoek2);
	//    }

	//    CurArmY = constrain(CurArmY, 300, 750);
	//
	//    motor1.goalPosition(CurArmY);
}

void moveArm(float dist)
{
	//  put your main code here, to run repeatedly:
	B = sqrt(dist * dist + D * D);
	Serial.println("b    =   " + String(B));

	if (B < 24 and B != 0) // 28 normaal
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
		motor3.goalPosition(getMotor3Value(servohoek1));
		motor4.goalPosition(servohoek2);
	}
	sendBack("Arm moved");
}

void turn(String dir)
{
	if (dir == "left")
	{
		intensity = 200;
		angle = M_PI;
	}

	if (dir == "right")
	{
		intensity = 200;
		angle = 0.01 * M_PI;
	}

	if (dir.indexOf("good") != -1)
	{
		intensity = 0;
	}

	//  drive();
}

void moveRobot(String direction)
{
	sendBack("Move robot towards: " + direction);
}

void dance()
{
	if (ms > 0)
	{
		if (led_vars.step == 0)
		{
			up_beat();
			led_vars.step++;
			led_vars.timer_start = millis();
		}

		else if (led_vars.step == 1)
		{
			if (millis() - led_vars.timer_start > ms * 4)
			{
				led_vars.timer_start = millis();
				led_vars.step++;
			}
		}

		else if (led_vars.step == 2)
		{
			down_beat();
			led_vars.timer_start = millis();
			led_vars.step++;
		}

		else if (led_vars.step == 3)
		{
			if (millis() - led_vars.timer_start > ms * 4)
			{
				led_vars.step = 0;
			}
		}
	}
}

void randArmMove()
{
	motors.speed(300);
	int x1 = random(384, 640);
	int x2 = random(384, 640);
	int y1 = random(300, 750);

	motor2.goalPosition(x1);
	motor3.goalPosition(getMotor3Value(x1));
	motor4.goalPosition(x2);
	motor1.goalPosition(y1);
}

void up_beat()
{
	angle = 0.5 * M_PI;
	intensity = 200;
	drive();
	randArmMove();
}

void down_beat()
{
	angle = -0.5 * M_PI;
	intensity = 200;
	drive();
	randArmMove();
}

void danceLine()
{
	sendBack("Line dance");
}

void blinkLed()
{
	int i = 0;

	while (i < 3)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);

		i++;
	}
}
