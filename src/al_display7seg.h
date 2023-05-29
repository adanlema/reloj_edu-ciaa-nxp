/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_DISPLAY7_SEG_H
#define AL_DISPLAY7_SEG_H

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "al_gpio.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef struct display_s * display_pt;
typedef void (*funcion_pt)(display_pt);

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
display_pt DisplayCreate(void);
void       DisplayWrite(display_pt AL, DigitalOutput_pt digito, char * cadena,
                        uint8_t size);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif