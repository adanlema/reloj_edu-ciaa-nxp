// Copyright 2022, Adan Lema <adanlema@hotmail.com>

/*==================[inclusions]=============================================*/
#include "chip.h"
#include "al_config_pin.h"
#include <stdbool.h>
/*==================[macros and definitions]=================================*/

#define CANTIDAD 20

struct DigitalPin {
    uint8_t  port;
    uint8_t  pin;
    uint32_t modo;
    bool     ocupado;
};

/*==================[internal data declaration]==============================*/
static struct DigitalPin Pin[CANTIDAD] = {0};
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
DigitalPin_pt DigitalPin_Create(uint8_t port, uint8_t pin, uint32_t modo) {
    uint8_t posicion = 0;
    for (int i = 0; (i < CANTIDAD) & (posicion == 0); i++) {
        if (Pin[i].ocupado == false)
            posicion = i;
    }
    DigitalPin_pt AL = &Pin[posicion];
    AL->port         = port;
    AL->pin          = pin;
    AL->modo         = modo;
    AL->ocupado      = true;
    return AL;
}

void DigitalPin_GPIO(DigitalPin_pt AL) {
    Chip_SCU_PinMuxSet(AL->port, AL->pin, AL->modo);
}

/**  doxygen end group definition */
/*==================[end of file]============================================*/