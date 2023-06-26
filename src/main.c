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
#define INT_POR_SEG   1000
#define TICKS_POR_SEG 100
#define ESPERA_SEG    3000

#define CONT_TIME     0
#define CONT_ALRM     1
#define MAX_CONT      2
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static board_t       board_educia;
static clock_t       reloj;
static modo_t        modo;
static uint8_t       hora_auxiliar[TIME_SIZE];
static uint16_t      contador_pulsos[3] = {0};

static const uint8_t LIMITE_MINUTOS[]   = {5, 9};
static const uint8_t LIMITE_HORAS[]     = {2, 3};
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void CambiarModo(modo_t estado) {
    modo = estado;
    switch (modo) {
        case SIN_CONFIGURAR:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 1, 1}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 1, 0, 0}, 200);
            break;
        case MOSTRANDO_HORA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 0, 0}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 1, 0, 0}, 200);

            break;
        case AJUSTAR_MINUTOS_ACTUAL:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 1, 1}, 200);
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 0, 0}, 200);
            break;
        case AJUSTAR_MINUTOS_ALARMA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){0, 0, 1, 1}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 0, 1, 1}, 200);
            break;
        case AJUSTAR_HORAS_ALARMA:
            DisplayNewParpadeoDigitos(board_educia->display, (uint8_t[]){1, 1, 0, 0}, 200);
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){1, 1, 0, 0}, 200);
            break;
        default:
            break;
    }
}
int main(void) {
    board_educia = board_Create();
    reloj        = ClockCreate(TICKS_POR_SEG, AlarmaToggle);

    SysTickConfig(INT_POR_SEG);
    CambiarModo(SIN_CONFIGURAR);

    while (true) {

        if (DigitalInput_HasActivate(board_educia->aceptar)) {
            if (modo == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(board_educia->buz)) {
                    ClockPosponerAlarma(reloj, TIME_POST);
                } else if (!ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(board_educia->display, true);
                }
            } else if (modo == AJUSTAR_MINUTOS_ACTUAL) {
                CambiarModo(AJUSTAR_HORAS_ACTUAL);
            } else if (modo == AJUSTAR_HORAS_ACTUAL) {
                ClockSetTime(reloj, hora_auxiliar, sizeof(hora_auxiliar));
                CambiarModo(MOSTRANDO_HORA);
            } else if (modo == AJUSTAR_MINUTOS_ALARMA) {
                CambiarModo(AJUSTAR_HORAS_ALARMA);
            } else if (modo == AJUSTAR_HORAS_ALARMA) {
                ClockSetAlarma(reloj, hora_auxiliar, sizeof(hora_auxiliar));
                CambiarModo(MOSTRANDO_HORA);
                DisplayPuntoAlarma(board_educia->display, true);
            }
        }

        if (DigitalInput_HasActivate(board_educia->rechazar)) {
            if (modo == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(board_educia->buz)) {
                    ClockCancelarAlarma(reloj);
                } else if (ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(board_educia->display, false);
                    ClockCancelarAlarma(reloj);
                }
            } else {
                if (ClockGetTime(reloj, hora_auxiliar, sizeof(hora_auxiliar))) {
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        }

        if (DigitalInput_HasActivate(board_educia->f1)) {
            contador_pulsos[CONT_TIME] = 1;
        }
        if (contador_pulsos[CONT_TIME] > (INT_POR_SEG * 3)) {
            contador_pulsos[CONT_TIME] = 0;
            contador_pulsos[MAX_CONT]  = 1;
            DisplayParpadeoPuntos(board_educia->display, (uint8_t[]){0, 0, 0, 0}, 0);
            CambiarModo(AJUSTAR_MINUTOS_ACTUAL);
            ClockGetTime(reloj, hora_auxiliar, sizeof(hora_auxiliar));
            DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
        }

        if (DigitalInput_HasActivate(board_educia->f2)) {
            contador_pulsos[CONT_ALRM] = 1;
        }
        if (contador_pulsos[CONT_ALRM] > (INT_POR_SEG * 3)) {
            contador_pulsos[CONT_ALRM] = 0;
            contador_pulsos[MAX_CONT]  = 1;
            DisplayPuntoAlarma(board_educia->display, false);
            CambiarModo(AJUSTAR_MINUTOS_ALARMA);
            ClockGetAlarma(reloj, hora_auxiliar, sizeof(hora_auxiliar));
            DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
        }

        if (DigitalInput_HasActivate(board_educia->f3)) {
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_MINUTOS_ALARMA)) {
                DecrementarBCD(&hora_auxiliar[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTAR_HORAS_ACTUAL) || (modo == AJUSTAR_HORAS_ALARMA)) {
                DecrementarBCD(&hora_auxiliar[0], LIMITE_HORAS);
            }
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
            } else if (((modo == AJUSTAR_MINUTOS_ALARMA) || (modo == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
            }
        }

        if (DigitalInput_HasActivate(board_educia->f4)) {
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_MINUTOS_ALARMA)) {
                IncrementarBCD(&hora_auxiliar[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTAR_HORAS_ACTUAL) || (modo == AJUSTAR_HORAS_ALARMA)) {
                IncrementarBCD(&hora_auxiliar[0], LIMITE_HORAS);
            }
            if ((modo == AJUSTAR_MINUTOS_ACTUAL) || (modo == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
            } else if (((modo == AJUSTAR_MINUTOS_ALARMA) || (modo == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(board_educia->display, hora_auxiliar, sizeof(hora_auxiliar));
            }
        }

        if (contador_pulsos[MAX_CONT] > (INT_POR_SEG * 30)) {
            contador_pulsos[MAX_CONT] = 0;
            CambiarModo(MOSTRANDO_HORA);
        }
    }
}

void SysTick_Handler(void) {

    uint8_t hora[TIME_SIZE];

    DisplayRefresh(board_educia->display);
    ClockTick(reloj);
    if ((DigitalInput_GetState(board_educia->f1)) && (contador_pulsos[CONT_TIME] > 0)) {
        contador_pulsos[CONT_TIME]++;
    }
    if ((DigitalInput_GetState(board_educia->f2)) && (contador_pulsos[CONT_ALRM] > 0)) {
        contador_pulsos[CONT_ALRM]++;
    }
    if (!((DigitalInput_GetState(board_educia->f2))) &&
        !((DigitalInput_GetState(board_educia->f1))) &&
        !((DigitalInput_GetState(board_educia->aceptar))) &&
        !((DigitalInput_GetState(board_educia->rechazar))) && (contador_pulsos[MAX_CONT] > 0)) {
        contador_pulsos[MAX_CONT]++;
    }

    if (modo <= MOSTRANDO_HORA) {
        ClockGetTime(reloj, hora, 4);
        DisplayWriteBCD(board_educia->display, hora, 4);
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/