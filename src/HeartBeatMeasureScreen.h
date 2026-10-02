#ifndef HEARTBEATMEASURESCREEN_H
#define HEARTBEATMEASURESCREEN_H

#include <Arduino.h>
#include <WelcomeScreen.h>

class HeartBeatMeasureScreen : public WelcomeScreen
{
    private:
        int bpm = 0;
        bool isFingerOn = false;
        int buzzerPin = 5;                  
        unsigned int toneFrequency = 1800;   
        unsigned long toneDuration = 40;    
        void drawHeart();
        void heartShape(int cx, int cy, int r, uint16_t color);
    protected:
        void onSetup();
    public:
        void onUpdate(long currentMillis);
        void setFinger(bool isFingerOn);
        void setBPM(int bpm);
        void doBeat();
        void setBuzzer(int pin);
        void setTone( int frequency, long duration);
};

#endif