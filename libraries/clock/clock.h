#ifndef _CLOCK_ABSTRACT_INTERFACE_H_
#define _CLOCK_ABSTRACT_INTERFACE_H_

#include <stdint.h>

class clock {
public:
    virtual void ShowTime(void)            = 0;
    virtual void SetTime(uint32_t seconds) = 0;
    virtual uint32_t GetTime(void)         = 0;
};

#endif /* _CLOCK_ABSTRACT_INTERFACE_H_ */