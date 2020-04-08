#ifndef _VIRTUAL_PORT_ABSTRACT_INTERFACE_H_
#define _VIRTUAL_PORT_ABSTRACT_INTERFACE_H_

#include <stdint.h>

class virtualPort {
public:
    virtual void     Write (uint32_t value) = 0;
    virtual uint32_t Read  (void)           = 0;
};

#endif /* _VIRTUAL_PORT_ABSTRACT_INTERFACE_H_ */