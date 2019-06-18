#include <NexButton.h>
#include <NexPage.h>
#include <Nextion.h>
#include "btLib.h"

//VEH-Vehicle = joystick2
//ARM = joystick1
#define StickOne_X_AS A4
#define StickOne_Y_AS A3
#define StickTwo_X_AS A2
#define StickTwo_Y_AS A1

btLib bluetooth_send;
unsigned long bt_counter = millis();

int currentpage = 1;
bool clawBool = false;
bool driveBool = true;
int rupsInt = 0;
SoftwareSerial HMISerial(7, 8);
NexButton bewegen = NexButton(1, 3, "bewegen");
NexButton klauw = NexButton(3, 2, "klauw");
NexButton rups = NexButton(3, 5, "rups");
NexButton rijden = NexButton(3, 4, "rijden");
NexButton terug2 = NexButton(2, 1, "terug");
NexButton terug3 = NexButton(3, 1, "terug");
NexButton terug4 = NexButton(4, 1, "terug");
NexButton duckstad = NexButton(4, 2, "duckstad");
NexButton eibergen = NexButton(4, 3, "eibergen");
NexButton eindhoven = NexButton(4, 4, "eindhoven");
NexButton barneveld = NexButton(4, 5, "barneveld");
NexButton setLocText = NexButton(4, 11, "setLocText");

NexTouch *knoppenlijst[] = {
  &bewegen,
  &klauw,
  &rups,
  &rijden,
  &terug2,
  &terug3,
  &terug4,
  &duckstad,
  &eibergen,
  &eindhoven,
  &barneveld,
  NULL

};

void setup() {
  nexInit();
  Serial.begin(38400);
  bewegen.attachPush(bewegenIn);
  rups.attachPush(rupsIn);
  klauw.attachPush(klauwDicht);
  rijden.attachPush(rijdenIn);
  terug2.attachPush(terugIn);
  terug3.attachPush(terugIn);
  terug4.attachPush(terugIn);
  duckstad.attachPush(duckIn);
  eibergen.attachPush(eiIn);
  eindhoven.attachPush(eindIn);
  barneveld.attachPush(barnIn);
  bluetooth_send.begin(&Serial);
}

void loop() {
  nexLoop(knoppenlijst);

  if (currentpage == 3) {
    if (millis() - bt_counter > 100)
    {
      bt_counter = millis();
      bluetooth_send.send_int("StickOne_Xas", analogRead(StickOne_X_AS));
      bluetooth_send.send_int("StickOne_Yas", analogRead(StickOne_Y_AS));
      bluetooth_send.send_int("StickTwo_Xas", (analogRead(StickTwo_X_AS) - 1024) * -1);// JS2 moet worden geinvert
      bluetooth_send.send_int("StickTwo_Yas", (analogRead(StickTwo_Y_AS) - 1024) * -1);
      bluetooth_send.send_int("Hand", clawBool);
      bluetooth_send.send_int("Drive", driveBool);
    }
  }
}

void rupsIn(void *ptr){
  rupsInt++;
  if (rupsInt > 2) rupsInt = 0;
  switch (rupsInt) {
    case 0:
      rups.setText("Low");
      break;
    case 1:
      rups.setText("High");
      break;
    case 2:
      rups.setText("Caterpillar");
      break;
  bluetooth_send.send_int("Legs", rupsInt);
  }
}

void klauwDicht(void *ptr) {
  clawBool = !clawBool;
  if (clawBool) {
    klauw.setText("Release");
  } else {
    klauw.setText("Grab");
  }
}

void rijdenIn(void *ptr) {
  driveBool = !driveBool;
  if (driveBool) {
    rijden.setText("Claw");
  } else {
    rijden.setText("Drive");
  }
}

void bewegenIn (void *ptr) {
  currentpage = 3;
}

void terugIn (void *ptr) {
  currentpage = 1;
}

void duckIn(void *ptr) {
  bluetooth_send.send_int("Location", 0);
  updateLocText(0);
}

void eiIn(void *ptr) {
  bluetooth_send.send_int("Location", 1);
  updateLocText(1);
}

void eindIn(void *ptr) {
  bluetooth_send.send_int("Location", 2);
  updateLocText(2);
}

void barnIn(void *ptr) {
  bluetooth_send.send_int("Location", 3);
  updateLocText(3);
}

void updateLocText(int location) {
  switch (location) {
    case 0:
      setLocText.setText("Duckstad");
      break;
    case 1:
      setLocText.setText("Eibergen");
      break;
    case 2:
      setLocText.setText("Eindhoven");
      break;
    case 3:
      setLocText.setText("Barneveld");
      break;
    default:
      setLocText.setText("None");
      break;
  }
}
