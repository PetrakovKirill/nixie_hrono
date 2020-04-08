#include "button.h"
#include <stddef.h>


button::nopEvent button::nop;


button :: button(void) {
    hold        = (event *)&nop;
    click       = (event *)&nop;
    doubleClick = (event *)&nop;
}


button :: button(gpio *inputPin) {
    Init(inputPin);
}


void button :: Init(gpio *inputPin) {
    this->inputPin  = inputPin;
    releaseState    = inputPin->ReadState();
    Reset();
}



void button :: Scan(void) {
    /* mMemState - битовая память состояний кнопки,
     * Работает как сдиговый регистр */
    mMemState <<= 1;
    mMemState  |= inputPin->ReadState() != releaseState;

    /* TODO:
     * В механизме есть изъян. После HOLD почти всегда следует СLICK.
     * так как часть маски успевает заполнииться. Поэтому используется дополнительная
     * переменная last, которая нужна для проверки: при получении события CLICK, если предыдущее
     * было HOLD, то пропустить его. */

    if (mMemState == 0xFFFF) {
        /* Все биты == 1, значит кнопку удерживают */
        buttonState = HOLD;
        last = buttonState;
    } else if (mMemState & (1 << 15)) {
        /* Считаем клики */
        uint8_t clicks = CalcClicks();
        if (clicks > 1) {
            buttonState = DOUBLE_CLICK;
            last = buttonState;
        } else if ( (clicks == 1) && (last != HOLD) ) {
            buttonState = CLICK;
            last = buttonState;
        } else {
            last = CLICK;
            Reset();
        }
    }

    switch (buttonState) {
        case     HOLD:         hold->EventHandler();        Reset(); break;
        case     CLICK:        click->EventHandler();       Reset(); break;
        case     DOUBLE_CLICK: doubleClick->EventHandler(); Reset(); break;
        default:                                                     break;
    }
}



void button :: Reset(void) {
    mMemState   = 0x00;
    buttonState = NONE;
}



uint8_t button :: CalcClicks(void) {
    uint8_t i, result = 0;
    uint16_t tmp;
    /* проходим по памяти состояний и считаем
     * сколько было состояний '0b10' */
    for (i = 0; i < 15; ++i) {
        tmp = (mMemState >> i) & 0b11;
        result += (tmp == 0b10) ? (1) : (0);
    }

    return (result);
}



void button :: AddEvent(event *newEvent, buttonState_t state) {
    if (newEvent == NULL) {
        newEvent = (event *)&nop;
    }

    switch (state) {
        case     HOLD:         hold        = newEvent; break;
        case     CLICK:        click       = newEvent; break;
        case     DOUBLE_CLICK: doubleClick = newEvent; break;
        default:                                       break;
    }
}



void button :: RemoveEvent(buttonState_t state) {
    AddEvent(NULL, state);
}


void button :: nopEvent :: EventHandler(void) { }