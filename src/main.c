/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "al_gpio.h"
#include "al_bsp.h"
#include "al_display.h"
#include "reloj.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
typedef enum {
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTAR_MINUTOS_ACTUAL,
    AJUSTAR_HORAS_ACTUAL,
    AJUSTAR_MINUTOS_ALARMA,
    AJUSTAR_HORAS_ALARMA,
} modo_t;
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
board_t        board_educia;
clock_t        reloj;
static modo_t  modo;
static uint8_t hora_actual[TIME_SIZE];
/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(10, AlarmaToggle);
    modo         = SIN_CONFIGURAR;

    SysTickConfig(1000);
    DisplayParpadeoDigitos(board_educia->display, 0, 3, 200);

    while (true) {

        if (DigitalInput_HasActivate(board_educia->aceptar)) {
        }
        if (DigitalInput_HasActivate(board_educia->rechazar)) {
        }

        if (DigitalInput_HasActivate(board_educia->f3)) {
        }
        if (DigitalInput_HasActivate(board_educia->f4)) {
        }

        if (DigitalInput_HasActivate(board_educia->f1)) {
        }
        if (DigitalInput_HasActivate(board_educia->f2)) {
        }
    }
}

void SysTick_Handler(void) {
    static bool valor_anterior = false;
    bool        valor_actual;

    DisplayRefresh(board_educia->display);
    valor_actual = ClockTick(reloj);

    if (valor_actual != valor_anterior) {
        valor_anterior = valor_actual;
        if (modo <= MOSTRANDO_HORA) {
            ClockGetTime(reloj, hora_actual, 4);
            DisplayWriteBCD(board_educia->display, hora_actual, 4);
            DisplayTogglePunto(board_educia->display, 1);
        }
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/