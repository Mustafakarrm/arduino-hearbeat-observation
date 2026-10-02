#include <WelcomeScreen.h>

WelcomeScreen::WelcomeScreen(){
    this->lastUpdate = 0;
}

void WelcomeScreen::pinScreen(Adafruit_SSD1306 &screen){
    this->display = &screen;
};

void WelcomeScreen::pinScreenPhase(int &currentPhase){
    this->phase = &currentPhase;
}

void WelcomeScreen::onSetup(){
    if (this->isStarted) //happens once after being called
        return;
    this->display->clearDisplay();
    this->display->setTextSize(1);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->setCursor(1,2);
    this->display->print(F("Heartbeat Observation"));
    this->display->setCursor(4,10);
    this->display->print(F("Mustafa"));
    this->display->setCursor(60,16);
    this->display->print(F("'n'"));
    this->display->setCursor(98,24);
    this->display->print(F("Omar"));
    this->isStarted = true;
    //endl
}
void WelcomeScreen::onUpdate(long currentMillis){
    if (this->display == nullptr || this->phase == nullptr)
        return;
    if (*this->phase != WELCOME_PHASE)
        return;
    if (currentMillis - this->lastUpdate < WELCOME_SCREEN_INTERVAL_UPDATE )
        return;
    this->lastUpdate = currentMillis;
    this->onSetup();
    this->isInverted = !this->isInverted;
    this->display->invertDisplay(this->isInverted);
    if (currentMillis >= WELCOME_SCREEN_TIMEOUT){
        *this->phase = SELECT_SCREEN_PHASE;
        this->onQuit();
    }
    this->display->display(); //endl

}

void WelcomeScreen::onQuit(){
        this->display->invertDisplay(false);
        this->isStarted = false;
}