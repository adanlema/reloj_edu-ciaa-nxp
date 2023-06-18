/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_BSP_H
#define AL_BSP_H

/*==================[inclusions]=============================================*/
#include "al_gpio.h"
#include "al_display.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
//! Estructura de la placa que utilizaremos para el reloj despertador.
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
//! Creacion de un tipo de dato que es un puntero a una estructura de la placa.
typedef struct board_s * board_t;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/**
 * @brief Board Create
 * Se encarga de crear una placa, la cual contendra cuatro entradas digitales y
 * seis salidas digitales.
 * @return board_t Retorna el puntero de la estructura de la placa.
 */
board_t board_Create(void);
/**
 * @brief Configurar el SysTick
 * Se encarga de configurar el SysTick, habilitando sus interrupciones y pasamos el valor  de
 * interrupciones que tendremos por segundo.
 * @param ticks interrupciones por segundo.
 */
void SysTickConfig(uint32_t ticks);

/**
 * @brief Cambiar el estado de la Alarma
 * Se encarga de encender o apagar la alarma.
 * @param estado *true* si se quiere activar, *false* si se quiere apagar.
 */
void AlarmaToggle(bool estado);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif