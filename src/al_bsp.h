/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_BSP_H
#define AL_BSP_H

/*==================[inclusions]=============================================*/
#include "al_gpio.h"
#include "al_display.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
struct board_s {
    DigitalInput_pt  f1;
    DigitalInput_pt  f2;
    DigitalInput_pt  f3;
    DigitalInput_pt  f4;
    DigitalInput_pt  aceptar;
    DigitalInput_pt  rechazar;
    DigitalOutput_pt buz;
    display_t        display;
};
// } const * const board_pt;
typedef struct board_s * board_pt;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/**
 * @brief Board Create
 * Se encarga de crear una placa, la cual contendra cuatro entradas digitales y
 * seis salidas digitales.
 * @return board_pt Retorna el puntero de la estructura de la placa.
 */
board_pt BoardptCreate(void);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif