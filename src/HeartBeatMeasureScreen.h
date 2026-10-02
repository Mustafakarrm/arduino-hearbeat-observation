#ifndef HEARTBEATMEASURESCREEN_H
#define HEARTBEATMEASURESCREEN_H

#include <WelcomeScreen.h>

class HeartBeatMeasureScreen : public WelcomeScreen
{
    private:
        int bpm = 0;
        bool isFingerOn = false;
        void drawHeart();
        void heartShape(int cx, int cy, int r, uint16_t color);
    protected:
        void onSetup();
    public:
        void onUpdate(long currentMillis);
        void setFinger(bool isFingerOn);
        void setBPM(int bpm);
        void doBeat();
};

#endif