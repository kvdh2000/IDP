//All libraries which we need
#include "CommandHandler.h"
#include "MegunoLink.h"

//Define variables
String val;

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
  val = Serial.read();
  Serial.println(val);
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
