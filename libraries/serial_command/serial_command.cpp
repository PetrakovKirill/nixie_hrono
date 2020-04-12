#include "serial_command.h"
#include <string.h>


serialCommand :: serialCommand(Stream *serialStream, uint8_t *rcvBuffer, size_t sizeBuffer,  uint8_t quantityEvent) :
    serial(serialStream), pBuffer(rcvBuffer), sizeBuffer(sizeBuffer), quantity(quantityEvent)  {

    events   = new event*[quantity];
    eventKey = new const char*[quantity];

    memset(events,   0x00, sizeof(event *) * quantity);
    memset(eventKey, 0x00, sizeof(const char *) * quantity);

    Serial.println(__FUNCTION__);
}



void serialCommand :: AddEvent(const char *strKey, event *eventHandler) {
    uint8_t i;
    for (i = 0; i < quantity; ++i) {
        if (events[i] == nullptr) {
            events[i]   = eventHandler;
            eventKey[i] = strKey;
            break;
        }
    }
}



void serialCommand :: Scan(void) {
    char *strPtr;
    size_t len = serial->available();
    uint8_t i;

    if (!len) {
        return;
    }
    
    len = (len < sizeBuffer) ? (len) : (sizeBuffer);

    serial->readBytes(pBuffer, len);

    /* Защиаем границы буфера строки */
    pBuffer[len] = '\0';

    for(i = 0; i < quantity; ++i) {
        strPtr = strstr((char *)pBuffer, eventKey[i]);
        if (strPtr && events[i]) {
            events[i]->EventHandler();
        } 
    }
}