#include <Arduino.h>
#include <ScreenConstants.h>
#include <Wire.h>
#include <WelcomeScreen.h>
//#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WelcomeScreen welcomeScreen;
int currentPhase = WELCOME_PHASE;
void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  welcomeScreen.pinScreen(display);
  welcomeScreen.pinScreenPhase(currentPhase);
}

void loop(){
  welcomeScreen.update(millis());
}