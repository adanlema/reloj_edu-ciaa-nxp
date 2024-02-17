/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include "al_modo.h"
#include "al_display.h"
#include <stdint.h>
/*==================[macros and definitions]=================================*/
struct mef_s {
    board_t placa;
    modo_t  estado;
};
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static mef_t MefAllocate(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static mef_t MefAllocate(void) {
    static struct mef_s mef_creada = {0};
    mef_t               self       = &mef_creada;
    return self;
}
/*==================[external functions definition]==========================*/
mef_t MefCreate(board_t placa) {
    mef_t self = MefAllocate();
    if (self) {
        self->estado = SIN_CONFIGURAR;
        self->placa  = placa;
    }
    return self;
}

modo_t MefGetState(mef_t mef) {
    return mef->estado;
}

void CambiarMef(mef_t mef, modo_t estado_prox, uint16_t * contador) {
    mef->estado = estado_prox;
    switch (mef->estado) {
        case SIN_CONFIGURAR:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){1, 1, 1, 1}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){0, 1, 0, 0}, 50);
            (*contador) = 0;
            break;
        case MOSTRANDO_HORA:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){0, 0, 0, 0}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){0, 1, 0, 0}, 50);
            (*contador) = 0;
            break;
        case AJUSTAR_MINUTOS_ACTUAL:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){0, 0, 1, 1}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){0, 0, 0, 0}, 0);
            (*contador) = 1;
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){1, 1, 0, 0}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){0, 0, 0, 0}, 0);
            (*contador) = 1;
            break;
        case AJUSTAR_MINUTOS_ALARMA:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){0, 0, 1, 1}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){0, 0, 1, 1}, 50);
            (*contador) = 1;
            break;
        case AJUSTAR_HORAS_ALARMA:
            DisplayNewParpadeoDigitos(mef->placa->display, (uint8_t[]){1, 1, 0, 0}, 50);
            DisplayParpadeoPuntos(mef->placa->display, (uint8_t[]){1, 1, 0, 0}, 50);
            (*contador) = 1;
            break;
        default:
            break;
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/