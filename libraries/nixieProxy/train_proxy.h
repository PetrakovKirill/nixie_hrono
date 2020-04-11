#ifndef _trainProxy_h_
#define _trainProxy_h_


/* 
 * Прокси-объект реализующий эффект заезжающей строки .
 *  */

class trainProxy : public display {
public:
    trainProxy(nexDisplay *pDisplay, timerStrategy *timStrategy,  uint32_t period);
    void     SetPeriod(uint32_t value);
    uint32_t GetPeriod(void);
    void     Show(bool state);
    void     Print(char *str);

private:
    class timerEvent : public event {
    public:
        timerEvent(trainProxy *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        trainProxy *inst;
    };

    friend timerEvent;

    void Effect(void);

    timerStrategy *tStrategy;
    timerEvent     timEvent;
    timer          tim;

    uint8_t cnt;
    char    buffer[(sizeof(nixieRaw_t) * 3) + 1];

    nexDisplay    *displ;
};



trainProxy :: trainProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t period) :
                    timEvent(this), tim(&timEvent, period) {
    displ     = pDisplay;
    tStrategy = timStrategy;

    timStrategy->AddTimer(&tim);
}



void trainProxy :: timerEvent :: EventHandler(void) {
    inst->Effect();
}



void trainProxy :: Effect(void) {
    displ->Print(&buffer[cnt]);
    Serial.println(&buffer[cnt]);
    if (++cnt < 8) {
        tim.ResetCounter();
        tim.Start();
    }
}



/**
 * Параметризация скорости эффекта
 */
void trainProxy :: SetPeriod(uint32_t value) {
    tim.SetInterval(value);
}



uint32_t trainProxy :: GetPeriod(void) {
    return (tim.GetInterval());
}



/**
 * Вкл.Выкл.
 */
void trainProxy :: Show(bool state) {
    displ->Show(state);
}



/* Функция печати интерфейса display */
void trainProxy :: Print(char *str) {
    uint8_t length;

    const char *lastPrint = displ->LastPrint();
    length = strlen(lastPrint);

    memcpy(&buffer[0],              lastPrint, length);
    memset(&buffer[length],         ' ',       length);
    memcpy(&buffer[length +length], str,       length);
    cnt = 0;
    Effect();
}


#endif /* _trainProxy_h_ */