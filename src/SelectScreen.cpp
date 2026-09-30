#include <SelectScreen.h>

void SelectScreen::onUpdate(long currentMillis)
{
    if (*this->phase != SELECT_SCREEN_PHASE)
        return;
    
    if (currentMillis - lastUpdate < SELECT_SCREEN_INTERVAL_UPDATE)
        return;
    
    this->onSetup();


    this->display->display();
}

int SelectScreen::getSelection(){
    return this->selectedItem;
}

void SelectScreen::onSelect(int selection){
    if (*this->phase != SELECT_SCREEN_PHASE)
        return ;
    if (selection > MAX_SELECT_SCREEN_SELECTION || selection <= 0)
        return ;
    this->selectedItem = selection;
    this->refreshSelectionArrow();
    return ;
}

void SelectScreen::refreshSelectionArrow(){
    for (int i = 1 ; i <= MAX_SELECT_SCREEN_SELECTION ; i++){
            this->display->setCursor(8,8*i);
            this->display->setTextColor(SSD1306_BLACK);
            this->display->print(F(">"));
        }
    
    this->display->setCursor(8,8*this->selectedItem);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->print(F(">"));

    this->display->display();
}

void SelectScreen::onSetup()
{
    if (this->isStarted)
        return;
    this->display->clearDisplay();
    this->display->setCursor(16,8);
    this->display->print  (F("HBPM & Graph"));
    this->display->setCursor(16,16);
    this->display->print  (F("Stress Measure"));
    this->isStarted = true;
}

void SelectScreen::confirmSelection(){
    this->onQuit();
}
