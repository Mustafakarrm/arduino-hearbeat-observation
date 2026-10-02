#include <Arduino.h>
#include <ScreenConstants.h>
#include <Wire.h>
#include <WelcomeScreen.h>
#include <SelectScreen.h>
#include <HeartBeatMeasureScreen.h>
#include <FakePulseSensor.h>
#include <Adafruit_SSD1306.h>
#define HEARTLEDSTATES 3
#define BUTTON 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WelcomeScreen welcomeScreen;
SelectScreen selectScreen;
HeartBeatMeasureScreen heartBeatMeasureScreen;
FakePulseSensor fakePulseSensor;
int currentPhase = WELCOME_PHASE;
char input = 0;
bool finger = false;
int ledbr = 0;

FakePulseSensor sensor(5, 8, 3, LED_BUILTIN);
 
void onBeat(uint16_t bpm) {
    Serial.print(F("Beat detected, BPM: "));
    Serial.println(bpm);
    ledbr = map (bpm, 55, 110, 0, 255);
    analogWrite(HEARTLEDSTATES, ledbr);
    heartBeatMeasureScreen.doBeat();
    heartBeatMeasureScreen.setBPM(bpm);
    heartBeatMeasureScreen.setBuzzer(5);
    heartBeatMeasureScreen.setTone(3000,50);
}

void onFinger(bool on) {
    Serial.println(on ? F("Finger ON - measuring...") : F("Finger OFF"));
    finger = on;
    heartBeatMeasureScreen.setFinger(on);
}

void setup(){
  Serial.begin(9600);
  pinMode(HEARTLEDSTATES, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  Wire.setClock(400000);
  welcomeScreen.pinScreen(display);
  welcomeScreen.pinScreenPhase(currentPhase);
  selectScreen.pinScreen(display);
  selectScreen.pinScreenPhase(currentPhase);
  heartBeatMeasureScreen.pinScreen(display);
  heartBeatMeasureScreen.pinScreenPhase(currentPhase);
  sensor.begin();
  sensor.onBeat(onBeat);
  sensor.onFinger(onFinger);
}

void loop(){
  if (Serial.available() > 0){
    input = Serial.read();
    if (input == '\n' || input == '\r')
    {
      selectScreen.confirmSelection();
      heartBeatMeasureScreen.doBeat();
    }
    else if (input == 'f' || input == 'F'){
      if (currentPhase != HEARTBEAT_MEASURE_PHASE)
        return;
      finger = !finger;
      heartBeatMeasureScreen.setFinger(finger);
      if (finger)
        sensor.startMeasuring();
      else 
        sensor.stopMeasuring();
    }
    else
    {
      selectScreen.onSelect(input-'0');
    }
  }
  if(digitalRead(BUTTON) == LOW){
    if (currentPhase != HEARTBEAT_MEASURE_PHASE)
        return;
    finger = !finger;
    heartBeatMeasureScreen.setFinger(finger);
    if (finger)
        sensor.startMeasuring();
    else 
        sensor.stopMeasuring();
    delay(200);
  }

  
  welcomeScreen.onUpdate(millis());
  selectScreen.onUpdate(millis());
  heartBeatMeasureScreen.onUpdate(millis());
  sensor.update();
  delay(10);
}
