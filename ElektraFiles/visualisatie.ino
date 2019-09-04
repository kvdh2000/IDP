#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>

#define xas A4
#define yas A5
#define button 2

int bxas;
int byas;
int currentpage = 1;
int dansing = 0;
int tracking = 0;
int eitransporting = 0;
int linedanceing = 0;



SoftwareSerial HMISerial(10, 11);

//page1
NexText welkom = NexText(0, 1, "welkom");

//page2
NexButton functies = NexButton(1, 1, "functies");
NexButton bewegen = NexButton(1, 2, "bewegen");

//page3
NexButton terug1 = NexButton(2, 1, "terug");
NexButton dans = NexButton(2, 2, "dans");
NexButton tracker = NexButton(2, 3, "tracker");
NexButton eitransport = NexButton(2, 4, "eitransport");
NexButton linedance = NexButton(2, 5, "linedance");

//page4
NexButton terug2 = NexButton(3, 2, "terug");
NexSlider hight = NexSlider(3, 1, "hight");

//pages
NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page3 = NexPage(3, 0, "page3");


NexTouch *knoppenlijst[] = {
  &welkom,
  &functies,
  &bewegen,
  &terug1,
  &dans,
  &tracker,
  &eitransport,
  &linedance,
  &terug2,
  &hight,
  &page0,
  &page1,
  &page2,
  &page3,
  NULL

};

void welkomin(void *ptr) {
  currentpage = 1;
}

void functiesin (void *ptr) {
  currentpage = 2;
}

void bewegenin (void *ptr) {
  currentpage = 3;
}

void terugin (void *ptr) {
  currentpage = 1;
  dansing = 0;
  tracking = 0;
  eitransporting = 0;
  linedanceing = 0;
}

void dansin(void *ptr) {
  if (dansing == 1) {
    dansing = 0;
  }
  if (dansing == 0) {
    dansing = 1;
    tracking = 0;
    eitransporting = 0;
    linedanceing = 0;
  }
}

void trackerin(void *ptr) {
  if (tracking == 1) {
    tracking = 0;
  }
  if (tracking == 0) {
    dansing = 0;
    tracking = 1;
    eitransporting = 0;
    linedanceing = 0;
  }
}

void eitransportin(void *ptr) {
  if (eitransporting == 1) {
    eitransporting = 0;
  }
  if (eitransporting == 0) {
    dansing = 0;
    tracking = 0;
    eitransporting = 1;
    linedanceing = 0;
  }
}

void linedancein(void *ptr) {
  if (linedanceing == 1) {
    linedanceing = 0;
  }
  if (linedanceing == 0) {
    dansing = 0;
    tracking = 0;
    eitransporting = 0;
    linedanceing = 1;
  }
}

void page0in (void *ptr) {
  currentpage = 0;
}

void page1in (void *ptr) {
  currentpage = 1;
}

void page2in (void *ptr) {
  currentpage = 2;
}

void page3in (void *ptr) {
  currentpage = 3;
}

void setup() {
  nexInit();
  Serial.begin(9600);
  welkom.attachPush(welkomin);
  functies.attachPush(functiesin);
  bewegen.attachPush(bewegenin);
  terug1.attachPush(terugin);
  dans.attachPush(dansin);
  tracker.attachPush(trackerin);
  eitransport.attachPush(eitransportin);
  linedance.attachPush(linedancein);
  terug2.attachPush(terugin);
  page0.attachPush(page0in);
  page1.attachPush(page1in);
  page2.attachPush(page2in);
  page3.attachPush(page3in);
}

void loop() {
  nexLoop(knoppenlijst);
  //Serial.print(dansing);
  //Serial.print(tracking);
  //Serial.print(eitransporting);
  //Serial.println(linedanceing);
  bxas = analogRead(xas);
  Serial.println(bxas);
  delay(100);
}
