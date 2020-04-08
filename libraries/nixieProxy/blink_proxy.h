#ifndef _blinkProxy_h_
#define _blinkProxy_h_


class blinkProxy {
public:
    blinkProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t blinkPeriod);
    
    void     SetPeriod(uint32_t period);
    uint32_t GetPeriod(void);

    void Enable(void);
    void Disable(void);

    bool IsEnable(void);

private:
    class timerEvent : public event {
    public:
        timerEvent(blinkProxy *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        blinkProxy *inst;
    };

    friend timerEvent;

    void blinkProxyEffect(void);

    timerStrategy *tStrategy;
    timerEvent     timEvent;
    timer          tim;

    bool        isActive;
    bool        isShow = true;
    nexDisplay *displ;
};



blinkProxy :: blinkProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t blinkPeriod = 300) :
                    timEvent(this), tim(&timEvent, blinkPeriod) {
    displ     = pDisplay;
    tStrategy = timStrategy;
    timStrategy->AddTimer(&tim);
    SetPeriod(blinkPeriod);
}



/* Обработчик события таймера
 * Внутри вызываются непосредстенно обработчики дейстия.
 * Для переключения ипользуем флаг glitchAction, который
 * отражает состояние глюка в реальном времени.
 */
void blinkProxy :: timerEvent :: EventHandler(void) {
    inst->blinkProxyEffect();
}



/**
 *  Визуальная реализация глюка
 */
void blinkProxy :: blinkProxyEffect(void) {
    uint8_t bright;
    isShow = !isShow;

    bright = (isShow) ? (PWM_MAX) : (0);

    displ->SetBrightDigis(0, bright);
    displ->SetBrightDigis(1, bright);
    displ->SetBrightDigis(2, bright);
    displ->SetBrightDigis(3, bright);

    tim.ResetCounter();
    tim.Start();
}



void blinkProxy :: SetPeriod(uint32_t period) {
    tim.SetInterval(period);
}



uint32_t blinkProxy :: GetPeriod(void) {
    return ( tim.GetInterval() );
}



void blinkProxy :: Enable(void) {
    tim.ResetCounter();
    tim.Start();
}



void blinkProxy :: Disable(void) {

    displ->SetBrightDigis(0, PWM_MAX);
    displ->SetBrightDigis(1, PWM_MAX);
    displ->SetBrightDigis(2, PWM_MAX);
    displ->SetBrightDigis(3, PWM_MAX);

    isShow = true;

    tim.Stop();
}



bool blinkProxy :: IsEnable(void) {
    return (isActive);
}


#endif /* _blinkProxy_h_ */