#ifndef RELOJ_H
#define RELOJ_H

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <stdint.h>
#include "al_bsp.h"
/*==================[macros]=================================================*/
#define TIME_SIZE   6
#define TIME_POST   5
#define DECENA_TIME 6
#define UNIDAD_TIME 10

#define UNIDAD_SEG  5
#define DECENA_SEG  4
#define UNIDAD_MIN  3
#define DECENA_MIN  2
#define UNIDAD_HOR  1
#define DECENA_HOR  0

#define CONTROLAR_REBALSE_MIN(DECENA, UNIDAD, SIGUIENTE)                                           \
    if (UNIDAD > 9) {                                                                              \
        DECENA = DECENA + UNIDAD / 9;                                                              \
        UNIDAD = 0 + ((UNIDAD / 9) * UNIDAD - 10);                                                 \
    }                                                                                              \
    if (DECENA > 5) {                                                                              \
        DECENA = DECENA - 6;                                                                       \
        SIGUIENTE += 1;                                                                            \
    }

#define CONTROLAR_REBALSE_HOR(DECENA, UNIDAD)                                                      \
    if (UNIDAD > 9) {                                                                              \
        UNIDAD = 0;                                                                                \
        DECENA += 1;                                                                               \
    }                                                                                              \
    if ((UNIDAD >= 4) & (DECENA >= 2)) {                                                           \
        UNIDAD = 0;                                                                                \
        DECENA = 0;                                                                                \
    }

/*==================[typedef]================================================*/
typedef struct clock_s *  clock_t;
typedef struct alarma_s * alarma_pt;
typedef void (*evento_t)(bool);
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Funcion para crear un reloj.
 * Es una funcion que te devuelve a un puntero de una estructura del reloj creada.
 * @param tics_por_seg Cantidad de ticks por segundos que tendra el reloj.
 * @param toggleAlarma Funcion de callback que sirve para encender o apagar la alarma.
 * @return clock_t Devuelve un puntero al reloj.
 */
clock_t ClockCreate(int tics_por_seg, evento_t toggleAlarma);
/**
 * @brief Funcion para avanzar el ticks.
 * Es una funcion que se utiliza para avanzar el reloj, en el mismo tiene un comparador para
 * verificar si la alarma debe sonar o no.
 * @param reloj Puntero al reloj.
 * @return true En caso de que se cumpla un segundo.
 * @return false Caso de que no paso un segundo.
 */
bool ClockTick(clock_t reloj);
/**
 * @brief Funcion para obtener la hora.
 * Es una funcion que te devuelva la hora del reloj.
 * @param reloj Puntero al reloj.
 * @param hora Vector donde se guardara la hora.
 * @param size Tamaño que se copiara la hora.
 * @return true En caso de que ya se haya configurado el reloj.
 * @return false En caso de que no se haya configurado el reloj.
 */
bool ClockGetTime(clock_t reloj, uint8_t * hora, int size);
/**
 * @brief Funcion para configurar la hora del reloj.
 * @param reloj Puntero al reloj.
 * @param hora Vector con la hora a configurar deseada.
 * @param size Tamaño del vector que contiene la hora.
 * @return true Si se pudo hacer la operacion.
 * @return false Si no se pudo hacer la operacion.
 */
bool ClockSetTime(clock_t reloj, const uint8_t * hora, int size);
/**
 * @brief Funcion para obtener la hora de la alarma.
 * Es una funcion que te devuelva la hora de la alarma.
 * @param reloj Puntero al reloj.
 * @param hora Vector donde se guardara la hora de la alarma.
 * @param size Tamaño que se copiara la hora de la alarma.
 * @return true En caso de que ya se haya configurado la alarma.
 * @return false En caso de que no se haya configurado la alarma.
 */
bool ClockGetAlarma(clock_t reloj, uint8_t * hora, int size);
/**
 * @brief Funcion para comprobar si la alarma esta habilitada o no.
 * @param reloj Puntero al reloj.
 * @return true En caso de que este habilitada.
 * @return false En caso de que este deshabilitada.
 */
bool ClockGetAlarmaHabilitada(clock_t reloj);
/**
 * @brief Funcion para configurar la hora de la alarma del reloj.
 * @param reloj Puntero al reloj.
 * @param hora Vector con la hora de la alarma a configurar.
 * @param size Tamaño del vector que contiene la hora.
 */
void ClockSetAlarma(clock_t reloj, const uint8_t * hora, int size);
/**
 * @brief Funcion para cambiar el estado de la alarma.
 * @param reloj Puntero al reloj.
 */
void ClockToggleAlarma(clock_t reloj);
/**
 * @brief Funcion para posponer la alarma configurada.
 * @param reloj Puntero al reloj.
 * @param time_post Tiempo deseado para postergar.
 */
void ClockPosponerAlarma(clock_t reloj, uint8_t time_post);
/**
 * @brief Funcion para cancelar la alarma.
 * @param reloj Puntero al reloj.
 */
void ClockCancelarAlarma(clock_t reloj);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif