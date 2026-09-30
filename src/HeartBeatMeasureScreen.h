#ifndef HEARTBEATMEASURESCREEN_H
#define HEARTBEATMEASURESCREEN_H

#include <WelcomeScreen.h>

class HeartBeatMeasureScreen : public WelcomeScreen
{
    private:
        void drawHeart(int state); 
        void drawDivider();
        void drawFlatLine();
        void drawWaveForm(int minValue, int range);
        bool isFingerOn = false;
        void printPlaceFinger();
    protected:
        void onSetup();
    public:
        void setBPM(int bpm);
        void doBeat();
        void setFinger(bool isFingerOn);
        void onUpdate(long currentMillis);
};

#endif