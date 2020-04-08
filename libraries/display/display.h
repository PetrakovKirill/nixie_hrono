#ifndef _DISPLAY_ABSTRACT_INTERFACE_H_
#define _DISPLAY_ABSTRACT_INTERFACE_H_

#include <stdint.h>

class display {
public:
    virtual void Show(bool state) = 0;
    virtual void Print(char *str) = 0;
};

#endif /* _DISPLAY_ABSTRACT_INTERFACE_H_ */