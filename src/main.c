/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "al_bsp.h"
#include "al_bcd.h"
#include "al_gpio.h"
#include "al_modo.h"
#include "reloj.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/*==================[macros and definitions]=================================*/

#define EVENT_F1_ON    (1 << 0)
#define EVENT_F2_ON    (1 << 1)
#define EVENT_F3_ON    (1 << 2)
#define EVENT_F4_ON    (1 << 3)
#define EVENT_RC_ON    (1 << 4)
#define EVENT_AC_ON    (1 << 5)

#define EVENT_F1_OFF   (1 << 8)
#define EVENT_F2_OFF   (1 << 9)
#define EVENT_F3_OFF   (1 << 10)
#define EVENT_F4_OFF   (1 << 11)
#define EVENT_RC_OFF   (1 << 12)
#define EVENT_AC_OFF   (1 << 13)

#define INT_POR_SEG    1000
#define TICKS_POR_SEG  10

#define COUNT_F1       0
#define COUNT_F2       1
#define COUNT_F_TIME   (3 * INT_POR_SEG)

#define COUNT_MAX      2
#define COUNT_MAX_TIME (10 * INT_POR_SEG)

/*==================[internal data declaration]==============================*/
//! Funcion de callback para los botones F1 y F2.
typedef void (*cambiar_estado_t)(void);
//! Estructura que guarda los eventos que modifican la hora y minutos.
typedef struct config_s {
    uint32_t key;
} * config_s;
//! Estructura para el funcionamiento de los botones F1 y F2.
typedef struct boton_s {
    uint8_t          posicion;
    uint32_t         key;
    uint32_t         delay;
    uint32_t         key_end;
    modo_t           estado;
    cambiar_estado_t funcion;
} * boton_s;

/*==================[internal functions declaration]=========================*/
static void CambiarMinActual(void);
static void CambiarMinAlarma(void);

static void IncrementTask(void * object);
static void RefreshTask(void * object);

static void KeyTask(void * object);
static void ConfigUserTask(void * object);

static void StartCountTask(void * object);
static void CountTask(void * object);
static void FinishCountTask(void * object);
static void FinishMaxTask(void * object);
/*==================[internal data definition]===============================*/
static board_t       edu_cia;
static clock_t       reloj;
static uint8_t       hora_aux[CANTIDAD_DIGITOS];
static uint16_t      contador_pulsos[3] = {0};
static mef_t         mef;

static const uint8_t LIMITE_MINUTOS[] = {5, 9};
static const uint8_t LIMITE_HORAS[]   = {2, 3};
EventGroupHandle_t   key_events;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void CambiarMinActual(void) {
    ClockGetTime(reloj, hora_aux, sizeof(hora_aux));
    DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
}
static void CambiarMinAlarma(void) {
    DisplayPuntoAlarma(edu_cia->display, false);
    ClockGetAlarma(reloj, hora_aux, sizeof(hora_aux));
    DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
}

/* TAREAS */

static void IncrementTask(void * object) {
    TickType_t last_value = xTaskGetTickCount();
    uint8_t    hora[CANTIDAD_DIGITOS];
    modo_t     estado;
    bool       valor;
    while (true) {
        valor  = ClockTick(reloj);
        estado = MefGetState(mef);
        if (estado <= MOSTRANDO_HORA) {
            if (valor) {
                ClockGetTime(reloj, hora, CANTIDAD_DIGITOS);
                DisplayWriteBCD(edu_cia->display, hora, CANTIDAD_DIGITOS);
            }
        }
        xTaskDelayUntil(&last_value, pdMS_TO_TICKS(1));
    }
}
static void RefreshTask(void * object) {
    while (true) {
        DisplayRefresh(edu_cia->display);
        vTaskDelay(pdMS_TO_TICKS(5));
    };
}

static void ConfigUserTask(void * object) {
    config_s options = object;
    modo_t   estado;
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        estado = MefGetState(mef);
        if (DigitalInput_GetState(edu_cia->aceptar)) {
            if (estado == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(edu_cia->buz)) {
                    ClockPosponerAlarma(reloj, TIME_POST);
                } else if (!ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(edu_cia->display, true);
                }
            } else if (estado == AJUSTAR_MINUTOS_ACTUAL) {
                CambiarMef(mef, AJUSTAR_HORAS_ACTUAL, &contador_pulsos[COUNT_MAX]);
            } else if (estado == AJUSTAR_HORAS_ACTUAL) {
                ClockSetTime(reloj, hora_aux, sizeof(hora_aux));
                CambiarMef(mef, MOSTRANDO_HORA, &contador_pulsos[COUNT_MAX]);
            } else if (estado == AJUSTAR_MINUTOS_ALARMA) {
                CambiarMef(mef, AJUSTAR_HORAS_ALARMA, &contador_pulsos[COUNT_MAX]);
            } else if (estado == AJUSTAR_HORAS_ALARMA) {
                ClockSetAlarma(reloj, hora_aux, sizeof(hora_aux));
                CambiarMef(mef, MOSTRANDO_HORA, &contador_pulsos[COUNT_MAX]);
                DisplayPuntoAlarma(edu_cia->display, true);
            }
        }

        if (DigitalInput_GetState(edu_cia->rechazar)) {
            if (estado == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(edu_cia->buz)) {
                    ClockCancelarAlarma(reloj);
                } else if (ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(edu_cia->display, false);
                    ClockCancelarAlarma(reloj);
                }
            } else {
                if (ClockGetTime(reloj, hora_aux, sizeof(hora_aux))) {
                    CambiarMef(mef, MOSTRANDO_HORA, &contador_pulsos[COUNT_MAX]);
                } else {
                    CambiarMef(mef, SIN_CONFIGURAR, &contador_pulsos[COUNT_MAX]);
                }
            }
        }
        if (DigitalInput_GetState(edu_cia->f3)) {
            if (contador_pulsos[COUNT_MAX] > 1) {
                contador_pulsos[COUNT_MAX] = 1;
            }
            if ((estado == AJUSTAR_MINUTOS_ACTUAL) || (estado == AJUSTAR_MINUTOS_ALARMA)) {
                DecrementarBCD(&hora_aux[2], LIMITE_MINUTOS);
            } else if ((estado == AJUSTAR_HORAS_ACTUAL) || (estado == AJUSTAR_HORAS_ALARMA)) {
                DecrementarBCD(&hora_aux[0], LIMITE_HORAS);
            }
            if ((estado == AJUSTAR_MINUTOS_ACTUAL) || (estado == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            } else if (((estado == AJUSTAR_MINUTOS_ALARMA) || (estado == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            }
        }

        if (DigitalInput_GetState(edu_cia->f4)) {
            if (contador_pulsos[COUNT_MAX] > 1) {
                contador_pulsos[COUNT_MAX] = 1;
            }
            if ((estado == AJUSTAR_MINUTOS_ACTUAL) || (estado == AJUSTAR_MINUTOS_ALARMA)) {
                IncrementarBCD(&hora_aux[2], LIMITE_MINUTOS);
            } else if ((estado == AJUSTAR_HORAS_ACTUAL) || (estado == AJUSTAR_HORAS_ALARMA)) {
                IncrementarBCD(&hora_aux[0], LIMITE_HORAS);
            }
            if ((estado == AJUSTAR_MINUTOS_ACTUAL) || (estado == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            } else if (((estado == AJUSTAR_MINUTOS_ALARMA) || (estado == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
static void KeyTask(void * object) {
    board_t  placa = object;
    uint16_t last_state, current_state, changes, events;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(100));

        current_state = 0;
        if (DigitalInput_GetState(placa->f1)) {
            current_state |= EVENT_F1_ON;
        } else {
            current_state |= EVENT_F1_OFF;
        }

        if (DigitalInput_GetState(placa->f2)) {
            current_state |= EVENT_F2_ON;
        } else {
            current_state |= EVENT_F2_OFF;
        }

        if (DigitalInput_GetState(placa->f3)) {
            current_state |= EVENT_F3_ON;
        } else {
            current_state |= EVENT_F3_OFF;
        }

        if (DigitalInput_GetState(placa->f4)) {
            current_state |= EVENT_F4_ON;
        } else {
            current_state |= EVENT_F4_OFF;
        }

        if (DigitalInput_GetState(placa->rechazar)) {
            current_state |= EVENT_RC_ON;
        } else {
            current_state |= EVENT_RC_OFF;
        }

        if (DigitalInput_GetState(placa->aceptar)) {
            current_state |= EVENT_AC_ON;
        } else {
            current_state |= EVENT_AC_OFF;
        }

        changes    = current_state ^ last_state;
        last_state = current_state;
        events     = changes & current_state;

        xEventGroupSetBits(key_events, events);
    }
}

static void StartCountTask(void * object) {
    boton_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        contador_pulsos[options->posicion] = 1;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
static void CountTask(void * object) {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1));
        if (contador_pulsos[COUNT_F1] > 0) {
            contador_pulsos[COUNT_F1]++;
        }
        if (contador_pulsos[COUNT_F2] > 0) {
            contador_pulsos[COUNT_F2]++;
        }
        if (contador_pulsos[COUNT_MAX] > 0) {
            contador_pulsos[COUNT_MAX]++;
        }
    }
}
static void FinishCountTask(void * object) {
    boton_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key_end, TRUE, FALSE, portMAX_DELAY);
        if (contador_pulsos[options->posicion] > options->delay) {
            CambiarMef(mef, options->estado, &contador_pulsos[COUNT_MAX]);
            options->funcion();
        }
        contador_pulsos[options->posicion] = 0;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
static void FinishMaxTask(void * object) {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(250));
        if (contador_pulsos[COUNT_MAX] > COUNT_MAX_TIME) {
            CambiarMef(mef, MOSTRANDO_HORA, &contador_pulsos[COUNT_MAX]);
        }
    }
}

/*==================[external functions definition]==========================*/

int main(void) {

    static struct boton_s  boton[2];
    static struct config_s config[1];

    edu_cia = board_Create();
    reloj   = ClockCreate(TICKS_POR_SEG, AlarmaToggle);
    SysTickConfig(INT_POR_SEG);
    mef = MefCreate(edu_cia);

    /* El reloj comienza sin configurar.*/
    CambiarMef(mef, SIN_CONFIGURAR, &contador_pulsos[COUNT_MAX]);

    /* Configuramos los struct de las tareas */
    boton[COUNT_F1].key      = EVENT_F1_ON;
    boton[COUNT_F1].posicion = COUNT_F1;
    boton[COUNT_F1].delay    = COUNT_F_TIME;
    boton[COUNT_F1].estado   = AJUSTAR_MINUTOS_ACTUAL;
    boton[COUNT_F1].key_end  = EVENT_F1_OFF;
    boton[COUNT_F1].funcion  = CambiarMinActual;

    boton[COUNT_F2].key      = EVENT_F2_ON;
    boton[COUNT_F2].posicion = COUNT_F2;
    boton[COUNT_F2].delay    = COUNT_F_TIME;
    boton[COUNT_F2].estado   = AJUSTAR_MINUTOS_ALARMA;
    boton[COUNT_F2].key_end  = EVENT_F2_OFF;
    boton[COUNT_F2].funcion  = CambiarMinAlarma;

    config[0].key            = EVENT_F3_ON | EVENT_F4_ON | EVENT_AC_ON | EVENT_RC_ON;

    /* Comenzamos con la creacion de las tareas.*/

    key_events = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(edu_cia, 1);
    }

    if (xTaskCreate(IncrementTask, "ClockTick", 256, NULL, tskIDLE_PRIORITY + 4, NULL) != pdPASS) {
        StopByError(edu_cia, 5);
    }
    if (xTaskCreate(RefreshTask, "RefrescoDisplay", 256, NULL, tskIDLE_PRIORITY + 1, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 1);
    }
    if (xTaskCreate(KeyTask, "Teclado", 256, (void *)edu_cia, tskIDLE_PRIORITY + 3, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 3);
    }
    if (xTaskCreate(ConfigUserTask, "ConfigurarUsuario", 256, &config[0], tskIDLE_PRIORITY + 2,
                    NULL) != pdPASS) {
        StopByError(edu_cia, 2);
    }

    /* Contadores */
    if (xTaskCreate(StartCountTask, "StartF1", 256, &boton[COUNT_F1], tskIDLE_PRIORITY + 2, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 2);
    }
    if (xTaskCreate(StartCountTask, "StartF2", 256, &boton[COUNT_F2], tskIDLE_PRIORITY + 2, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 2);
    }

    if (xTaskCreate(CountTask, "IncrementarContador", 256, NULL, tskIDLE_PRIORITY + 1, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 1);
    }

    if (xTaskCreate(FinishCountTask, "FinishF1", 256, &boton[COUNT_F1], tskIDLE_PRIORITY + 2,
                    NULL) != pdPASS) {
        StopByError(edu_cia, 2);
    }
    if (xTaskCreate(FinishCountTask, "FinishF2", 256, &boton[COUNT_F2], tskIDLE_PRIORITY + 2,
                    NULL) != pdPASS) {
        StopByError(edu_cia, 2);
    }
    if (xTaskCreate(FinishMaxTask, "FinishMax", 256, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
        StopByError(edu_cia, 2);
    }

    vTaskStartScheduler();

    while (true) {
    };
    return 0;
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
