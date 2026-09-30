#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H
#include <ScreenConstants.h>
#include <Adafruit_SSD1306.h>

class WelcomeScreen {
    protected:
        Adafruit_SSD1306* display = nullptr;
        int* phase = nullptr;
        long lastUpdate;
        bool isStarted = false;
        void setup();
    private:
        bool isInverted = false;
    public:
        WelcomeScreen();
        void pinScreen(Adafruit_SSD1306 &screen);
        void pinScreenPhase(int &phase);
        void update(long currentmillis);
};

#endif