#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <TinyGPS++.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define	HW_REV_COLS	1
#define PAUSE_TIME    0.1
#define SCROLL_SPEED  50
#define MAX_DEVICES 3
#define CLK_PIN   12  // or SCK
#define DATA_PIN  10  // or MOSI
#define CS_PIN    11  // or SS

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  P.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  P.setZoneEffect(0, true, PA_FLIP_UD); //hud display mode
}

void loop() {
  if (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      printDataOnLed();
    }
  }
  handleError();
}

void printDataOnLed() {
  printSpeed();
}

void handleError() {
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    // blink L led when TX/RX wiring to GPS is incorrect
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(1000);                      
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}

void printSpeed() {
  if (P.displayAnimate()) {
    char buffer[5];
    if (gps.satellites.value() == 0 ) {
      sprintf(buffer, "%s", "***");
    } else {
      sprintf(buffer, "%d", round(gps.speed.kmph()));
    }
    P.displayText(buffer, PA_CENTER, SCROLL_SPEED, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  }
}
