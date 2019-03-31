#include <Wire.h>
#include "RTClib.h"
#include "SevSeg.h"

#define dig0 A2
#define dig1 11
#define dig2 12
#define dig3 13
#define seg0 A3
#define seg1 A0
#define seg2 A1
#define seg3 10
#define seg4 9
#define seg5 8
#define seg6 7
#define seg7 4
#define led1 5
#define led2 6
#define rtcSCL A5
#define rtcSDA A4
#define btn1 2
#define btn2 3

RTC_DS1307 rtc;
SevSeg sevseg;
int timer1_counter;
volatile int hourInt;
volatile int setHourFlag;
volatile int minInt;
volatile int setMinFlag;
volatile int b;

/* */ // SCHIMBA 10-A2
//          9-A3
//

void setup() {
  pinMode(dig0, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);
  pinMode(seg0, OUTPUT);
  pinMode(seg1, OUTPUT);
  pinMode(seg2, OUTPUT);
  pinMode(seg3, OUTPUT);
  pinMode(seg4, OUTPUT);
  pinMode(seg5, OUTPUT);
  pinMode(seg6, OUTPUT);
  pinMode(seg7, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(rtcSCL, INPUT);
  pinMode(rtcSDA, INPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(btn1), adjustHour, RISING);
  attachInterrupt(digitalPinToInterrupt(btn2), adjustMin, RISING);
  /* 7-Seg Display */
  
  byte numDigits = 4;
  byte digitPins[] = {dig0, dig1, dig2, dig3};
  //byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {seg0, seg1, seg2, seg3, seg4, seg5, seg6, seg7};
  //byte segmentPins[] = {9, 2, A0, A1, 6, 8, 7, 4};
  //byte segmentPins[] = {9,    2,    3,   5,    6,     8,     7,    4};

  bool resistorsOnSegments = true;
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(20);


  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  /* Timer interrupt */
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 45911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 58286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

  interrupts();             // enable all interrupts
}
void loop () {
   
  DateTime now = rtc.now();
  if (setHourFlag == 1) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hourInt, now.minute(), now.second()));
    Serial.println("Setting hour: \n");
    Serial.print(hourInt);
    Serial.print(':');
    Serial.print(minInt);
    Serial.print('\n');
  }
  if (setMinFlag == 1) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minInt, 0));
    Serial.println("Setting minute: \n");
    Serial.print(hourInt);
    Serial.print(':');
    Serial.print(minInt);
    Serial.print('\n');
  }
  Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  
  sevseg.setNumber(now.hour() * 100 + now.minute(), 2);
  sevseg.refreshDisplay();
  

}


ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  TCNT1 = timer1_counter;   // preload timer
  setMinFlag = 0;
  setHourFlag = 0;
 // if(b==255) b = 0;
   b += 25;
  if(b>250) b = 0;
  //else if (b>230) b = 0;
  analogWrite(led1, b);
  if(255-b < 0) b = 0;
  analogWrite(led2, 250-b);
 //Serial.print(b);
  //Serial.println("adjusting brightness ");

}

void adjustHour() {
  if (hourInt > 23) hourInt = 0;  else hourInt++;
  setHourFlag = 1;
  Serial.println("pressed hour");
}

void adjustMin() {
  if (minInt > 58) minInt = 0;  else  minInt++;
  setMinFlag = 1;
  Serial.println("pressed min");
}
