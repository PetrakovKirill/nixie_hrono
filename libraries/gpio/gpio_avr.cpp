#include <avr/io.h>
#include "gpio_avr.h"


struct avrPort_s {
    volatile uint8_t pin;
    volatile uint8_t ddr;
    volatile uint8_t port;
};


gpioAVR :: gpioAVR(volatile uint8_t *baseRegPort, uint8_t pin, gpio::gpioMode mode) {
    avrPort = (avrPort_t *)(baseRegPort - 2);
    this->pin = pin;
    
    /* Init Port on output */
    switch (mode) {
    case gpio::IN_FLOAT:
    case gpio::IN_PULLDOWN: 
    case gpio::OUT_OD_FLOAT: 
        avrPort->ddr &= ~(1 << pin);
        WriteState(false);
        break;

    case gpio::IN_PULLUP: 
    case gpio::OUT_OD_PULLUP: 
        avrPort->ddr &= ~(1 << pin);
        WriteState(true);
        break;

    case gpio::OUT_PP: 
        avrPort->ddr |= (1 << pin);
        break;
    
    default:
        avrPort->ddr &= ~(1 << pin);
        WriteState(false);
        break;
}
}



void gpioAVR :: WriteState(bool state) {
    if (state) {
        avrPort->port |= (1 << pin);
    } else {
        avrPort->port &= ~(1 << pin);
    }
}



bool gpioAVR :: ReadState(void) {
    return ( !!(avrPort->pin & (1 << pin)) );
}


