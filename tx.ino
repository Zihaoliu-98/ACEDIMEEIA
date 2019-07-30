/*TECHNICAL DEMONSTRATION FOR PROJECT AECDMEEIA*/
/*VERY IMPORTANT:PLEASE READ THROUGH TECHNICAL REPORT BEFORE EXAMINING THE CODE*/
/*TRANSMITTER. AUTHOR:ZIHAO LIU*/
#include <stdio.h>;
#include <stdlib.h>
#include <string.h>
#include "Arduino.h"
#include "heltec.h"
#include <dht11.h>
#define BAND 1090E6 /*ADS-B frequency. Please use permitted local frequency when testing*/

dht11 DHT11;
int DHT11PIN = 4;
int photopin = 2;
int gaspin = 5;

/*df-format Identification number*/
String din1 = String(17, BIN); /*DF-17. Regular ADS-B*/
String din2 = String(18, BIN); /*DF-18. Non-interrogatable*/
String din3 = String(24, BIN); /*DF-24. Extended Code*/

/*aircraft identification number*/
String aid1 = String("001111000100101100110010"); /*D-ABYR operated by Lufthansa*/
String aid2 = String("100001101100111001101100"); /*JA801A operated by All Nippon Airways*/

/*aircraft borne information. In real application these data will be received from satellite downlink*/
/*32km/h wind speed from SSW, 32 degrees celsius, thunder storm, emergency situation alert*/
String aws = String(32, BIN);
String awd = String("101");
String at = String(32, BIN);
String acw = String("00100");
String aem = String("1");

/*the area to be served, in this case (1,1)*/
String latpos = String("00000000000001");
String logpos = String("00000000000001");

/*now we can get the message*/
/*to accerlate the compile, some message will be pre-built here*/
/*the differences are the aircraft that carries the information*/
String msg1 = String (din2 + "111" + aid1 + latpos + logpos + aws + awd + at + acw + aem);
String msg2 = String (din1 + "111" + aid2 + latpos + logpos + aws + awd + at + acw + aem);

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(115200);
  pinMode(DHT11PIN, INPUT);
  pinMode(photopin, INPUT);
  pinMode(gaspin, INPUT);
  LoRa.setSpreadingFactor(7);
}


void loop() {
  /*read the data from sensor*/
  int chk = DHT11.read(DHT11PIN);
  char a[50];
  float aa = (float)DHT11.humidity;
  char b[50];
  float bb = (float)DHT11.temperature;
  float p = 5000 / analogRead(photopin);
  float g = analogRead(gaspin);
  char c[50];
  char d[50];
  gcvt(aa, 5, a);
  gcvt(bb, 5, b);
  gcvt(p, 5, c);
  gcvt(g, 5, d);
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Humidity(%): ");
  Heltec.display->drawString(60, 0, a);
  Heltec.display->drawString(0, 10, "Temperature(C): ");
  Heltec.display->drawString(80, 10, b);
  Heltec.display->drawString(0, 20, "LUX: ");
  Heltec.display->drawString(30, 20, c);
  Heltec.display->drawString(0, 30, "GAS: ");
  Heltec.display->drawString(30, 30, d);
  Heltec.display->display();
  delay(5000);

  /*now we build a third message based on the data we collected from the luminosity sensor as an example*/
  /*this is to demonstrate how DF-24 system works for local stations*/
  /*we transmit the data from the luminosity sensor as an example*/
  String gws = String(64, BIN);
  String gwd = String("001111000");
  String gcw = String("000100");
  /*we use a flag here. If it is 0, then the wind direction is in magnetic heading, if 1 then it is in true heading*/
  String gwdflg = String("1");
  String gl = String ((int)p, BIN);
  /*device id*/
  String did = String("1110101");
  String nt = String("11111");
  String msg3 = String(din3 + "111" + latpos + logpos + gws + gwd + gwdflg + gcw + "1" + did + nt + gl);

  /*now we send our packets*/
  LoRa.beginPacket();
  LoRa.setTxPower(6, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(msg1);
  int q = LoRa.endPacket();
  digitalWrite(25, HIGH);
  delay(2000);
  digitalWrite(25, LOW);
  delay(2000);
  Serial.print(q);

  LoRa.beginPacket();
  LoRa.setTxPower(6, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(msg2);
  int w = LoRa.endPacket();
  digitalWrite(25, HIGH);
  delay(2000);
  digitalWrite(25, LOW);
  delay(2000);

  Serial.println(w);

  LoRa.beginPacket();
  LoRa.setTxPower(6, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(msg3);
  int e = LoRa.endPacket();
  digitalWrite(25, HIGH);
  delay(2000);
  digitalWrite(25, LOW);
  delay(2000);
  Serial.print(e);
}
