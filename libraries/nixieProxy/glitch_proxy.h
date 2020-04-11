#ifndef _glitchProxy_h_
#define _glitchProxy_h_

/* 
 * Прокси объект реализующий glitch-эффект.
 * Эффект носит случайный характер с заданной вероятностью.
 * 1 глюк - это последовательность случайных миганий.
 * Период миганий из последовательности изменяется слчайным образом.
 *  */

class glitchProxy {
public:
    glitchProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t period);
    void Possibility(uint8_t value);
    void Period(uint8_t value);
    void Enable(void);
    void Disable(void);
    bool IsEnable(void);

private:
    class timerEvent : public event {
    public:
        timerEvent(glitchProxy *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        glitchProxy *inst;
    };

    friend timerEvent;

    void CalcEvent(void);
    void GlitchProxyEffect(void);

    timerStrategy *tStrategy;
    timerEvent     timEvent;
    timer          tim;

    bool           glitchAction = false;
    bool           isEnable;

    uint8_t        possibility = 10;
    uint32_t       period      = 3000;

    uint8_t        glitchProxyCnt;

    nexDisplay    *displ;
};



glitchProxy :: glitchProxy(nexDisplay *pDisplay, timerStrategy *timStrategy, uint32_t period) :
                    timEvent(this), tim(&timEvent, period) {
    displ     = pDisplay;
    tStrategy = timStrategy;

    timStrategy->AddTimer(&tim);
    Disable();
}



/* Обработчик события таймера
 * Внутри вызываются непосредстенно обработчики действия.
 * Для переключения ипользуем флаг glitchAction, который
 * отражает состояние глюка в реальном времени.
 */
void glitchProxy :: timerEvent :: EventHandler(void) {
    if (inst->glitchAction) {
        inst->GlitchProxyEffect();
    } else {
        inst->CalcEvent();
    }
}



/**
 * Включение эффекта
 */
void glitchProxy :: Enable(void) {
    isEnable = true;

    /* При ввключении демонсрируем эффект */
    glitchAction = true;

    /* Задаем длинну последовательности */
    glitchProxyCnt = random(3, 5);
    GlitchProxyEffect();
}



/**
 * Отключение эффекта
 */
void glitchProxy :: Disable(void) {
    isEnable = false;
    tim.Stop();
}



/**
 * Состояние активности прокси-объекта
 */
bool glitchProxy :: IsEnable(void) {
    return (isEnable);
}



/**
 * Установка вероятности появления эффекта (от 0 до 100).
 */
void glitchProxy :: Possibility(uint8_t value) {
    possibility = (value < 100) ? (value) : (100);
}


/**
 * Вычисление события глюка.
 * Определяем произошел ли глюк.
 */
void glitchProxy :: CalcEvent(void) {
    int32_t rnd;

    /* генерируем случайное событие глюка */
    rnd = random(0, 100) + possibility;
    Serial.println(rnd);
    if (rnd < 100) {
        glitchAction = false;
        tim.ResetCounter();
        tim.Start();
        return;
    }

    /* Переключаем флаг, для визуальной реализации глюка */
    glitchAction = true;
    
    /* Задаем длинну последовательности */
    glitchProxyCnt = random(1, 5);

    GlitchProxyEffect();
}



/**
 *  Визуальная реализация глюка
 */
void glitchProxy :: GlitchProxyEffect(void) {
    static uint8_t digit = 0;

    /* Восстаналиваем яркость индикатора */
    displ->SetBrightDigis(digit, PWM_MAX);

    /* Выбираем случайный индикатор */
    digit = random(0, 4);

    displ->SetBrightDigis(digit, random(0, PWM_MAX/2));


    if (--glitchProxyCnt) {
        /* Определяем время воспроизведения следующего глюка из 
         * определеной последовательности */
        tim.SetInterval(random(30, 150));
    } else {
        /* Последовательность закончилась, возвращаем период */
        glitchAction = false;
        tim.SetInterval(period);
        displ->SetBrightDigis(digit, PWM_MAX);
    }

    tim.ResetCounter();
    tim.Start();
}



/**
 * Параметризация периода расчета появления события "Глюк"
 */
void glitchProxy :: Period(uint8_t value) {
    period = value;
    tim.SetInterval(value);
}

#endif /* _glitchProxy_h_ */