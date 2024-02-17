/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_BSP_H
#define AL_BSP_H

/*==================[inclusions]=============================================*/
#include "al_gpio.h"
#include "al_display.h"
/*==================[macros]=================================================*/
#define CANTIDAD_DIGITOS 4
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
    DigitalOutput_pt led_1;
    DigitalOutput_pt led_2;
    DigitalOutput_pt led_3;
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
/**
 * @brief Funcion para indicar un error al crear una tarea.
 *
 * @param board Puntero a la placa.
 * @param codigo Codigo para indicar el error.
 */
void StopByError(board_t board, uint8_t codigo);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif