#ifndef _visual_controller_h_
#define _visual_controller_h_

/* Прокси-объект, агрегирующий другие прокси-объекты отвечающие за визуализацию.
 * Объект управляет текущей визуализацией. */

class visualController : public display {
public:
    visualController(display **array, uint8_t quantity, glitchProxy *glitch, blinkProxy *blink);
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



visualController :: visualController(display **array, uint8_t quantity, glitchProxy *glitch, blinkProxy *blink) {
    this->glitch = (glitch);
    this->blink  = (blink);
    cntDisplays  = (quantity);
    displs       = array;
    SetStrategy(0);
}



void visualController :: SetStrategy(uint8_t numStrategy) {
    if (numStrategy < cntDisplays) {
        currentIndexDisplay = numStrategy;
        current = displs[currentIndexDisplay];
    }
}



uint8_t visualController :: GetStrategy(void) {
    return (currentIndexDisplay);
}



void visualController :: SwitchNext(void) {
    currentIndexDisplay = (currentIndexDisplay + 1) % cntDisplays;
    current = displs[currentIndexDisplay];
}


void visualController :: SwitchGlitch(bool state) {
    if (glitch) {
        if (state) {
            glitch->Enable();
        } else {
            glitch->Disable();
        }
    }
}



void visualController :: SwitchBlink(bool state) {
    if (blink) {
        if (state) {
            blink->Enable();
        } else {
            blink->Disable();
        }
    }
}



bool visualController :: GlitchIsOn(void) {
    bool result = false;
    if (glitch) {
        result = glitch->IsEnable();
    }

    return (result);
}



bool visualController :: BlinkIsOn(void) {
    bool result = false;
    if (glitch) {
        result = blink->IsEnable();
    }

    return (result);
}



void visualController :: PrintBlock(bool block) {
    isBlock = block;
}



bool visualController :: PrintIsBlock(void) {
    return (isBlock);
}



display * visualController :: GetDisplay(void) {
    return (current);
}



void visualController :: Show(bool state) {
    displs[currentIndexDisplay]->Show(state);
}



void visualController :: Print(char *str) {
    if (isBlock == false) {
        current->Print(str);
    }
}

#endif /* _visual_controller_h_ */