/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "chip.h"
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void) {

    board_pt board = BoardptCreate();

    while (true) {

        if (DigitalInput_HasActivate(board->f2)) {
            DigitalOutput_Toggle(board->buz);
        }
        if (DigitalInput_HasActivate(board->f3)) {
            DigitalOutput_Toggle(board->buz);
        }
        // if (DigitalInput_HasActivate(board->f4)) {
        //     DigitalOutput_Toggle(board->buz);
        // }

        if (DigitalInput_HasActivate(board->f1)) {
            DisplayWriteBCD(board->display, (uint8_t[]){1, 2, 3, 4}, 4);
        }

        // if (DigitalInput_HasActivate(board->rechazar)) {
        //     DisplayWriteBCD(board->display, NULL, 0);
        // }
        DisplayRefresh(board->display);

        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }
    }
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/