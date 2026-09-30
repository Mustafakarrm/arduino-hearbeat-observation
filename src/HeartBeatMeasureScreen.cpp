#include <HeartBeatMeasureScreen.h>

void HeartBeatMeasureScreen::onUpdate(long currentMillis){
    if (currentMillis - lastUpdate < HEARTBEAT_MEASURE_SCREEN_INTERVAL_UPDATE )
    return;
    this->drawDivider();
    if (this->isFingerOn)
    {
        this->drawHeart(NORMAL_HEART);
    }
    else
    {
        this->drawHeart(OUTLINED_HEART);
        this->printPlaceFinger();
        this->drawFlatLine();
    }
    
    this->display->display();
}



void HeartBeatMeasureScreen::onSetup(){}

void HeartBeatMeasureScreen::drawHeart(int state){
    switch (state) {
    case BEAT_HEART:
        this->display->fillCircle(4, 5, 4, SSD1306_WHITE);
        this->display->fillCircle(11, 5, 4, SSD1306_WHITE);
        this->display->fillTriangle(0, 7, 15, 7, 7, 14, SSD1306_WHITE);
        break;
    case NORMAL_HEART:
        this->display->fillCircle(4, 4, 3, SSD1306_WHITE);
        this->display->fillCircle(10, 4, 3, SSD1306_WHITE);
        this->display->fillTriangle(1, 5, 13, 5, 7, 11, SSD1306_WHITE);
        break;
    case OUTLINED_HEART:
        this->display->drawCircle(4, 4, 3, SSD1306_WHITE);
        this->display->drawCircle(10, 4, 3, SSD1306_WHITE);
        this->display->drawLine(1, 5, 7, 11, SSD1306_WHITE);
        this->display->drawLine(13, 5, 7, 11, SSD1306_WHITE);
        break;
  }
}

void HeartBeatMeasureScreen::setBPM(int bpm){
    if (!this->isFingerOn)
        return;
    if (bpm == 0)
        this->display->print(F("--"));
    else
        this->display->print(bpm);
}

void HeartBeatMeasureScreen::setFinger(bool isOn){
    this->isFingerOn = isOn;
}

void HeartBeatMeasureScreen::doBeat(){
    this->drawHeart(BEAT_HEART);
    this->display->display();
}

void HeartBeatMeasureScreen::printPlaceFinger() {
    this->display->setTextSize(1);
    this->display->setCursor(0, 17);
    this->display->print(F("Place"));
    this->display->setCursor(0, 25);
    this->display->print(F("finger"));
}

void HeartBeatMeasureScreen::drawDivider(){
    this->display->drawFastVLine(PANEL_WIDTH, 0, SCREEN_HEIGHT, SSD1306_WHITE);
}

void HeartBeatMeasureScreen::drawFlatLine(){
    this->display->drawFastHLine(0, GRAPH_TOP + GRAPH_HEIGHT / 2, SCREEN_WIDTH, SSD1306_WHITE);
}
