#ifndef _GPIO_ARDUINO_IMPLEMENTATION_H_
#define _GPIO_ARDUINO_IMPLEMENTATION_H_

#include "gpio.h"

typedef struct avrPort_s avrPort_t;

class gpioAVR : public gpio {
public:
    gpioAVR(volatile uint8_t *baseRegPort, uint8_t pin, gpio::gpioMode mode);
    void WriteState(bool state);
    bool ReadState(void);

private:
    avrPort_t *avrPort;
    uint8_t    pin;
};

#endif /* _gpio_ARDUINO_IMPLEMENTATION_H_ */