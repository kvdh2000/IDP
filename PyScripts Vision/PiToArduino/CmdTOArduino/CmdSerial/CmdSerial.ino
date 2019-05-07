#include "Servo.h"
String readString;
String Text = "No text found";
Servo serv;

void setup()
{
  Serial.begin(9600); 
  serv.attach(12);
  serv.write(90);
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
   }
}

void moveRobot(String direction){
  Text = "Move robot towards: " + direction;
  
}

void dance(){
  Text = "Dance";
}


void SendBack(){
  Serial.print("Arduino sends: ");
  Serial.println(Text);
  Serial.print("\n");
  Serial.flush();
  readString = "";
}
