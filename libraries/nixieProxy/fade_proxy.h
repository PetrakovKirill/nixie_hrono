#ifndef _fadeProxy_h_
#define _fadeProxy_h_

class fadeProxy : public display {
public:
    fadeProxy(nexDisplay *pDisplay, timerStrategy *timStrategy,  uint32_t period);
    void     SetPeriod(uint32_t value);
    uint32_t GetPeriod(void);
    void     Show(bool state);
    void     Print(char *str);

private:
    class timerEvent : public event {
    public:
        timerEvent(fadeProxy *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        fadeProxy *inst;
    };

    friend timerEvent;

    void Fadeout(void);
    void Fadein(void);

    timerStrategy *tStrategy;
    timerEvent     timEvent;
    timer          tim;

    bool       isFadout;
    char       lastPrint[(sizeof(nixieRaw_t) + 1)];
    uint8_t    maskChange;
    uint8_t    cnt;

    nexDisplay    *displ;
};



fadeProxy :: fadeProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t period) :
                    timEvent(this), tim(&timEvent, period) {
    displ     = pDisplay;
    tStrategy = timStrategy;

    timStrategy->AddTimer(&tim);
}



void fadeProxy :: timerEvent :: EventHandler(void) {
    if (inst->isFadout) {
        inst->Fadeout();
    } else {
        inst->Fadein();
    }
}




void fadeProxy :: Fadeout(void) {
    uint8_t i;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        if ( (1 << i) & maskChange) {
            displ->SetBrightDigis(i, PWM_MAX - cnt);
        }
    }

    if (cnt == PWM_MAX) {
        isFadout = false;
        displ->Print(lastPrint);
    }

    cnt++;
    tim.ResetCounter();
    tim.Start();
}







/**
 *  Визуальная реализация глюка
 */
void fadeProxy :: Fadein(void) {
    uint8_t i;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        if ( (1 << i) & maskChange) {
            displ->SetBrightDigis(i, cnt - PWM_MAX);
        }
    }

    if (cnt - PWM_MAX < PWM_MAX) {
        cnt++;
        tim.ResetCounter();
        tim.Start();
    }
}





/**
 * Параметризация периода расчета появления события "Глюк"
 */
void fadeProxy :: SetPeriod(uint32_t value) {
    tim.SetInterval(value);
}



uint32_t fadeProxy :: GetPeriod(void) {
    return (tim.GetInterval());
}



/**
 * Вкл.Выкл.
 */
void fadeProxy :: Show(bool state) {
    displ->Show(state);
}



/* Функция печати интерфейса display */
void fadeProxy :: Print(char *str) {
    uint8_t i;

    maskChange = 0x00;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        /* Отмечаем в маске те разряды которые были изменены */
        if (lastPrint[i] != str[i]) {
            maskChange |= (1 << i);
        }
    }

    memcpy(lastPrint, str, sizeof(nixieRaw_t));
    lastPrint[sizeof(nixieRaw_t)] = '\0';

    cnt = 0;
    isFadout = true;
    Fadeout();
}


#endif /* _fadeProxy_h_ */