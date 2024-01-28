#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT// Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

#define REPORTING_PERIOD_MS 1000

char auth[] = "xxxxxxxxxxxxxxxx";
char ssid[] = "xxxxxxxxxxxxxxxx";
char pass[] = "xxxxxxxxxxxxxxxx";

#define IR1 D3
#define IR2 D4
#define IR3 D5

PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");
  }
  else
  {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  timer.setInterval(100L, IR1sensor);
  timer.setInterval(100L, IR2sensor);
  timer.setInterval(100L, IR3sensor); 
}

void IR1sensor(){
  bool value = digitalRead(IR1);
  if (value == 0) {
    Blynk.logEvent("notification_1","I Need Food!"); 
    WidgetLED LED(V3);
    LED.on();
  } else {
    WidgetLED LED(V3);
    LED.off();
  }

}

void IR2sensor(){
  bool value = digitalRead(IR2);
  if (value == 0) {
    Blynk.logEvent("notification_2","I Need Water!!"); 
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void IR3sensor(){
  bool value = digitalRead(IR3);
  if (value == 0) {
    Blynk.logEvent("notification_3","Emergency! Please Come Here!!!"); 
    WidgetLED LED(V5);
    LED.on();
  } else {
    WidgetLED LED(V5);
    LED.off();
  }

}

void loop()
{
  pox.update();
  Blynk.run();
  timer.run();

  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
      if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");

    Blynk.virtualWrite(V1, BPM);
    Blynk.virtualWrite(V2, SpO2);

    tsLastReport = millis();
  }
  
}
