//int16_t speed1 = 1023;
//int16_t speed2 = 512;
//
//HardwareDynamixelInterface interface(Serial1, DIR_PIN);
//DynamixelMotor motor1(interface, 1);
//DynamixelMotor motor2(interface, 2);
//DynamixelMotor motor3(interface, 3);
//DynamixelMotor motor4(interface, 4);
//DynamixelMotor motor5(interface, 5);
//DynamixelMotor motor6(interface, 6);
////DynamixelMotor motor7(interface, 7);
////DynamixelMotor motor8(interface, 8);
////DynamixelMotor motor9(interface, 9);
////DynamixelMotor motor10(interface, 10);
////DynamixelMotor motor11(interface, 11);
////DynamixelMotor motor12(interface, 12);
////DynamixelMotor motor13(interface, 13);
////DynamixelMotor motor14(interface, 14);
////DynamixelMotor motor15(interface, 15);
////DynamixelMotor motor16(interface, 16);
//DynamixelMotor motors(interface, BROADCAST_ID);
//
//void initServos(){  
//  interface.begin(baudrate);  
//  motors.enableTorque();
//  motors.speed(speed2);
//  motor1.jointMode(0, 1023);
//  motor2.jointMode(124, 897);
//  motor3.jointMode(33, 680);
//  motor4.jointMode(19, 615);
//  motor5.jointMode(0, 595);
//  motor6.jointMode(28, 684);
//  //  motor7.jointMode(0, 1023);
//  //  motor8.jointMode(0, 1023);
//  //  motor9.jointMode(0, 1023);
//  //  motor10.jointMode(0, 1023);
//  //  motor11.jointMode(0, 1023);
//  //  motor12.jointMode(0, 1023);
//  //  motor13.jointMode(0, 1023);
//  //  motor14.jointMode(0, 1023);
//  //  motor15.jointMode(0, 1023);
//  //  motor16.jointMode(0, 1023);
//
//  //digitalWrite(DIR_PIN, HIGH);
//  motor1.goalPosition(825);
//  motor2.goalPosition(512);
//  motor3.goalPosition(512);
//  motor4.goalPosition(512);
//  motor5.goalPosition(512);
//  motor6.goalPosition(350);
//  //  motor7.goalPosition(900);
//  //  motor8.goalPosition(200);
//  //  motor9.goalPosition(200);
//  //  motor10.goalPosition(350);
//  //  motor11.goalPosition(900);
//  //  motor12.goalPosition(200);
//  //  motor13.goalPosition(200);
//  //  motor14.goalPosition(350);
//}
//void moveArm(){
//  int i = 0;
//  //  put your main code here, to run repeatedly:
//  while(i < 5){
//    motor1.goalPosition(900);
//    motor4.goalPosition(50);
//    motor5.goalPosition(50);
//    motor6.goalPosition(350);
//    delay(1000);
//
//    motor1.goalPosition(900);
//    motor4.goalPosition(670);
//    motor5.goalPosition(400);
//    motor6.goalPosition(700);
//    delay(1000);
//    i++;
//  }
//  sendBack("Arm moved");
//}
//
//void lookAround(){
//  int i = 0;
//  //  checking left and right 2 times
//  // Eerst kijken voordat je oversteekt  
//  motor2.goalPosition(512);
//  motor3.goalPosition(512);
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
//  sendBack("ff spieken");
//}
//
//void moveRobot(String direction){
//  sendBack("Move robot towards: " + direction);
//}
//
//void dance(){
//  sendBack("Dance");
//}
//
//void danceLine(){
//  sendBack("Line dance");
//}
//
//void blinkLed(){
//  int i = 0;
//  while(i < 3){
//    digitalWrite(LED_BUILTIN, HIGH);
//    delay(1000);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(1000);
//    i++;
//  }
//}
