/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_DISPLAY_H
#define AL_DISPLAY_H

/*==================[inclusions]=============================================*/
#include <stdint.h>

#include "al_gpio.h"
/*==================[macros]=================================================*/
#define SEGMENTO_A (1 << 0)
#define SEGMENTO_B (1 << 1)
#define SEGMENTO_C (1 << 2)
#define SEGMENTO_D (1 << 3)
#define SEGMENTO_E (1 << 4)
#define SEGMENTO_F (1 << 5)
#define SEGMENTO_G (1 << 6)
#define SEGMENTO_P (1 << 7)

/*==================[typedef]================================================*/
//! Puntero a una estructura del tipo *display_s*
typedef struct display_s * display_t;
//! Funcion de callback para apagar la pantalla
typedef void (*display_segments_off_t)(void);
//! Funcion de callback para prender los segmentos de la pantalla
typedef void (*display_segments_on_t)(uint8_t segmentos);
//! Funcion de callback para prender un digito en la pantalla
typedef void (*display_digit_on_t)(uint8_t digitos);
//! Estructura para manejar el display
typedef struct display_driver_s {
    display_segments_off_t ScreenTurnOff; //!
    display_segments_on_t  ScreenTurnOn;  //!
    display_digit_on_t     DigitTurnOn;   //!
} const * const display_driver_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
display_t DisplayCreate(uint8_t digitos, display_driver_t driver);
void      DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size);
void      DisplayRefresh(display_t display);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif