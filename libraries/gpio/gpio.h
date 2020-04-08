#ifndef _PINOUT_ABSTRACT_INTERFACE_H_
#define _PINOUT_ABSTRACT_INTERFACE_H_

#include <stdint.h>



class gpio {
public:
enum gpioMode { IN_FLOAT, IN_PULLUP, IN_PULLDOWN,
                OUT_OD_FLOAT, OUT_OD_PULLUP, OUT_PP };
    virtual void WriteState(bool state) = 0;
    virtual bool ReadState(void)        = 0;
};

#endif /* _PINOUT_ABSTRACT_INTERFACE_H_ */