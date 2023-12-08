#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1          //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c    //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;
char buffer[10];
 
void setup()
{
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  delay(200);
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
  printSatellites();
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

void printCourse() {
  if (gps.course.isValid()) { 
    float delta = 45 / 2;
    display.setCursor(80, 10);
    display.setTextSize(1);
    display.print(F("DIR: "));
    if (gps.course.deg() >= (360 - delta) || gps.course.deg() <= delta) {
      display.print("N ");
    } else if (gps.course.deg() >= (45 - delta) && gps.course.deg() <= (45 + delta)) {
      display.print("NE");
    } else if (gps.course.deg() >= (90 - delta) && gps.course.deg() <= (90 + delta)) {
      display.print("E ");
    } else if (gps.course.deg() >= (135 - delta) && gps.course.deg() <= (135 + delta)) {
      display.print("SE");
    } else if (gps.course.deg() >= (180 - delta) && gps.course.deg() <= (180 + delta)) {
      display.print("S ");
    } else if (gps.course.deg() >= (225 - delta) && gps.course.deg() <= (225 + delta)) {
      display.print("SW");
    } else if (gps.course.deg() >= (270 - delta) && gps.course.deg() <= (270 + delta)) {
      display.print("W ");
    } else if (gps.course.deg() >= (315 - delta) && gps.course.deg() <= (315 + delta)) {
      display.print("NW");
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
void printSatellites() {
  display.setCursor(80, 0);
  display.setTextSize(1);
  display.print(F("SAT:"));
  display.print(gps.satellites.value());
}
