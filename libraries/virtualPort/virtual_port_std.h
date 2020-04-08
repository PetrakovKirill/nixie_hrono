#ifndef _VIRTUAL_PORT_IMPLEMENTATION_WITH_GPIO_H_
#define _VIRTUAL_PORT_IMPLEMENTATION_WITH_GPIO_H_

#include "virtual_port.h"
#include "gpio.h"

class vPortGpio : public virtualPort {
public:
    vPortGpio(gpio **arrayGpio, uint8_t size);
    void     Write (uint32_t value);
    uint32_t Read  (void);

private:
    gpio  **vPort;
    uint8_t sizePort;
};


#endif /* _VIRTUAL_PORT_IMPLEMENTATION_WITH_GPIO_H_ */