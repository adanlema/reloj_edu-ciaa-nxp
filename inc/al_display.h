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

typedef struct parpadeo_s {
    uint8_t  parpadeo[4];
    uint16_t contador;
    uint16_t frecuencia;
    bool     bandera;
} * parpadeo_t;
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
/**
 * @brief Funcion para cambiar el estado de un punto
 * Es una funcion para cambiar el estado de un puntero de un segmento especifico.
 * @param display Puntero al display.
 * @param posicion Posicion del punto que se quiere encender.
 */
void DisplayTogglePunto(display_t display, uint8_t posicion);
/**
 * @brief Parpadeo de los Digitos
 * Es una funcion que se encarga del parpadeo de los digitos del display. Se le debe pasar un vector
 * indicando que puntos se quiere encender y la frecuencia que tendra el parpadeo deseado. Se debe
 * colocar un 1 si se desea parpadear ese segmento, y un 0 en el caso contrario.
 * @param display Puntero al display
 * @param number Vector con los digitos a parpadear
 * @param frecuencia Frecuencia de parpadeo
 */
void DisplayNewParpadeoDigitos(display_t display, uint8_t * number, uint16_t frecuencia);
/**
 * @brief Parpadeo de los puntos del display
 * Es una funcion que se encarga del parpadeo de los puntos del display. Se le debe pasar un vector
 * con los digitos que desea encender y la frecuencia que tendra el mismo. El dutty cicle viene
 * establecido, y es de un 50%.
 * @param display Puntero al display.
 * @param number Vector con los puntos a parpadear.
 * @param frecuencia Frecuencia de parpadeo.
 */
void DisplayParpadeoPuntos(display_t display, uint8_t * number, uint16_t frecuencia);
/**
 * @brief Encendido del indicador de la alarma.
 * Es una funcion que utilizamos para indicar visualmente si la alarma esta encendido o no.
 * @param display Puntero al display.
 * @param estado Se envia "true" si la alarma esta encendido, "false" caso contrario.
 */
void DisplayPuntoAlarma(display_t display, bool estado);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif