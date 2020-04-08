#ifndef _displayStrategy_h_
#define _displayStrategy_h_


class displayStrategy : public display {
public:
    displayStrategy(display **array, uint8_t quantity, glitchProxy *glitch, blinkProxy *blink);
    void    SetStrategy(uint8_t numStrategy);
    uint8_t GetStrategy(void);
    void    SwitchNext(void);

    void SwitchGlitch(bool state);
    bool GlitchIsOn(void);

    void SwitchBlink(bool state);
    bool BlinkIsOn(void);

    void PrintBlock(bool block);
    bool PrintIsBlock(void);

    display * GetDisplay(void);

    void Show(bool state);
    void Print(char *str);

private:
    glitchProxy   *glitch;
    blinkProxy    *blink;

    bool      isBlock = false;

    uint8_t   cntDisplays;
    uint8_t   currentIndexDisplay;
    display  *current;
    display **displs;
};



displayStrategy :: displayStrategy(display **array, uint8_t quantity, glitchProxy *glitch, blinkProxy *blink) {
    this->glitch = (glitch);
    this->blink  = (blink);
    cntDisplays  = (quantity);
    displs       = array;
    SetStrategy(0);
}



void displayStrategy :: SetStrategy(uint8_t numStrategy) {
    if (numStrategy < cntDisplays) {
        currentIndexDisplay = numStrategy;
        current = displs[currentIndexDisplay];
    }
}



uint8_t displayStrategy :: GetStrategy(void) {
    return (currentIndexDisplay);
}



void displayStrategy :: SwitchNext(void) {
    currentIndexDisplay = (currentIndexDisplay + 1) % cntDisplays;
    current = displs[currentIndexDisplay];
}


void displayStrategy :: SwitchGlitch(bool state) {
    if (glitch) {
        if (state) {
            glitch->Enable();
        } else {
            glitch->Disable();
        }
    }
}



void displayStrategy :: SwitchBlink(bool state) {
    if (blink) {
        if (state) {
            blink->Enable();
        } else {
            blink->Disable();
        }
    }
}



bool displayStrategy :: GlitchIsOn(void) {
    bool result = false;
    if (glitch) {
        result = glitch->IsEnable();
    }

    return (result);
}



bool displayStrategy :: BlinkIsOn(void) {
    bool result = false;
    if (glitch) {
        result = blink->IsEnable();
    }

    return (result);
}



void displayStrategy :: PrintBlock(bool block) {
    isBlock = block;
}



bool displayStrategy :: PrintIsBlock(void) {
    return (isBlock);
}



display * displayStrategy :: GetDisplay(void) {
    return (current);
}



void displayStrategy :: Show(bool state) {
    displs[currentIndexDisplay]->Show(state);
}



void displayStrategy :: Print(char *str) {
    if (isBlock == false) {
        current->Print(str);
    }
}

#endif /* _displayStrategy_h_ */