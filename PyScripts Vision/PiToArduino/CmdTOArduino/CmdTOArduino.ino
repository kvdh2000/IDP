//All libraries which we need
#include "CommandHandler.h"
#include "MegunoLink.h"

//Define variables
String readString;

CommandHandler<> SerialCommandHandler;

//Setup all the things
void setup(){
  Serial.begin(9600);
  SerialCommandHandler.AddCommand(F("TEST"), Cmd_Testing);
  SerialCommandHandler.AddCommand(F("PLACEHOLDER"), Cmd_PlaceHolder);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);
}

//Loop everything
void loop(){
  SerialCommandHandler.Process();
  while (!Serial.available()) {} // wait for data to arrive
  // serial read section
  while (Serial.available())
  {
    // continue as before
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c;
    Serial.println(readString);
    Serial.println(Serial.read());
  }
  char ard_sends = '1';
  Serial.print("Arduino sends: ");
  Serial.println(ard_sends);
  Serial.print("\n");
  Serial.flush();
}

//Functions to be executed by the commandhandler
void Cmd_Testing(CommandParameter &Parameters){
  Serial.println("I'm here now!\nYa Yeet.....");
}

void Cmd_PlaceHolder(CommandParameter &Parameters){
  Serial.println("PlaceHolder desu");
}

//Default function to be executed by the commandhandler
void Cmd_Unknown(){
  Serial.println("Shits broken yo, i dun get it");
}
