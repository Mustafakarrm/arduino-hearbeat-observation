#include <SelectScreen.h>

void SelectScreen::onUpdate(long currentMillis)
{
    if (currentMillis - lastUpdate < SELECT_SCREEN_INTERVAL_UPDATE)
        return;
}

void SelectScreen::onSetup()
{
    if (this->isStarted)
        return;
    this->display->clearDisplay();
    this->display->setCursor(16,1);
    this->display->print  ("HBPM & Graph");
    this->display->println("Stress Measure");
    this->display->println("Splash Screen");    
}
