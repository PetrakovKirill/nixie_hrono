#ifndef _SERIAL_COMMAND_H_
#define _SERIAL_COMMAND_H_

#include <stdint.h>
#include <Arduino.h>
#include <event.h>

class serialCommand {
public:
    serialCommand(Stream *serialStream, uint8_t *rcvBuffer, size_t sizeBuffer,  uint8_t quantityEvent);
    void AddEvent(const char *strKey, event *eventHandler);
    void Scan(void);
    
private:
    Stream       *serial;
    uint8_t      *pBuffer;
    size_t        sizeBuffer;
    const uint8_t quantity;
    
    const char **eventKey;
    event       **events;
};

#endif /* _SERIAL_COMMAND_H_ */