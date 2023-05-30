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
struct display_s {
    DigitalOutput_pt segment[7];
    DigitalOutput_pt seg_punto;
    DigitalOutput_pt digito_1;
    DigitalOutput_pt digito_2;
    DigitalOutput_pt digito_3;
    DigitalOutput_pt digito_4;
    funcion_pt       funcion;
};
/*==================[external functions declaration]=========================*/
display_pt DisplayCreate(void);
void       DisplayWrite(display_pt display_al, DigitalOutput_pt digito, const uint8_t numero);
void       DisplayClear(display_pt display_al, DigitalOutput_pt digito);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif