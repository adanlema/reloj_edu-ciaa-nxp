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
typedef void (*display_apagar_t)(void);
//! Funcion de callback para prender los segmentos de la pantalla
typedef void (*display_encender_segmento_t)(uint8_t segmentos);
//! Funcion de callback para prender un digito en la pantalla
typedef void (*display_encender_digito_t)(uint8_t digitos);
//! Estructura para manejar el display
/**
 * @brief
 * @param DisplayApagar Funcion para apagar el display.
 * @param DisplayEncenderSegmento Funcion para encender los segmentos.
 * @param DisplayEncenderDigito Funcion para encender un digito.
 */
typedef struct display_driver_s {
    display_apagar_t            DisplayApagar;
    display_encender_segmento_t DisplayEncenderSegmento;
    display_encender_digito_t   DisplayEncenderDigito;
} const * const display_driver_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Funcion para crear un display.
 * Se encarga de devolver un puntero a la estructura de un display.
 * @param digitos Cantidad de digitos del display.
 * @param driver_dato Driver que utilizara el display para encender,apagar los digitos/segmentos
 * @return display_t Puntero a la estructura del display creada.
 */
display_t DisplayCreate(uint8_t digitos, display_driver_t driver_dato);
/**
 * @brief Funcion para escribir en el display
 * Es una funcion que escribe en el display utilizando BCD.
 * @param display Puntero al display.
 * @param number Cadena que desea escribir.
 * @param size Tamaño de digitos del display.
 */
void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size);
/**
 * @brief Funcion para refrescar el display
 * Es una funcion para colocar lo que guarda en memoria el display y ponerlo en la pantalla.
 * @param display Puntero al display.
 */
void DisplayRefresh(display_t display);

void DisplayParpadeoDigitos(display_t display, uint8_t desde, uint8_t hasta, uint16_t frecuencia);
void DisplayTogglePunto(display_t display, uint8_t posicion);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif