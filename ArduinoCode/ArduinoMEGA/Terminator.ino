int16_t speed1 = 1023;
int16_t speed2 = 512;

void initServos() {
  interface.begin(baudrate);
  motors.enableTorque();
  motors.speed(speed2);
  motor1.jointMode(0, 1023);
  motor2.jointMode(150, 900);
  motor3.jointMode(150, 900);
  motor4.jointMode(20, 1023);
  motor5.jointMode(0, 1023);
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
  motor1.goalPosition(512);
  motor2.goalPosition(512);
  motor3.goalPosition(getMotor3Value(512));

  motor4.goalPosition(512);
  motor5.goalPosition(512);
  motor6.goalPosition(460);
  //  motor7.goalPosition(900);
  //  motor8.goalPosition(200);
  //  motor9.goalPosition(200);
  //  motor10.goalPosition(350);
  //  motor11.goalPosition(900);
  //  motor12.goalPosition(200);
  //  motor13.goalPosition(200);
  //  motor14.goalPosition(350);
}

int getMotor3Value(int mtr2Val) {
  return 1023 - mtr2Val;
}

void armMovement()
{
  motors.speed(300);

  Serial.print("x1  -  ");
  Serial.println(stickOneXas);
  Serial.print("y =  ");
  Serial.println(stickOneYas);
  Serial.print("x2 =  ");
  Serial.println(stickTwoXas);

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
    CurArmX2 -= 10;
    Serial.println("minderGAss");
  }
  if (stickTwoXas < deadzone_min)
  {
    CurArmX2 += 10;
    Serial.println("gasss");
  }
  
  CurArmX1 = constrain(CurArmX1, 150, 900);
  CurArmX2 = constrain(CurArmX2, 50, 900);
  Serial.print(CurArmX2);
  CurArmY = constrain(CurArmY, 1, 1023);
  motor1.goalPosition(CurArmY);
  motor2.goalPosition(CurArmX1);
  motor3.goalPosition(getMotor3Value(CurArmX1));
  motor4.goalPosition(CurArmX2);
  //  CurArmX = constrain(CurArmX, 1, 28);
  //
  //  B = sqrt(CurArmX * CurArmX + D * D);
  //  Serial.println("b    =   " + String(B));
  //  if (B < 24 and B != 0) // 28 normaal
  //  {
  //    alphar = acos((-A * A + B * B + C * C) / (2 * B * C));
  //    betar = acos((A * A - B * B + C * C) / (2 * A * C));
  //    gammar = acos((A * A + B * B - C * C) / (2 * A * B));
  //    alpha = alphar * 180 / M_PI + 60;
  //    beta = betar * 180 / M_PI - 20;
  //    gamma = gammar * 180 / M_PI;
  //    float servohoek1 = map(alpha, 0, 360, 100, 1023);
  //    float servohoek2 = map(beta, 0, 360, 50, 1023);
  //    motor2.goalPosition(servohoek1);
  //    motor3.goalPosition(getMotor3Value(servohoek1));
  //    motor4.goalPosition(servohoek2);
  //  }
  //
  //  CurArmY = constrain(CurArmY, 0, 1023);

//  motor1.goalPosition(CurArmY);
}

void moveArm() {
  int i = 0;
  //  put your main code here, to run repeatedly:
  while (i < 5) {
    motor1.goalPosition(900);
    motor4.goalPosition(50);
    motor5.goalPosition(50);
    motor6.goalPosition(350);
    delay(1000);

    motor1.goalPosition(900);
    motor4.goalPosition(670);
    motor5.goalPosition(400);
    motor6.goalPosition(700);
    delay(1000);
    i++;
  }
  sendBack("Arm moved");
}

void lookAround() { // de robot omhoog en naar beneden laten bewegen
  int i = 0;
  //  checking left and right 2 times
  // Eerst kijken voordat je oversteekt
  //  motor2.goalPosition(512);
  //  motor3.goalPosition(getMotor3Value(512));
  //  motor4.goalPosition(512);
  //  motor5.goalPosition(512);
  //  motor6.goalPosition(350);
  //
  //  while(i < 2){
  //    motor1.goalPosition(1023);
  //    delay(1000);
  //
  //    motor1.goalPosition(627);
  //    delay(1000);
  //    i++;
  //  }
  //
  //  motor1.goalPosition(825);
  sendBack("ff spieken");
}

void moveRobot(String direction) {
  sendBack("Move robot towards: " + direction);
}

void dance() {
  sendBack("Dance");
}

void danceLine() {
  sendBack("Line dance");
}

void blinkLed() {
  int i = 0;
  while (i < 3) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    i++;
  }
}
