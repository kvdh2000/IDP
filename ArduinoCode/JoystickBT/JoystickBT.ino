#include <NexButton.h>
#include <NexPage.h>
#include <Nextion.h>
#include "JohnsSpecialEasyTransfer.h"

//VEH-Vehicle = joystick2
//ARM = joystick1
#define VEH_X_AS A4
#define VEH_Y_AS A3
#define ARM_X_AS A2
#define ARM_Y_AS A1

JohnsSpecialEasyTransfer bluetooth_send;
unsigned long bt_counter = millis();

int currentpage = 1;
bool klauwbool = false;
SoftwareSerial HMISerial(7, 8);
NexButton bewegen = NexButton(1, 3, "bewegen");
NexButton klauw = NexButton(3, 8, "klauw");

NexTouch *knoppenlijst[] = {
    &bewegen,
    &klauw,
    NULL

};

void setup() {
    nexInit();
    Serial.begin(38400);
    bewegen.attachPush(bewegenIn);
    klauw.attachPush(klauwDicht);
    bluetooth_send.begin(&Serial);
}

void loop() {
    nexLoop(knoppenlijst);
    
    if(currentpage == 3){      
       if (millis() - bt_counter > 100)
        {
          bt_counter = millis();
          bluetooth_send.send_int("Motor_Xas", analogRead(VEH_X_AS));
          bluetooth_send.send_int("Motor_Yas", analogRead(VEH_Y_AS));
          bluetooth_send.send_int("Arm_Xas", (analogRead(ARM_X_AS) - 1024) * -1);// JS2 moet worden geinvert
          bluetooth_send.send_int("Arm_Yas", (analogRead(ARM_Y_AS)- 1024) * -1);
          bluetooth_send.send_int("Hand", klauwbool);
        }
    }
}

void klauwDicht(void *ptr){
  klauwbool = !klauwbool;
}
void bewegenIn (void *ptr) {
    currentpage = 3;
}
