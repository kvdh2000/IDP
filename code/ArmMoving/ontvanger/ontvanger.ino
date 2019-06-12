//idp code
//motoraansturing en bluetooth ontvanger

//adres van de slave: 98d3:31:fd1781

//liberarys
#include "JohnsSpecialEasyTransfer.h"


JohnsSpecialEasyTransfer bluetooth_conn;


//info
#define motor_out 3
#define motor_linksom 6
#define motor_rechtsom 7

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
    bluetooth_conn.add_recieve_int("VEH_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("VEH_Yas", js_neutral);
    bluetooth_conn.add_recieve_int("ARM_Xas", js_neutral);
    bluetooth_conn.add_recieve_int("ARM_Yas", js_neutral);

    pinMode(3, OUTPUT);
    pinMode(motor_linksom, OUTPUT);
    pinMode(motor_rechtsom, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {
	
	bluetooth_conn.update();
	VEHXas = bluetooth_conn.get_int("VEH_Xas");
	VEHYas = bluetooth_conn.get_int("VEH_Yas");

    Serial.print("Xas");
    Serial.println(VEHXas);
    Serial.print("Yas");
    Serial.println(VEHYas);

    if (Yas > deadzone_min && Yas < deadzone_max) {
        snelheid = 0;
        //Serial.println("neutraal");
    }
    if (VEHYas > deadzone_max) {
        richting = 1;
        //Serial.print("richting");
        //Serial.println(richting);
        vooruitschaal = VEHYas - 512;
        snelheid = vooruitschaal;
        //Serial.print("snelheid");
        //Serial.println(snelheid);
    }

    if (VEHYas < deadzone_min) {
        richting = 0;
        //Serial.print("richting");
        //Serial.println(richting);
        vooruitschaal = (VEHYas - 512) * (-1);
        snelheid = vooruitschaal;
        //Serial.print("snelheid");
        //Serial.println(snelheid);
    }

    Serial.println("------------------------");
    snelheid = map(snelheid, 0, 511, 0, 255);
    //Serial.println(snelheid);
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

void vehicleMovement(){
}
}
