#ifndef _EVENT_ABSTRACT_INTERFACE_H_
#define _EVENT_ABSTRACT_INTERFACE_H_

class event {
public:
    virtual void EventHandler(void) = 0;
};

#endif /* _EVENT_ABSTRACT_INTERFACE_H_ */