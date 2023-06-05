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

    board_t board = board_Create();

    while (true) {

        if (DigitalInput_HasActivate(board->f3)) {
            DigitalOutput_Activate(board->buz);
        }
        if (DigitalInput_HasActivate(board->f4)) {
            DigitalOutput_Desactivate(board->buz);
        }

        if (DigitalInput_HasActivate(board->f1)) {
            DisplayWriteBCD(board->display, (uint8_t[]){0, 9, 1, 2}, 4);
        }
        if (DigitalInput_HasActivate(board->aceptar)) {
            DisplayWriteBCD(board->display, (uint8_t[]){0, 2, 0, 8}, 4);
        }
        if (DigitalInput_HasActivate(board->f2) | DigitalInput_HasActivate(board->rechazar)) {
            DisplayWriteBCD(board->display, NULL, 0);
        }

        DisplayRefresh(board->display);

        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 500; delay++) {
                __asm("NOP");
            }
        }
    }
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/