#include <millis_dispatch.h>
#include <rtc_sim.h>
#include <nexDisplay.h>
#include <timer.h>
#include <gpio_avr.h>
#include <virtual_port_std.h>
#include "rtc_clock.h"
#include "adjustment_clock.h"
#include <timer_strategy.h>

#include <button.h>

/* TODO: Вынести во вкладки .ino */
#include <glitch_proxy.h>
#include <blink_proxy.h>
#include <fade_proxy.h>
#include <train_proxy.h>
#include <visual_control.h>
#include <keyboard_controller.h>


millisDispatch_t<3> dispatch;
timerStrategy       timSoftStrategy;
timerStrategy       timHardStrategy;


static clock              *mainClock;
static keyboardController *keyboard;



/* *************************************************************** *
 * ********************** Задачи устройства ********************** *
 * *************************************************************** */

void ShowCurrentTime(void) {
    static uint32_t lastMin = 0;
    static gpioAVR dot(&PORTB, 2, gpio::OUT_PP);
    uint32_t min;
    
    /* Мигаем точкой */
    dot.WriteState(!dot.ReadState());

    min  = (mainClock->GetTime() % 3600) / 60;
    if (min != lastMin) {
        lastMin = min;

        /* Показываем время */
        mainClock->ShowTime();
    }
}



void KeyboardScaner(void) {
    keyboard->Scan();
}



void SoftwareTimers(void) {
    timSoftStrategy.Tick();
}


static void ConstTimerInit() {
    /* Прервыния таймера с периодичностью 7.8кГц */
    OCR2A   = 255;
    TCCR2A |= (1 << WGM21);
    TIMSK2 |= (1 << OCIE2A);
    TCCR2B  = (TCCR2B & B11111000) | 2;

    /* Генератор для повышающего преобразователя */
    gpioAVR(&PORTB, 1, gpio::OUT_PP);
    TCCR1B = (TCCR1B & 0b11111000) | 0x01;
    TCCR1A |= (1<<COM1A1);
    OCR1A = 180;
}



ISR(TIMER2_COMPA_vect) {
    timHardStrategy.Tick();
}



/* ***************************************************************************** *
 * ********************** Arduino-точка входа в программу ********************** *
 * ***************************************************************************** */

void setup(void) {
    Serial.begin(9600);
    ConstTimerInit();

    /* Пины управления анодами */
    static gpio *anodes[4] = {
        static_cast<gpio *>(new gpioAVR(&PORTD, 3, gpio::OUT_PP)), 
        static_cast<gpio *>(new gpioAVR(&PORTD, 4, gpio::OUT_PP)),
        static_cast<gpio *>(new gpioAVR(&PORTD, 5, gpio::OUT_PP)), 
        static_cast<gpio *>(new gpioAVR(&PORTD, 6, gpio::OUT_PP))
    };

    /* Пины управления катодами */
    static gpio *catodes[4] = {
        static_cast<gpio *>(new gpioAVR(&PORTC, 0, gpio::OUT_PP)), 
        static_cast<gpio *>(new gpioAVR(&PORTC, 1, gpio::OUT_PP)),
        static_cast<gpio *>(new gpioAVR(&PORTC, 2, gpio::OUT_PP)), 
        static_cast<gpio *>(new gpioAVR(&PORTC, 3, gpio::OUT_PP))
    };

    /* Виртуальные порты управления анодами и катодами */
    virtualPort *anodePort   = static_cast<virtualPort *>(new vPortGpio(anodes,  4));
    virtualPort *catodesPort = static_cast<virtualPort *>(new vPortGpio(catodes, 4));


    /* Дисплей на ГРИ */
    static nexDisplay displayInstance(&timHardStrategy, anodePort, catodesPort);

    /* Прокси объекты реализующие спецэффекты */
    static glitchProxy glitchInstance(&displayInstance, &timSoftStrategy, 3000);
    static blinkProxy  blinkInstance(&displayInstance,  &timSoftStrategy, 300);

    /* Прокси объекты реализующие эффекты перелистывавнии */
    static fadeProxy   fadeInstance(&displayInstance,   &timSoftStrategy, 15);
    static trainProxy  trainInstance(&displayInstance,  &timSoftStrategy, 100);
    
    /* список прокси-объектов реализующие эффекты при смене цыфр */
    static display *flipList[] = {
        &displayInstance,
        &fadeInstance,
        &trainInstance
    };

    /* прокси-объект управляющий выбором спецэффекта  */
    static displayStrategy flipControl(flipList, sizeof(flipList) / sizeof(*flipList), &glitchInstance, &blinkInstance);

    /* Объект отслеживания реального времени */
    static rtcSim rtcInstance(&timSoftStrategy);

    /* Объект часов связанный с объектом реального времени 
     * используется при работе в режиме отображения текуего времени.
     * Для отображения необходим обект дисплея. Передаем проскси объект
     * для использования эффектов при перелистывании. */
    static rtcClock hrono(static_cast<display *>(&flipControl), 
                          static_cast<rtc     *>(&rtcInstance));

    /* Объект-для отображения процесса установки времени */
    static adjustmentClock adjHrono(static_cast<display *>(&displayInstance));

    /* Объекты клавиатуры */
    static gpioAVR btn1(&PORTD, 7, gpio::IN_PULLUP);
    static gpioAVR btn2(&PORTB, 0, gpio::IN_PULLUP);
    static gpioAVR btn3(&PORTB, 4, gpio::IN_PULLUP);

    static keyboardController keyboardInstance(&btn1, &btn2, &btn3, &hrono, &flipControl);
  
    /* Инициализация абстрактных интерфейсов */
    mainClock = &hrono;
    keyboard  = &keyboardInstance;

    // /* Регистрация задач в диспетчере */
    dispatch.AddTask(ShowCurrentTime, 500);
    dispatch.AddTask(KeyboardScaner,  50);
    dispatch.AddTask(SoftwareTimers,  1);
}



void loop(void) {
    dispatch.Dispatch();
}