#ifndef SELECTSCREEN_H
#define SELECTSCREEN_H
#include <WelcomeScreen.h>

class SelectScreen : public WelcomeScreen
{
    private:
        int     selectedItem = 1;
        bool    isArrowVisible = false; 
        void    refreshSelectionArrow();
    protected:
        void    onSetup();
    public:
        int     getSelection();
        void    onSelect(int selection);
        void    confirmSelection();
        void    onUpdate(long currentMillis);

};

#endif