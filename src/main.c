/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "chip.h"
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"

/*==================[macros and definitions]=================================*/
#define num_0 0b00111111
#define num_1 0b00000110
#define num_2 0b01011011
#define num_3 0b01001111
#define num_4 0b01100110
#define num_5 0b01101101
#define num_6 0b01111101
#define num_7 0b00000111
#define num_8 0b01111111
#define num_9 0b01101111
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void DelayBase(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void DelayBase(void) {
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
        if (DigitalInput_HasActivate(edu_cia->f1))
            DigitalOutput_Toggle(edu_cia->buz);
        if (DigitalInput_HasActivate(edu_cia->aceptar))
            DisplayWriteBCD(edu_cia->display, (uint8_t{}){1, 2, 3, 4}, 4);
        if (DigitalInput_HasActivate(edu_cia->rechazar))
            DisplayWriteBCD(edu_cia->display, NULL, 0);
    }
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/