#include "gpio_arduino.h"
#include "Arduino.h"


gpioArduino :: gpioArduino(uint8_t pin, gpio::gpioMode mode = gpio::IN_FLOAT) {
    pinBoard = pin;
    
    switch (mode) {
    case gpio::IN_FLOAT:
    case gpio::IN_PULLDOWN: 
    case gpio::OUT_OD_FLOAT: 
        pinMode(pinBoard, INPUT);
        break;

    case gpio::IN_PULLUP: 
    case gpio::OUT_OD_PULLUP: 
        pinMode(pinBoard, INPUT_PULLUP);
        break;

    case gpio::OUT_PP: 
        pinMode(pinBoard, OUTPUT);
        break;
    
    default:
        pinMode(pinBoard, INPUT);
        break;
    }
}



void gpioArduino :: WriteState(bool state) {
    digitalWrite(pinBoard, state);
}



bool gpioArduino :: ReadState(void) {
    return (digitalRead(pinBoard));
}