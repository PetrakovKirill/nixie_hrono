#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "gpio.h"
#include "event.h"


class button {
public:

    enum buttonState_t {
        NONE = 0, CLICK, DOUBLE_CLICK, HOLD
    };

    button(void);
    button(gpio *inputPin);
    void Init(gpio *inputPin);
    void AddEvent(event *newEvent, buttonState_t state);
    void RemoveEvent(buttonState_t state);
    void Scan(void);
    void Reset(void);

private:
    class nopEvent : public event {
    public:
        void EventHandler(void);
    };

    friend nopEvent;

    uint8_t CalcClicks(void);

    static nopEvent nop;

    event *hold;
    event *click;
    event *doubleClick;

    uint16_t      mMemState;
    buttonState_t buttonState;
    buttonState_t last;
    bool          releaseState;
    gpio         *inputPin;
};

#endif /* _BUTTON_H_ */