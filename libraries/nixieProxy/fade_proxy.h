#ifndef _fadeProxy_h_
#define _fadeProxy_h_

/* 
 * Прокси-объект реализующий плавное изменение символов дисплея .
 *  */

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

    bool        isFadout;
    char        newPrint[(sizeof(nixieRaw_t) + 1)];
    const char *lastPrint;
    uint8_t     maskChange;
    uint8_t     cnt;

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



/* Визуализация затухания */
void fadeProxy :: Fadeout(void) {
    uint8_t i;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        if ( (1 << i) & maskChange) {
            displ->SetBrightDigis(i, PWM_MAX - cnt);
        }
    }

    if (cnt == PWM_MAX) {
        /* Предыдущая печать затухла, 
         * пора печатать новую */
        isFadout = false;
        displ->Print(newPrint);
    }

    cnt++;
    tim.ResetCounter();
    tim.Start();
}



/**
 *  Визуализация разгорания
 */
void fadeProxy :: Fadein(void) {
    uint8_t i;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        if ( (1 << i) & maskChange) {
            displ->SetBrightDigis(i, cnt - PWM_MAX);
        }
    }

    /* Разжигаем пока яркость не достигнет максимума */
    if (cnt - PWM_MAX < PWM_MAX) {
        cnt++;
        tim.ResetCounter();
        tim.Start();
    }
}



/**
 * Параметризация скорости плавного затухания и разгорания
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

    lastPrint = displ->LastPrint();

    maskChange = 0x00;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        /* Отмечаем в маске те разряды которые были изменены */
        if (lastPrint[i] != str[i]) {
            maskChange |= (1 << i);
        }
    }

    memcpy(newPrint, str, sizeof(nixieRaw_t));
    newPrint[sizeof(nixieRaw_t)] = '\0';

    cnt = 0;
    isFadout = true;
    Fadeout();
}


#endif /* _fadeProxy_h_ */