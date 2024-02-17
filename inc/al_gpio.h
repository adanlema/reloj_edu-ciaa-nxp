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

#ifndef AL_GPIO_H
#define AL_GPIO_H

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdbool.h>
typedef struct DigitalInput *  DigitalInput_pt;
typedef struct DigitalOutput * DigitalOutput_pt;
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Digital Input Create
 * Se encarga de configurar como entrada digital a un pin proporcionado.
 * @param port Puerto
 * @param pin Pin
 * @param inverted Si trabajo con Pull-Down(false) o Pull-Up(true)
 * @return DigitalInput_pt
 */
DigitalInput_pt DigitalInput_Create(uint8_t port, uint8_t pin, bool inverted);
/**
 * @brief Digital Input Get State
 * Devuelve el estado de la entrada.
 * @param AL Puntero al struct de la entrada.
 * @return true Si se encuentra activo.
 * @return false Si no esta activo.
 */
bool DigitalInput_GetState(DigitalInput_pt AL);
/**
 * @brief Digital Input Has Change
 * Evalua si la entrada a cambiado.
 * @param AL Puntero al struct de la entrada.
 * @return true Si cambio.
 * @return false Si no cambio.
 */
bool DigitalInput_HasChange(DigitalInput_pt AL);
/**
 * @brief Digital Input Has Activate
 * Evalua si la entrada a sido activada.
 * @param AL Puntero al struct de la entrada.
 * @return true Si se activo.
 * @return false Si no se activo.
 */
bool DigitalInput_HasActivate(DigitalInput_pt AL);
/**
 * @brief Digital Input Has Desactivate
 * Evalua si la entrada a sido desactivada.
 * @param AL Puntero al struct de la entrada.
 * @return true Si se desactivo.
 * @return false Si sigue activa.
 */
bool DigitalInput_HasDesactivate(DigitalInput_pt AL);
/**
 * @brief Digital Output Create
 * Crea una salida digital, en la cual debemos proporcionar el pin y puerto.
 * Retorna el puntero de la esctructura creada.
 * @param port Puerto
 * @param pin Pin
 * @return DigitalOutput_pt
 */
DigitalOutput_pt DigitalOutput_Create(uint8_t port, uint8_t pin);
/**
 * @brief Digital Output Get State
 * Devuelve el estado de la salida.
 * @param AL Puntero al struct de la salida.
 * @return true Si se encuentra activo.
 * @return false Si no esta activo.
 */
bool DigitalOutput_GetState(DigitalOutput_pt AL);
/**
 * @brief Digital Output Activate
 * Activa la salida solicitada.
 * @param AL Puntero a la salida digital.
 */
void DigitalOutput_Activate(DigitalOutput_pt AL);
/**
 * @brief Digital Output Desactivate
 * Desactiva la salida solicitada.
 * @param AL Puntero a la salida digital.
 */
void DigitalOutput_Desactivate(DigitalOutput_pt AL);
/**
 * @brief Digital Output Toggle
 * Cambia el estado de la salida proporcionada.
 * @param AL Puntero a la salida digital.
 */
void DigitalOutput_Toggle(DigitalOutput_pt AL);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif