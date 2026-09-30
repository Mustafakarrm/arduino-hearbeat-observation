#include <Arduino.h>
#include <ScreenConstants.h>
#include <Wire.h>
#include <WelcomeScreen.h>
#include <SelectScreen.h>
#include <HeartBeatMeasureScreen.h>
//#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WelcomeScreen welcomeScreen;
SelectScreen selectScreen;
HeartBeatMeasureScreen heartBeatMeasureScreen;
int currentPhase = WELCOME_PHASE;
char input = 0;
void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Wire.setClock(400000);
  welcomeScreen.pinScreen(display);
  welcomeScreen.pinScreenPhase(currentPhase);
  selectScreen.pinScreen(display);
  selectScreen.pinScreenPhase(currentPhase);
  heartBeatMeasureScreen.pinScreen(display);
  heartBeatMeasureScreen.pinScreenPhase(currentPhase);
  
}

void loop(){
  if (Serial.available() > 0){
    input = Serial.read();
    if (input == '\n' || input == '\r')
    {
      
    }
    else
    {
      selectScreen.onSelect(input-'0');
    }
  }
  
  welcomeScreen.onUpdate(millis());
  selectScreen.onUpdate(millis());
  heartBeatMeasureScreen.onUpdate(millis());
}