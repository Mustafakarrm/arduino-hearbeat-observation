#include <HeartBeatMeasureScreen.h>

void HeartBeatMeasureScreen::onUpdate(long currentMillis){
    if (this->display == nullptr || this->phase == nullptr)
        return;
    if (*this->phase != HEARTBEAT_MEASURE_PHASE)
        return;
    if (currentMillis - this->lastUpdate < HEARTBEAT_MEASURE_SCREEN_INTERVAL_UPDATE)
        return;
    this->onSetup();
    this->drawHeart();
    this->display->setCursor(48,16);
    if (this->isFingerOn){
        
    }
    else
    {
        this->display->print(F("Put Finger on"));
    }
    this->display->display();
    this->lastUpdate = currentMillis;
}

void HeartBeatMeasureScreen::onSetup(){
    if (!this->isStarted)
        this->isStarted = true;
    this->display->clearDisplay();
    this->display->setTextColor(SSD1306_WHITE);
}



void HeartBeatMeasureScreen::drawHeart(){
    if (isFingerOn){
        heartShape(32, 16, 6, SSD1306_WHITE);
    }
    else{
        heartShape(32, 16, 6, SSD1306_WHITE);      
        heartShape(32, 16 + 1, 6 - 2, SSD1306_BLACK); 
    }
}

void HeartBeatMeasureScreen::doBeat(){
    
    heartShape(32, 16, 8, SSD1306_WHITE);      
    
    this->display->display();
}

void HeartBeatMeasureScreen::setBPM(int currentBPM){
    this->bpm = currentBPM;
}

void HeartBeatMeasureScreen::setFinger(bool setFinger){
    this->isFingerOn = setFinger;
}

void HeartBeatMeasureScreen::heartShape(int cx, int cy, int r, uint16_t color) {
  if (r < 1) 
    return;
  int cxL = cx - r;                  
  int cxR = cx + r;                    
  int cyC = cy - (int)(0.707 * r);     

  int dx  = (int)(0.707 * r);
  int ly  = cyC + dx;                 
  int bot = ly + (int)(1.707 * r);     

  this->display->fillCircle(cxL, cyC, r, color);
  this->display->fillCircle(cxR, cyC, r, color);
  this->display->fillRect(cxL, cyC, 2 * r, dx + 1, color);
  this->display->fillTriangle(cxL - dx, ly, cxR + dx, ly, cx, bot, color);
}