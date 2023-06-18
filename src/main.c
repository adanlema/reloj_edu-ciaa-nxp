/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"
#include "reloj.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
board_t board_educia;
clock_t reloj;
// static uint8_t hora_actual[TIME_SIZE];
/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(10, AlarmaToggle);

    SysTickConfig(1000);

    DisplayWriteBCD(board_educia->display, (uint8_t[]){9, 9, 0, 2}, 4);
    // ClockSetTime(reloj, (uint8_t[]){0, 8, 3, 0, 0, 0}, 6);
    // ClockSetAlarma(reloj, (uint8_t[]){0, 9, 0, 0, 0, 0}, 4);

    DisplayTogglePunto(board_educia->display, 1);
    while (true) {

        if (DigitalInput_HasActivate(board_educia->aceptar)) {
            DisplayTogglePunto(board_educia->display, 1);
            //        ClockPosponerAlarma(reloj, TIME_POST);
        }
        if (DigitalInput_HasActivate(board_educia->rechazar)) {
            //      ClockCancelarAlarma(reloj);
        }

        if (DigitalInput_HasActivate(board_educia->f3)) {
            //    DigitalOutput_Activate(board_educia->buz);
        }
        if (DigitalInput_HasActivate(board_educia->f4)) {
            //  DigitalOutput_Desactivate(board_educia->buz);
        }

        if (DigitalInput_HasActivate(board_educia->f1)) {
            //    DisplayParpadeoDigitos(board_educia->display, 1, 2, 10);
        }
        if (DigitalInput_HasActivate(board_educia->f2)) {
            //    DisplayParpadeoDigitos(board_educia->display, 1, 4, 0);
        }
    }
}

void SysTick_Handler(void) {
    ClockTick(reloj);
    // ClockGetTime(reloj, hora_actual, 4);
    // DisplayWriteBCD(board_educia->display, hora_actual, 4);
    DisplayRefresh(board_educia->display);
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/