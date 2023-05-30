/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "chip.h"
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display7seg.h"

/*==================[macros and definitions]=================================*/
#define num_0 0b00111111
// const uint8_t segment_display[] = {
//     0b00111111, // 0
//     0b00000110, // 1
//     0b01011011, // 2
//     0b01001111, // 3
//     0b01100110, // 4
//     0b01101101, // 5
//     0b01111101, // 6
//     0b00000111, // 7
//     0b01111111, // 8
//     0b01101111  // 9
// };
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void DelayBase(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void DelayBase(void) {
    for (int index = 0; index < 100; index++) {
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
    }
}
/*==================[external functions definition]==========================*/

int main(void) {

    board_pt edu_cia = BoardptCreate();

    while (true) {
        // if (DigitalInput_HasActivate(edu_cia->f1))
        //     DigitalOutput_Toggle(edu_cia->buz);

        DigitalOutput_Activate(edu_cia->display->digito_1);
        DisplayClear(edu_cia->display, edu_cia->display->digito_1);
        DisplayWrite(edu_cia->display, edu_cia->display->digito_1, num_0);
        DelayBase();
        DigitalOutput_Desactivate(edu_cia->display->digito_1);
    }
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/