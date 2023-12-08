#include <Adafruit_SH1106.h>
#include <TinyGPS++.h>
#include <TimeLib.h>
#define OLED_RESET -1        //Reset pin # (or -1 if sharing Arduino reset pin)
#define TIMEZONE_OFFSET 3        //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  //See datasheet for Address
Adafruit_SH1106 display(OLED_RESET);
TinyGPSPlus gps;
char buffer[10];
time_t t, prevDisplay = 0;

void setup() {
  //Set the baud rate in u-center to 115200
  Serial.begin(115200);
  display.begin(SH1106_SWITCHCAPVCC, SCREEN_ADDRESS);
  //invert display for a specific box, remove the next line in order to set default rotation
  display.setRotation(2);
  display.setTextColor(WHITE, BLACK);
  display.clearDisplay();
  delay(200);
  display.display();
}

void loop() {
  if (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      printDataOnOled();
    }
  }
  handleError();
}

void printDataOnOled() {
  printSpeed();
  printTime();
  prinSatellites();
  printGPSMark();
  printCourse();
  display.display();
}

void handleError() {
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print(F("Check GPS Wiring"));
    display.setCursor(0, 16);
    display.print(F("No Data from receiver"));
    display.display();
    delay(1000);
    display.clearDisplay();
  }
}

void printTime() {
  display.setCursor(0, 52);
  display.setTextSize(1);

  if (gps.date.year() > 2000) {
    setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
    adjustTime(TIMEZONE_OFFSET * SECS_PER_HOUR);
    if (now() != prevDisplay) {  //update the display only if the time has changed
      prevDisplay = now();
      sprintf(buffer, "%02d.", day());
      display.print(buffer);
      sprintf(buffer, "%02d.", month());
      display.print(buffer);
      display.print(year());
      sprintf(buffer, "\ %02d\:", hour());
      display.print(buffer);
      sprintf(buffer, "%02d\:", minute());
      display.print(buffer);
      sprintf(buffer, "%02d", second());
      display.print(buffer);
    }
  } else {
    display.println(F("Obtaining GPS time"));
  }
}

void printCourse() {
  if (gps.course.isValid()) { 
    float delta = 45 / 2;
    display.setCursor(80, 10);
    display.setTextSize(1);
    display.print(F("DIR: "));
    if (gps.course.deg() >= (360 - delta) || gps.course.deg() <= delta) {
      display.print(F("N "));
    } else if (gps.course.deg() >= (45 - delta) && gps.course.deg() <= (45 + delta)) {
      display.print(F("NE"));
    } else if (gps.course.deg() >= (90 - delta) && gps.course.deg() <= (90 + delta)) {
      display.print(F("E "));
    } else if (gps.course.deg() >= (135 - delta) && gps.course.deg() <= (135 + delta)) {
      display.print(F("SE"));
    } else if (gps.course.deg() >= (180 - delta) && gps.course.deg() <= (180 + delta)) {
      display.print(F("S "));
    } else if (gps.course.deg() >= (225 - delta) && gps.course.deg() <= (225 + delta)) {
      display.print(F("SW"));
    } else if (gps.course.deg() >= (270 - delta) && gps.course.deg() <= (270 + delta)) {
      display.print(F("W "));
    } else if (gps.course.deg() >= (315 - delta) && gps.course.deg() <= (315 + delta)) {
      display.print(F("NW"));
    }
  }
}

void printGPSMark() {
  display.setCursor(122, 0);
  display.setTextSize(1);
  if ((millis() & 1) == 0) {
    display.println(F("/"));
  } else {
    display.println(F("\\"));
  }
}
void printSpeed() {
  display.setCursor(0, 4);
  display.setTextSize(4);
  if (gps.speed.isValid()) {
    sprintf(buffer, "%03d", round(gps.speed.kmph()));
    display.println(buffer);
  } else {
    display.print(F("000"));
  }
  display.setCursor(80, 20);
  display.setTextSize(2);
  display.print(F("km/h"));
}
void prinSatellites() {
  display.setCursor(80, 0);
  display.setTextSize(1);
  display.print(F("SAT:"));
  display.print(gps.satellites.value());
}