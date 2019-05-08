#include "Servo.h"

//Define variables to be used
String readString;
String Text = "No text";

void setup()
{
  Serial.begin(9600);
  serv.attach(12);
}

void loop(){
  while (!Serial.available()) {} // wait for data to arrive
  // serial read section
  while (Serial.available())
  {
    delay(500);
    if (Serial.available() >0)
    {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }
  
  if (readString.length() >0)
  {
    Serial.print("Arduino received: ");  
    Serial.println(readString); //see what was received
  } 
  getCommand(readString);
  SendBack();
}

void getCommand(String command){
   //Command can be max 5 characters long   
   if(command == "forwa"||command=="back"||command=="left"||command=="right"){
    moveRobot(command);
   } else if(command == "dance"){
    dance();
   } else if(command == "blink"){
     blinkLed();
   }
}

void moveRobot(String direction){
  Text = "Move robot towards: " + direction;
  serv.write(90);
}

void dance(){
  Text = "Dance";
}

void blinkLed(){
  int i = 0;
  while(i < 3){
    pinMode(13, HIGH);
    delay(1000);
    pinMode(13, LOW);
    delay(1000);
    i++;
  }
}

void SendBack(){
  Serial.print("Arduino sends: ");
  Serial.println(Text);
  Serial.print("\n");
  Serial.flush();
  readString = "";
}
