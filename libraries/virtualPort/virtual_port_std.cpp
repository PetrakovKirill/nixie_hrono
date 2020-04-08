#include "virtual_port_std.h"


vPortGpio :: vPortGpio(gpio **arrayGpio, uint8_t size) {
    vPort = arrayGpio;
    sizePort = (size < 32) ? (size) : (31);
}



void vPortGpio :: Write (uint32_t value) {
    uint8_t i;
    for(i = 0; i < sizePort; ++i) {
        vPort[i]->WriteState(value & (1 << i));
    }
}



uint32_t vPortGpio :: Read(void) {
    uint8_t  i;
    uint32_t value = 0;
    for (i = 0; i < sizePort; ++i) {
        value |= (vPort[i]->ReadState() << i);
    }

    return (value);
}


