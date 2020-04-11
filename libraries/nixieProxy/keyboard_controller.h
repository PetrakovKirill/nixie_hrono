#ifndef _KEYBOARD_CONTROLLER_H_
#define _KEYBOARD_CONTROLLER_H_

/* 
 * Объект управвдения клавиатурой. 
 * Отслеживвает события кнопок и манипулирая объектами часов и контроллеров визуализации */

class keyboardController {
public:
    keyboardController(gpio *btn1, gpio *btn2, gpio *btn3, clock *main, visualController *visual);
    void Scan(void);
private:
    class switchClockEvent : public event {
    public:
        switchClockEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->SwitchClockHandler(); }
    private:
        keyboardController *inst;
    };

    class switchAdjstEvent : public event {
    public:
        switchAdjstEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->SwitchAdjstHandler(); }
    private:
        keyboardController *inst;
    };

    class switchGlitchEvent : public event {
    public:
        switchGlitchEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->SwitchGlitchHandler(); }
    private:
        keyboardController *inst;
    };

    class incrementEvent : public event {
    public:
        incrementEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->IncrementHandler(); }
    private:
        keyboardController *inst;
    };

    class decrementEvent   :public event {
    public:
        decrementEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->DecrementHandler(); }
    private:
        keyboardController *inst;
    };

    class switchEffectEvent : public event {
    public:
        switchEffectEvent(keyboardController *pInst) : inst(pInst) {}
        void EventHandler() { inst->SwitchEffectHandler(); }

    private:
        keyboardController *inst;
    };

    friend switchClockEvent;
    friend switchAdjstEvent;
    friend switchGlitchEvent;
    friend incrementEvent;
    friend decrementEvent;
    friend switchEffectEvent;

    void SwitchClockHandler(void);
    void SwitchAdjstHandler(void);
    void SwitchGlitchHandler(void);
    void IncrementHandler(void);
    void DecrementHandler(void);
    void SwitchEffectHandler(void);

    switchClockEvent    switchClock;
    switchAdjstEvent    switchAdjst;
    switchGlitchEvent   switchGlitch;
    incrementEvent      increment;
    decrementEvent      decrement;
    switchEffectEvent   switchEffect;

    button key1;
    button key2;
    button key3;

    visualController *visualControl;

    bool isMain    = true;
    bool adjIsHour = true;

    clock           *mainClock;
    adjustmentClock  adjClock;
};



keyboardController :: keyboardController(gpio *btn1, gpio *btn2, gpio *btn3, clock *main, visualController *visual) :
    /* Инициализируем объекты-события */
    switchClock(this), switchAdjst(this), switchGlitch(this), increment(this), decrement(this), switchEffect(this), 
    /* Управляющие кнопки */
    key1(btn1), key2(btn2), key3(btn3), 
    /* Подставные часы для отображения настройки */
    adjClock(visual->GetDisplay()) { 
    
    mainClock     = main; 
    visualControl = visual; 

    /* Удержать "выбор" - настройка времени */
    key1.AddEvent(&switchClock, button::HOLD);
    
    /* Удержание центральной кнопки включает и выключает "глюки" */
    key2.AddEvent(&switchGlitch, button::HOLD);
    
    /* Клик по правой кнопке переключает режимы перелистывания цифр */
    key3.AddEvent(&switchEffect, button::CLICK);

    // Serial.println(__FUNCTION__);
}



void keyboardController :: Scan(void) {
    key1.Scan(); key2.Scan(); key3.Scan();
}



void keyboardController :: SwitchClockHandler(void) {
    isMain = !isMain;

    /* Врубаем мигание */
    if (isMain) {
        /* Удержать "выбор" - настройка времени */
        key1.RemoveEvent(button::CLICK);
        key2.RemoveEvent(button::CLICK);
        
        /* Клик по правой кнопке переключает режимы перелистывания цифр */
        key3.AddEvent(&switchEffect, button::CLICK);
       
        /* Отрубаем блокировку, чтобы отображалось реальное время */
        visualControl->SwitchBlink(false);
        visualControl->PrintBlock(false);

        /* Обноляем время на часах */
        mainClock->SetTime(adjClock.GetTime());

        /* В следующий раз настройка начнется с часов */
        adjIsHour = true;
        adjClock.AdjustmentHour();

        /* показываем текущее время */
        mainClock->ShowTime();
    } else {
        /* Выбор: часы / минуты */
        key1.AddEvent(&switchAdjst, button::CLICK);
        
        /* Больше */
        key2.AddEvent(&increment, button::CLICK);
        
        /* Меньше */
        key3.AddEvent(&decrement, button::CLICK);

        /* Блокируем вывод на дисплей основных часов,
         * и врубаем мигание */
        visualControl->SwitchBlink(true);
        visualControl->PrintBlock(true);

        /* Получаем текущее время */
        adjClock.SetTime(mainClock->GetTime());

        /* Отображаем настройку */
        adjClock.ShowTime();
    }
}



void keyboardController :: SwitchAdjstHandler(void) {
    adjIsHour = !adjIsHour;
    if (adjIsHour) {
        adjClock.AdjustmentHour();
    } else {
        adjClock.AdjustmentMinutes();
    }
    adjClock.ShowTime();
}



void keyboardController :: SwitchGlitchHandler(void) {
    visualControl->SwitchGlitch(!visualControl->GlitchIsOn());
}



void keyboardController :: IncrementHandler(void) {
    adjClock.Increment();
    adjClock.ShowTime();
}



void keyboardController :: DecrementHandler(void) {
    adjClock.Decrement();
    adjClock.ShowTime();
}



void keyboardController :: SwitchEffectHandler(void) {
    visualControl->SwitchNext();
    visualControl->Print((char *)"    ");
    mainClock->ShowTime();
}


#endif /* _KEYBOARD_CONTROLLER_H_ */