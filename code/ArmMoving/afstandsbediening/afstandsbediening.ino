//IDP
//bluetooth module
//afstandsbediening (kant)

//pairing the moduals
//https://www.youtube.com/watch?v=BXXAcFOTnBo
#include "JohnsSpecialEasyTransfer.h"



JohnsSpecialEasyTransfer conn;


//info
#define potmeter A0
#define knop 7

int J1_Xas;
int J1_Yas;



void setup() {
  Serial.begin(38400);    //voor de bluetooth module
  conn.begin(&Serial);

}

void loop() {
  J1_Xas = analogRead(A1);    //xas op pin A1
  J1_Xas = analogRead(A1);
  J1_Yas = analogRead(A2);    //yas op pin A2
  J1_Yas = analogRead(A2);
  conn.send_int("J1_Xas", J1_Xas);
  conn.send_int("J2_Yas", J2_Yas);
  delay(40);
}
