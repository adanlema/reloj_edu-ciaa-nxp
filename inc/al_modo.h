/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef AL_MODO_H
#define AL_MODO_H

/*==================[inclusions]=============================================*/
#include "al_bsp.h"
#include "reloj.h"
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef enum {
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTAR_MINUTOS_ACTUAL,
    AJUSTAR_HORAS_ACTUAL,
    AJUSTAR_MINUTOS_ALARMA,
    AJUSTAR_HORAS_ALARMA,
} modo_t;

typedef struct mef_s * mef_t;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Crea un puntero a una maquina de estado finito (MEF)
 * La funcion se encarga de retornar un puntero a una estructura que contiene la maquina de estado
 * finito.
 * @param placa Puntero a la placa.
 * @return mef_t Retorna el puntero a la MEF.
 */
mef_t MefCreate(board_t placa);
/**
 * @brief Estado de la MEF
 * @param mef Puntero a la MEF
 * @return modo_t Devuelve el estado de la MEF.
 */
modo_t MefGetState(mef_t mef);
/**
 * @brief Funcion para cambiar el Estado de la MEF
 * Usamos esta funcion para cambiar el estado de la MEF, y ademas modificamos el valor del contador
 * COUNT_MAX para poder controlar que cuando estemos configurando, ya sea la alarma o la hora, no
 * supere el tiempo maximo admitido.
 * @param mef Puntero a la MEF.
 * @param estado_prox Proximo estado de la MEF.
 * @param contador Puntero al contador del tiempo maximo de espera.
 */
void CambiarMef(mef_t mef, modo_t estado_prox, uint16_t * contador);
// void CambiarModo(moto_t estado);
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif