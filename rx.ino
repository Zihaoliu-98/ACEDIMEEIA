/*TECHNICAL DEMONSTRATION FOR PROJECT AECDMEEIA*/
/*VERY IMPORTANT:PLEASE READ THROUGH TECHNICAL REPORT BEFORE EXAMINING THE CODE*/
/*RECEIVER. AUTHOR:ZIHAO LIU*/

#include "Arduino.h"
#include "heltec.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#define BAND 1090E6 /*ADS-B frequency. Please use permitted local frequency when testing*/

String temp = String("TEMPERATURE");
String ws = String("W-SPEED");
String wd = String ("W-DIRECTION");
String cw = String ("WEATHER");
String em = String("EMERGENCY");
String lapos = String ("LAT POSITION");
String lopos = String ("LO POSITION");
String lx = String("LUMINOSITY");
String sn = String("SIDE NOTE");

void setup() {
  /*Enable display, LoRa, Serial, PABOOST*/
  Heltec.begin(true, true, true, true, BAND);
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  LoRa.setSpreadingFactor(7);
  LoRa.receive();/*continuously receive packets*/
  Serial.begin(115200);
}

void loop() {
  /*First we try to parse the packet and store it in a string*/
  int packetSize = LoRa.parsePacket();
  Serial.println(packetSize);
  char rcvd1[packetSize];
  if (packetSize) {
    digitalWrite(25, HIGH);
    delay(500);
    digitalWrite(25, LOW);
    while (LoRa.available()) {
      for (int i = 0; i < packetSize; i++) {
        char* rcvd1 [i] = {(char*)LoRa.read()};
      }
      String rcvd = String(rcvd1);
      Serial.println("rcvd" + rcvd);
      /*Then we extract the DF code and do a case switch*/
      String DF = rcvd.substring(1, 6);
      int dfn = DF.toInt();
      switch (dfn) {
        /*all string extraction methods comply completly with the scheme delineated in technical report*/
        /*DF-17 case. Designed to be rejected as demonstration. So I will not bother write the real code*/
        case 10001: {
            String aid = rcvd.substring(9, 33);
            String latpos = rcvd.substring(33, 47);
            String logpos = rcvd.substring(47, 61);
            String aws = rcvd.substring(61, 66);
            String awd = rcvd.substring(66, 69);
            String at = rcvd.substring(69, 74);
            String acw = rcvd.substring(74, 79);
            String aem = rcvd.substring(79, 80);
            digitalWrite(25, HIGH);
            delay(500);
            digitalWrite(25, LOW);
            /*we see if aircraft ID is legitimate.*/
            /*in real application this will be a lookup in the file*/
            if (aid = "100001101100111001101100") {
              Heltec.display->drawString(0, 0, "DF-17,ACFT:JA801A");
              Heltec.display->drawString(10, 0, "REJECTED,UNTRUSTED SOURCE");
              Heltec.display->display();
            }
          }
          break;
        /*DF-18 case*/
        case 10010: {
            String aid = rcvd.substring(9, 33);
            String latpos = rcvd.substring(33, 47);
            String logpos = rcvd.substring(47, 61);
            String aws = rcvd.substring(61, 66);
            String awd = rcvd.substring(66, 69);
            String at = rcvd.substring(69, 74);
            String acw = rcvd.substring(74, 79);
            String aem = rcvd.substring(79, 80);
            /*we let LED blink to show everything before is fine and we are progressing*/
            digitalWrite(25, HIGH);
            delay(500);
            digitalWrite(25, LOW);
            /*We have extracted the substrings, and we now match it to yield information*/
            if (awd = "101") {
              String awd = String("SSW");
            }
            if (acw = "00100") {
              String acw = String("THUNDER STORM");
            }
            if (aem = "1") {
              String awd = String("IMMINENT");
            }
            else {
              String awd = String("CLEAR");
            }
            if (aid = "100001101100111001101100") {
              Heltec.display->drawString(0, 0, "DF-18,ACFT:D-ABYR");
              Heltec.display->drawString(10, 0, "REJECTED,UNTRUSTED SOURCE");
              Heltec.display->display();
            }
            /*if aid is legitimate*/
            else {
              Heltec.display->drawString(0, 0, "DF-18,ACFT:D-ABYR");
              Heltec.display->drawString(10, 0, temp + at);
              Heltec.display->drawString(20, 0, cw + acw);
              Heltec.display->drawString(30, 0, ws + aws);
              Heltec.display->drawString(40, 0, wd + awd);
              Heltec.display->drawString(50, 0, em + aem);
              Heltec.display->drawString(60, 0, lapos + latpos);
              Heltec.display->drawString(70, 0, lopos + logpos);
              Heltec.display->display();
            }
          }
          break;
        /*DF-24 case. Designed for local sensor station*/
        case 10100: {
            String latpos = rcvd.substring(9, 23);
            String logpos = rcvd.substring(23, 37);
            String gws = rcvd.substring(37, 44);
            String gwd = rcvd.substring(44, 50);
            String gwdflg = rcvd.substring(50, 51);
            String gcw = rcvd.substring(51, 57);
            String gem = rcvd.substring(57, 58);
            String did = rcvd.substring(58, 69);
            String nt = rcvd.substring(69, 74);
            String gl = rcvd.substring(74);
            digitalWrite(25, HIGH);
            delay(500);
            digitalWrite(25, LOW);
            if (nt = "11111") {
              nt = String("PLEASE SEND ENGINEER");
            }
            else {
              nt = String("ALLES KLAR!");/*Means everything fine (in case you don't speak German :D)*/
            }
            if (gwdflg = "1") {
              gwdflg = String("TRUE HEADING");
            }
            else {
              gwdflg = String("MAGNETIC HEADING");
            }
            if (gcw = "000100") {
              String gcw = String("THUNDER STORM");
            }
            if (gem = "1") {
              String gwd = String("IMMINENT");
            }
            else {
              String gwd = String("CLEAR");
            }
            Heltec.display->drawString(0, 0, "DF-24,SENSOR ID:" + did);
            Heltec.display->drawString(10, 0, lx + gl);
            Heltec.display->drawString(20, 0, cw + gcw);
            Heltec.display->drawString(30, 0, ws + gws);
            Heltec.display->drawString(40, 0, wd + gwd);
            Heltec.display->drawString(50, 0, em + gem);
            Heltec.display->drawString(60, 0, lapos + latpos);
            Heltec.display->drawString(70, 0, lopos + logpos);
            Heltec.display->drawString(80, 0, sn + nt);
            Heltec.display->display();
          }
          break;
        default: {
            Heltec.display->drawString(0, 0, "DEFAULT TEST");
            Heltec.display->display();
          }
          break;
      }
    }
  }
}
