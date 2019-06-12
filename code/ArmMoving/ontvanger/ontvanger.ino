//idp code
//motoraansturing en bluetooth ontvanger

//adres van de slave: 98d3:31:fd1781

//liberarys
#include "JohnsSpecialEasyTransfer.h"


JohnsSpecialEasyTransfer bluetooth_conn;


//info
#define motor_out 3
#define motor_linksom 8
#define motor_rechtsom 9

const int deadzone_min = 485;
const int deadzone_max = 535;
const int js_neutral = 512;


int Xas;
int Yas;
int totaal;
float snelheid = 0;
int richting;               //0 dan is het achteruit en 1 dan is vooruit
int vooruitschaal;


void setup() {
    Serial.begin(38400);
    Serial1.begin(38400);

    bluetooth_conn.begin(&Serial1);
    bluetooth_conn.add_recieve_int("J1_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("J1_Yas", js_neutral);
    bluetooth_conn.add_recieve_int("J2_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("J2_Yas", js_neutral);

    pinMode(3, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {
	
	bluetooth_conn.update();
	Xas = bluetooth_conn.get_int("J1_Xas");
	Yas = bluetooth_conn.get_int("J1_Yas");

    Serial.print("Xas");
    Serial.println(Xas);
    Serial.print("Yas");
    Serial.println(Yas);

    if (Yas > deadzone_min && Yas < deadzone_max) {
        snelheid = 0;
        //Serial.println("neutraal");
    }
    if (Yas > deadzone_max) {
        richting = 1;
        Serial.print("richting");
        Serial.println(richting);
        vooruitschaal = Yas - 512;
        snelheid = vooruitschaal;
        Serial.print("snelheid");
        Serial.println(snelheid);
    }

    if (Yas < deadzone_min) {
        richting = 0;
        Serial.print("richting");
        Serial.println(richting);
        vooruitschaal = (Yas - 512) * (-1);
        snelheid = vooruitschaal;
        Serial.print("snelheid");
        Serial.println(snelheid);
    }

    Serial.println("------------------------");
    snelheid = map(snelheid, 0, 511, 0, 255);
    Serial.println(snelheid);
    analogWrite(motor_out, snelheid);

    switch (richting) {
    case false:
        digitalWrite(motor_linksom, 1);
        digitalWrite(motor_rechtsom, 0);
        break;

    case true:
        digitalWrite(motor_linksom, 0);
        digitalWrite(motor_rechtsom, 1);
        break;

    default:
        break;
    }
}
