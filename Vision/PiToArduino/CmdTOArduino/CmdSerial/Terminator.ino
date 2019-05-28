void MoveArm(){
  int i = 0;
  //  put your main code here, to run repeatedly:
  while(i < 5){
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
  SendBack("Arm moved");
}

void moveRobot(String direction){
  SendBack("Move robot towards: " + direction);
}

void dance(){
  SendBack("Dance");
}

void danceLine(){
  SendBack("Line dance");
}

void blinkLed(){
  int i = 0;
  while(i < 3){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    i++;
  }
}
