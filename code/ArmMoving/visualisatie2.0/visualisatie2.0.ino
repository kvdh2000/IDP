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
#include "JohnsSpecialEasyTransfer.h"

JohnsSpecialEasyTransfer bluetooth_send;


#define JS1_X_AS A2
#define JS1_Y_AS A1
#define JS2_X_AS A4
#define JS2_Y_AS A3


unsigned long bt_counter = millis();

int bxas;
int byas;
int currentpage = 1;
int dansing = 0;
int tracking = 0;
int eitransporting = 0;
int linedanceing = 0;
int instelhoogte = 0;
byte Xcord = 0;
byte Ycord = 0;
byte incomming = 0;
int errorcode = 0;


SoftwareSerial HMISerial(7, 8);

//page1
NexButton duckie = NexButton(0, 3, "duckie");

//page2
NexButton functies = NexButton(1, 2, "functie");
NexButton bewegen = NexButton(1, 3, "bewegen");
NexButton error = NexButton(1, 4, "error");

//page3
NexButton terug1 = NexButton(2, 1, "terug");
NexDSButton dans = NexDSButton(2, 2, "dans");
NexDSButton tracker = NexDSButton(2, 3, "tracker");
NexDSButton linedance = NexDSButton(2, 4, "linedance");
NexDSButton eitransport = NexDSButton(2, 5, "eitransport");

//page4
NexButton terug2 = NexButton(3, 1, "terug");
NexSlider hoogte = NexSlider(3, 2, "hoogte");
NexSlider yas = NexSlider(3, 6, "yas");
NexSlider xas = NexSlider(3, 4, "xas");
NexDSButton klauw = NexDSButton(3, 8, "klauw");

//page5
NexButton terug3 = NexButton(4, 1, "terug");
NexText errormelding = NexText(4, 2, "errormelding");


//pages
NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page3 = NexPage(3, 0, "page3");
NexPage page4 = NexPage(4, 0, "page4");


NexTouch *knoppenlijst[] = {
    &duckie,
    &functies,
    &bewegen,
    &terug1,
    &dans,
    &tracker,
    &eitransport,
    &linedance,
    &terug2,
    &hoogte,
    &yas,
    &xas,
    &klauw,
    &terug3,
    &page0,
    &page1,
    &page2,
    &page3,
    &page4,
    NULL

};


void setup() {
    nexInit();
    Serial.begin(38400);
    duckie.attachPush(duckiein);
    functies.attachPush(functiesin);
    bewegen.attachPush(bewegenin);
    //error.attachPush(errorin);
    terug1.attachPush(terugin);
    dans.attachPush(dansin);
    tracker.attachPush(trackerin);
    eitransport.attachPush(eitransportin);
    linedance.attachPush(linedancein);
    terug2.attachPush(terugin);
    terug3.attachPush(terugin);
    page0.attachPush(page0in);
    page1.attachPush(page1in);
    page2.attachPush(page2in);
    page3.attachPush(page3in);

    HMISerial.print("page 0");
    HMISerial.write(0xff);
    HMISerial.write(0xff);
    HMISerial.write(0xff);
    bluetooth_send.begin(&Serial);

}

void loop() {
    nexLoop(knoppenlijst);
    Serial.println(currentpage);
//    if(currentpage == 3){
//    	if (millis() - bt_counter > 100)
//    	{
//    		bt_counter = millis();
//        bluetooth_send.send_int("J1_Xas", analogRead(JS1_X_AS));
//    		bluetooth_send.send_int("J1_Yas", analogRead(JS1_Y_AS));
//    		bluetooth_send.send_int("J2_Xas", (analogRead(JS2_X_AS) - 1024) * -1);// JS2 moet worden geinvert
//    		bluetooth_send.send_int("J2_Yas", (analogRead(JS2_Y_AS)- 1024) * -1);
//    	}
//    }
// delay(100);
}

void duckiein(void *ptr) {
    currentpage = 1;
}

void functiesin (void *ptr) {
    currentpage = 2;
}

void bewegenin (void *ptr) {
    currentpage = 3;
    Serial.println("kaaaaankaaaaaaeeerrrrr");
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
        HMISerial.print("dans.pic=");
        HMISerial.print(5);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
    } else {
        dansing = 1;
        HMISerial.print("dans.pic=");
        HMISerial.print(4);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("tracker.pic=");
        HMISerial.print(6);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("eitransport.pic=");
        HMISerial.print(9);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("linedance.pic=");
        HMISerial.print(11);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        tracking = 0;
        eitransporting = 0;
        linedanceing = 0;
    }
}

void trackerin(void *ptr) {
    if (tracking == 1) {
        HMISerial.print("tracker.pic=");
        HMISerial.print(6);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        tracking = 0;
    } else {
        HMISerial.print("dans.pic=");
        HMISerial.print(5);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("tracker.pic=");
        HMISerial.print(7);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("linedance.pic=");
        HMISerial.print(11);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("eitransport.pic=");
        HMISerial.print(9);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        dansing = 0;
        tracking = 1;
        eitransporting = 0;
        linedanceing = 0;
    }
}

void eitransportin(void *ptr) {
    if (eitransporting == 1) {
        eitransporting = 0;
        HMISerial.print("eitransport.pic=");
        HMISerial.print(9);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
    } else {
        HMISerial.print("dans.pic=");
        HMISerial.print(5);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("tracker.pic=");
        HMISerial.print(6);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("linedance.pic=");
        HMISerial.print(11);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("eitransport.pic=");
        HMISerial.print(10);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        dansing = 0;
        tracking = 0;
        eitransporting = 1;
        linedanceing = 0;
    }
}

void linedancein(void *ptr) {
    if (linedanceing == 1) {
        HMISerial.print("linedance.pic=");
        HMISerial.print(11);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        linedanceing = 0;
    } else {
        HMISerial.print("dans.pic=");
        HMISerial.print(5);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("tracker.pic=");
        HMISerial.print(6);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("linedance.pic=");
        HMISerial.print(12);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        delay(1);
        HMISerial.print("eitransport.pic=");
        HMISerial.print(9);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        HMISerial.write(0xff);
        dansing = 0;
        tracking = 0;
        eitransporting = 0;
        linedanceing = 1;
    }
}

//void hoogtein (void *ptr) {
//  hoogte.getValue(&instelhoogte);
//}
//
//void xasin (void *ptr) {
//  xas.getValue(&Xcord);
//}
//
//void yasin (void *ptr) {
//  yas.getValue(&Ycord);
//}

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
