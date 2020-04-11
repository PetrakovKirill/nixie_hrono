#ifndef _SERIAL_CONTROLLER_H_
#define _SERIAL_CONTROLLER_H_


class serialController {
public:
    serialController(Stream *stream, clock *mainClock, visualController *visual);
    void Polling(void);
private:
    class cmdEventShowtime : public event {
    public:
        cmdEventShowtime(serialController *pInst) : inst(pInst) {}
        void EventHandler(void) { inst->Showtime(); }
    private:
        serialController *inst;
    };

    class cmdEventSetTime : public event {
    public:
        cmdEventSetTime(serialController *pInst) : inst(pInst) {}
        void EventHandler(void) { inst->SetTime(); }
    private:
        serialController *inst;
    };

    class cmdEventSwitchEffect : public event {
    public:
        cmdEventSwitchEffect(serialController *pInst) : inst(pInst) {}
        void EventHandler(void) { inst->SwitchEffect(); }
    private:
        serialController *inst;
    };

    class cmdEventPrint : public event {
    public:
        cmdEventPrint(serialController *pInst) : inst(pInst) {}
        void EventHandler(void) { inst->Print(); }
    private:
        serialController *inst;
    };

    friend cmdEventShowtime;
    friend cmdEventSetTime;
    friend cmdEventSwitchEffect;
    friend cmdEventPrint;

    void Showtime(void);
    void SetTime(void);
    void SwitchEffect(void);
    void Print(void);

    cmdEventShowtime     showTime;
    cmdEventSetTime      setTime;
    cmdEventSwitchEffect switchEffect;
    cmdEventPrint        print;

    static const char *strShowTime;
    static const char *strSetTime ;
    static const char *strSwitchEffect;
    static const char *strPrint;

    serialCommand        serialCmd;
    clock               *mainClock;
    visualController    *visualControl;

    uint8_t rcvBuffer[32];
};


const char *serialController :: strShowTime     = "/showtime";
const char *serialController :: strSetTime      = "/settime" ;
const char *serialController :: strSwitchEffect = "/switcheffect";
const char *serialController :: strPrint        = "/print ";



void serialController :: Showtime(void) {
    visualControl->PrintBlock(false);
    mainClock->ShowTime();
}



void serialController :: SetTime(void) {
    uint32_t  newTime = 0;
    size_t    lentok, len;
    char     *curPtr;
    uint32_t  hour, min;

    curPtr = strstr((char *)rcvBuffer, strSetTime);
    lentok = strlen(strSetTime);
    len = strlen(curPtr);
    if (len == lentok) {
        /* строка содержет только токен */
        return;
    }

    curPtr += lentok;

    hour = atoi(curPtr);
    curPtr = strchr(curPtr, ':');
    if (curPtr == nullptr) {
        /* Не верный формат */
        return;
    }

    /* Сдвиг указателя на симол цифры */
    curPtr++;

    min = atoi(curPtr);
    if ((hour > 23) || (min > 59)){
        /* Не верный формат */
        return;
    }

    newTime = (hour * 3600) + (min * 60);
   
    mainClock->SetTime(newTime);
    mainClock->ShowTime();
}



void serialController :: SwitchEffect(void) {
    visualControl->SwitchNext();
}



void serialController :: Print(void) {
    size_t    lentok, len;
    char     *curPtr;

    curPtr = strstr((char *)rcvBuffer, strPrint);
    lentok = strlen(strSetTime);
    len = strlen(curPtr);
    if (len == lentok) {
        /* строка содержет только токен */
        return;
    }
    curPtr += lentok;

    visualControl->PrintBlock(true);
    visualControl->Print(curPtr);
}



serialController :: serialController(Stream *stream, clock *mainClock, visualController *visual) :
    showTime(this), setTime(this), switchEffect(this), print(this),
    serialCmd(stream, rcvBuffer, sizeof(rcvBuffer), 4) {
    
    this->mainClock     = mainClock;
    this->visualControl = visual;

    memset(rcvBuffer, 0x00, sizeof(rcvBuffer));

    serialCmd.AddEvent(strShowTime,     &showTime);
    serialCmd.AddEvent(strSetTime,      &setTime);
    serialCmd.AddEvent(strSwitchEffect, &switchEffect);
    serialCmd.AddEvent(strPrint,        &print);
}



void serialController :: Polling(void) {
    serialCmd.Scan();
}

#endif /* _SERIAL_CONTROLLER_H_ */