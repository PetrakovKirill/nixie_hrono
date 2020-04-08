#ifndef _PINOUT_ARDUINO_IMPLEMENTATION_H_
#define _PINOUT_ARDUINO_IMPLEMENTATION_H_

#include "gpio.h"

class gpioArduino : public gpio {
public:
    gpioArduino(uint8_t pin, gpio::gpioMode mode);
    void WriteState(bool state);
    bool ReadState(void);

private:
    uint8_t pinBoard;
};

#endif /* _PINOUT_ARDUINO_IMPLEMENTATION_H_ */