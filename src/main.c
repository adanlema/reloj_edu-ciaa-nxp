/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "chip.h"
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"
#include "reloj.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void SysTickConfig(uint32_t ticks);
void        ActivarAlarma(board_t self);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
board_t board_educia;
clock_t reloj;
/*==================[internal functions definition]==========================*/
static void SysTickConfig(uint32_t ticks) {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / ticks);

    __disable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();
}
void ActivarAlarma(board_t self) {
    DigitalOutput_Activate(self->buz);
}
/*==================[external functions definition]==========================*/

int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(10, ActivarAlarma);
    uint8_t hora_actual[TIME_SIZE];

    SysTickConfig(1000);
    ClockSetTime(reloj, (uint8_t[]){0, 8, 0, 0, 0, 0}, 6);
    while (true) {

        ClockGetTime(reloj, hora_actual, 4);
        DisplayWriteBCD(board_educia->display, hora_actual, 4);

        if (DigitalInput_HasActivate(board_educia->f3)) {
            DigitalOutput_Activate(board_educia->buz);
        }
        if (DigitalInput_HasActivate(board_educia->f4)) {
            DigitalOutput_Desactivate(board_educia->buz);
        }
    }
}

void SysTick_Handler(void) {
    ClockTick(reloj);
    DisplayRefresh(board_educia->display);
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/