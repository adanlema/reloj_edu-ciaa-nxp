/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include "al_bcd.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    if ((numero[0] >= limite[0]) && (numero[1] >= limite[1])) {
        numero[1] = 0;
        numero[0] = 0;
    } else if (numero[1] == 9) {
        numero[1] = 0;
        numero[0]++;
    } else {
        numero[1]++;
    }
}
void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    if ((numero[0] == 0) && (numero[1] == 0)) {
        numero[0] = limite[0];
        numero[1] = limite[1];
        ;
    } else if (numero[1] == 0) {
        numero[1] = 9;
        numero[0]--;
    } else {
        numero[1]--;
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/