/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_BCD_H
#define AL_BCD_H

/*==================[inclusions]=============================================*/
#include <stdint.h>
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Funcion para incrementar un numero BCD.
 * Esta funcion nos sirve para aumentar un numero BCD en una unidad, considetando los limites
 * maximos. Se trabaja de a dos numeros considerando que configuramos ya sea la hora o los minutos.
 * @param numero Numeros a modificar.
 * @param limite Limites maximos admitidos.
 */
void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

/** @brief Funcion para decrementar un numero BCD.
 * Esta funcion nos sirve para disminuir un numero BCD en una unidad, considetando los limites
 * maximos. Se trabaja de a dos numeros considerando que configuramos ya sea la hora o los minutos.
 * @param numero Numeros a modificar.
 * @param limite Limites maximos admitidos.
 */
void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif