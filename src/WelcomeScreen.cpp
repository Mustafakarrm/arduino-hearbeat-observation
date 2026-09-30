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
    this->display->print("Heartbeat Observation");
    this->display->setCursor(4,10);
    this->display->print("Mustafa");
    //this->display->setCursor(16,15);
    //this->display->print("'n'");
    //this->display->setCursor(26,15);
    //this->display->print("Omar");
    this->isStarted = true;
    //endl
}
void WelcomeScreen::onUpdate(long currentMillis){
    if (this->display == nullptr || this->phase == nullptr)
        return;
    if (currentMillis - this->lastUpdate < WELCOME_SCREEN_INTERVAL_UPDATE )
        return;
    this->lastUpdate = currentMillis;
    this->setup();
    this->isInverted = !this->isInverted;
    this->display->invertDisplay(this->isInverted);

    this->display->display(); //endl

    
}