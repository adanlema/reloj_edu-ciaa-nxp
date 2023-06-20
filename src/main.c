/* Copyright 2022, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include "al_gpio.h"
#include "al_bcd.h"
#include "al_bsp.h"
#include "al_display.h"
#include "reloj.h"
#include "al_modo.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static board_t       board_educia;
static clock_t       reloj;
static modo_t        modo;
static uint8_t       hora_actual[TIME_SIZE];

static const uint8_t LIMITE_MINUTOS[] = {5, 9};
static const uint8_t LIMITE_HORAS[]   = {2, 3};
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void CambiarModo(modo_t estado) {
    modo = estado;
    switch (modo) {
        case SIN_CONFIGURAR:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 1, 1}, 200);
            break;
        case MOSTRANDO_HORA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 0, 0}, 0);
            break;
        case AJUSTAR_MINUTOS_ACTUAL:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 1, 1}, 200);
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 0, 0}, 200);
            break;
        case AJUSTAR_MINUTOS_ALARMA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 1, 1}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){1, 1, 1, 1});
            break;
        case AJUSTAR_HORAS_ALARMA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 0, 0}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){1, 1, 1, 1});
            break;
        default:
            break;
    }
}
int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(10, AlarmaToggle);

    SysTickConfig(1000);
    CambiarModo(SIN_CONFIGURAR);

    while (true) {

        if (DigitalInput_HasActivate(board_educia->aceptar)) {
            if (modo == MOSTRANDO_HORA) {
                if (!ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                }
            } else if (modo == AJUSTAR_MINUTOS_ACTUAL) {
                CambiarModo(AJUSTAR_HORAS_ACTUAL);
            } else if (modo == AJUSTAR_HORAS_ACTUAL) {
                ClockSetTime(reloj, hora_actual, sizeof(hora_actual));
                CambiarModo(MOSTRANDO_HORA);
            } else if (modo == AJUSTAR_MINUTOS_ALARMA) {
                CambiarModo(AJUSTAR_HORAS_ALARMA);
            } else if (modo == AJUSTAR_HORAS_ALARMA) {
                ClockSetAlarma(reloj, hora_actual, sizeof(hora_actual));
                CambiarModo(MOSTRANDO_HORA);
            }
        }

        if (DigitalInput_HasActivate(board_educia->rechazar)) {
            if (modo == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(board_educia->buz)) {
                    ClockCancelarAlarma(reloj);
                } else if (ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                }
            } else {
                if (ClockGetTime(reloj, hora_actual, sizeof(hora_actual))) {
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        }

        if (DigitalInput_HasActivate(board_educia->f1)) {
            CambiarModo(AJUSTAR_MINUTOS_ACTUAL);
            ClockGetTime(reloj, hora_actual, sizeof(hora_actual));
            DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
        }

        if (DigitalInput_HasActivate(board_educia->f2)) {
            CambiarModo(AJUSTAR_MINUTOS_ALARMA);
            ClockGetAlarma(reloj, hora_actual, sizeof(hora_actual));
            DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
        }

        if (DigitalInput_HasActivate(board_educia->f3)) {
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_MINUTOS_ALARMA)) {
                DecrementarBCD(&hora_actual[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTAR_HORAS_ACTUAL) || (modo == AJUSTAR_HORAS_ALARMA)) {
                DecrementarBCD(&hora_actual[0], LIMITE_HORAS);
            }
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
            } else if (((modo == AJUSTAR_MINUTOS_ALARMA) || (modo == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
                // Debo agregar una funcion que haga parpadear los puntos del display.
            }
        }

        if (DigitalInput_HasActivate(board_educia->f4)) {
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_MINUTOS_ALARMA)) {
                IncrementarBCD(&hora_actual[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTAR_HORAS_ACTUAL) || (modo == AJUSTAR_HORAS_ALARMA)) {
                IncrementarBCD(&hora_actual[0], LIMITE_HORAS);
            }
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
            } else if (((modo == AJUSTAR_MINUTOS_ALARMA) || (modo == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(board_educia->display, hora_actual, sizeof(hora_actual));
                // Debo agregar una funcion que haga parpadear los puntos del display.
            }
        }
    }
}

void SysTick_Handler(void) {
    static uint16_t contador = 0;
    uint8_t         hora[TIME_SIZE];

    DisplayRefresh(board_educia->display);
    ClockTick(reloj);
    contador = (contador + 1) % 1000;

    if (modo <= MOSTRANDO_HORA) {
        ClockGetTime(reloj, hora, 4);
        DisplayWriteBCD(board_educia->display, hora, 4);
        if (contador > 500) {
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 1, 0, 0});
        }
        if (ClockGetAlarmaHabilitada(reloj)) {
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 0, 0, 1});
        }
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/