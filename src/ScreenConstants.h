#ifndef SCREENCONSTANTS_H
#define SCREENCONSTANTS_H

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define WELCOME_PHASE 100
#define WELCOME_SCREEN_INTERVAL_UPDATE 500
#define WELCOME_SCREEN_TIMEOUT 5000
#define SELECT_SCREEN_PHASE 200
#define SELECT_SCREEN_INTERVAL_UPDATE 250 
#endif