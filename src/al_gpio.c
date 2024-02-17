/* Copyright 2022, Adan Lema <adanlema@hotmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief HAL_GPIO
 ** \brief Implementacion de las funciones de una HAL_GPIO
 ** @{ */
/*==================[inclusions]=============================================*/
#include "chip.h"
#include <stdbool.h>
#include "al_gpio.h"
/*==================[macros and definitions]=================================*/
#define CANTIDAD 20

struct DigitalInput {
    uint8_t port;
    uint8_t pin;
    bool    inverted;
    bool    last_state;
    bool    ocupado;
};
struct DigitalOutput {
    uint8_t port;
    uint8_t pin;
    bool    ocupado;
};

/*==================[internal data declaration]==============================*/
static struct DigitalInput  Input[CANTIDAD]  = {0};
static struct DigitalOutput Output[CANTIDAD] = {0};
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Funciones para las Entradas */
DigitalInput_pt DigitalInput_Create(uint8_t port, uint8_t pin, bool inverted) {
    uint8_t posicion = 0;
    for (int i = 0; (i < CANTIDAD) & (posicion == 0); i++) {
        if (Input[i].ocupado == false)
            posicion = i;
    }
    DigitalInput_pt AL = &Input[posicion];
    AL->port           = port;
    AL->pin            = pin;
    AL->inverted       = inverted;
    AL->ocupado        = true;
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, port, pin, false);
    return AL;
}
bool DigitalInput_GetState(DigitalInput_pt AL) {
    if (AL->inverted == 0)
        return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, AL->port, AL->pin);
    else
        return !(Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, AL->port, AL->pin));
}
bool DigitalInput_HasChange(DigitalInput_pt AL) {
    bool current_state = DigitalInput_GetState(AL);
    bool resultado;
    if (current_state != AL->last_state)
        resultado = true;
    else
        resultado = false;
    AL->last_state = current_state;
    return resultado;
}
bool DigitalInput_HasActivate(DigitalInput_pt AL) {
    bool current_state = DigitalInput_GetState(AL);
    bool resultado;
    if ((0 == AL->last_state) & (1 == current_state))
        resultado = true;
    else
        resultado = false;
    AL->last_state = current_state;
    return resultado;
}
bool DigitalInput_HasDesactivate(DigitalInput_pt AL) {
    bool current_state = DigitalInput_GetState(AL);
    bool resultado;
    if ((1 == AL->last_state) & (0 == current_state))
        resultado = true;
    else
        resultado = false;
    AL->last_state = current_state;
    return resultado;
}

/* Funciones para las Salidas */
DigitalOutput_pt DigitalOutput_Create(uint8_t port, uint8_t pin) {
    uint8_t posicion = 0;
    for (int i = 0; (i < CANTIDAD) & (posicion == 0); i++) {
        if (Output[i].ocupado == false)
            posicion = i;
    }
    DigitalOutput_pt AL = &Output[posicion];
    AL->port            = port;
    AL->pin             = pin;
    AL->ocupado         = true;
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, port, pin, true);
    return AL;
}

bool DigitalOutput_GetState(DigitalOutput_pt AL) {
    return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, AL->port, AL->pin);
}

void DigitalOutput_Activate(DigitalOutput_pt AL) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, AL->port, AL->pin, true);
}
void DigitalOutput_Desactivate(DigitalOutput_pt AL) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, AL->port, AL->pin, false);
}
void DigitalOutput_Toggle(DigitalOutput_pt AL) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, AL->port, AL->pin);
}

/**  doxygen end group definition */
/*==================[end of file]============================================*/