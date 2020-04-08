#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "nexDisplay.h"



void nexDisplay :: timerEvent :: EventHandler(void) {
    inst->DynamicIndication();
}


void nexDisplay::DynamicIndication(void) {
    /* Реализуем программный ШИМ в четные состояния счетчика cnt
     * высталяем активное состояние пина, в нечетные выполняем сброс
     * Заполнение регулируем изменением периода таймера для каждого 
     * пина упраления Анодом. */
    if (cnt++ % 2) {
        decoderHV->Write(videoMem.digi[cntDigis]);
        anodeDigis->Write((1 << cntDigis) & digisMask);
        indcTimer.SetInterval(digitPwm[cntDigis]);
    } else {
        anodeDigis->Write(0x00);
        indcTimer.SetInterval(PWM_MAX - digitPwm[cntDigis]);
        cntDigis = (cntDigis + 1) % VIDEOMEM_SIZE;
    }

    indcTimer.ResetCounter();
    indcTimer.Start();
}



void nexDisplay::Show(bool state) {
    if (state) {
        cntDigis = 0;
        indcTimer.ResetCounter();
        indcTimer.Start();
    } else {
        indcTimer.Stop();
        anodeDigis->Write(0x00);
    }
}



void nexDisplay::Print(char *str) {
    memcpy(lastPrint, str, VIDEOMEM_SIZE);
    lastPrint[VIDEOMEM_SIZE] = '\0';
    LoadRaw(NixieStringDecoder(lastPrint));

}


const char * nexDisplay :: LastPrint(void) {
    return ((const char *)lastPrint);
}


void nexDisplay::SetBrightDigis(uint8_t digitNum, uint8_t bright) {
    // Serial.println("Brig");
    if ( (digitNum <  VIDEOMEM_SIZE) &&
         (bright   <= PWM_MAX) ) {
        digitPwm[digitNum] = bright;
    }
}



nexDisplay::nexDisplay(timerStrategy *strategy, virtualPort *vpAnods, virtualPort *vpCathods) :
                            timEvent(this), indcTimer(&timEvent, 0) {
    anodeDigis = vpAnods;
    decoderHV  = vpCathods;

    timStrategy = strategy;

    timStrategy->AddTimer(&indcTimer);

    digitPwm[0] = PWM_MAX;
    digitPwm[1] = PWM_MAX;
    digitPwm[2] = PWM_MAX;
    digitPwm[3] = PWM_MAX;

    indcTimer.ResetCounter();
    indcTimer.Start();
    Show(true);
    Print((char *)"0000");
}



void nexDisplay::LoadRaw(nixieRaw_t rawImage) {
   
    uint8_t i, newMask = 0x0F;

    indcTimer.Stop();
    videoMem.word = rawImage.word;
    for(i = 0; i < sizeof(nixieRaw_t); ++i) {
        if ( rawImage.digi[i] == 0xFF) {
            /* Сигнатуру 0xFF маскируем и не отображаем */
            newMask &= ~(1 << i);
        }
    }
    digisMask = newMask;
    indcTimer.Start();
}



nixieRaw_t NixieStringDecoder(char *str) {
    nixieRaw_t raw;
    uint8_t    i;

    if (str) {
        for (i = 0; i < sizeof(nixieRaw_t); ++i) {
            if ( isdigit(str[i]) ) {
                /* Записываем код в raw-структуру */
                raw.digi[i] = str[i] - '0';
            } else {
                /* Все что не цифра помечаем спец сигнатурой */
                raw.digi[i] = 0xFF;
            }
        }
    }

    return (raw);
}