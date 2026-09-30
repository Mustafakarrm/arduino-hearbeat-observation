#ifndef SELECTSCREEN_H
#define SELECTSCREEN_H
#include <WelcomeScreen.h>

class SelectScreen : public WelcomeScreen
{
    private:
        int selectedItem = 0;
    protected:
        void onSetup();
    public:
        void onUpdate(long currentMillis);
};

#endif