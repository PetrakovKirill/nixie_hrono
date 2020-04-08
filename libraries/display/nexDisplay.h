#ifndef _NEX_DISPLAY_H_
#define _NEX_DISPLAY_H_

#include "display.h"
#include "timer_strategy.h"
#include "virtual_port.h"
#include "event.h"

#define VIDEOMEM_SIZE  (4)
#define PWM_MAX        (20)


typedef union nixieRaw_u nixieRaw_t;
union nixieRaw_u {
    uint8_t  digi[VIDEOMEM_SIZE];
    uint32_t word;
};


class nexDisplay : public display {
public:
    nexDisplay(timerStrategy *strategy, virtualPort *anodeDigis, virtualPort *decoderHV);
    void Show(bool state);
    void Print(char *str);
    void SetBrightDigis(uint8_t digitNum, uint8_t bright);
    void LoadRaw(nixieRaw_t rawImage);
    
    const char * LastPrint(void);

private:
    class timerEvent : public event {
    public:
        timerEvent(nexDisplay *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        nexDisplay *inst;
    };


    void DynamicIndication(void);
    // friend void TimerHandler(void *args);

    friend timerEvent;

    timerStrategy  *timStrategy;
    timerEvent      timEvent;
    timer           indcTimer;

    virtualPort    *anodeDigis;
    virtualPort    *decoderHV;
    nixieRaw_t      videoMem;

    uint32_t        digisMask;
    uint8_t         digitPwm[VIDEOMEM_SIZE];
    uint8_t         cntDigis, cnt;

    char lastPrint[VIDEOMEM_SIZE + 1];
    // char lastPrint[5];
};

nixieRaw_t NixieStringDecoder(char *str);

#endif /* _NEX_DISPLAY_H_ */

// Выбранный активен при первом запуске и меняется кнопками. Запоминается в память
// 0 - нет эффекта
// 1 - плавное угасание и появление (рекомендуемая скорость: 100-150)
// 2 - перемотка по порядку числа (рекомендуемая скорость: 50-80)
// 3 - перемотка по порядку катодов в лампе (рекомендуемая скорость: 30-50)
// 4 - поезд (рекомендуемая скорость: 50-170)
// 5 - резинка (рекомендуемая скорость: 50-150)