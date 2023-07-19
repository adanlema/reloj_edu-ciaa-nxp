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

#define EVENT_F1_ON     (1 << 0)
#define EVENT_F2_ON     (1 << 1)
#define EVENT_F3_ON     (1 << 2)
#define EVENT_F4_ON     (1 << 3)
#define EVENT_RC_ON     (1 << 4)
#define EVENT_AC_ON     (1 << 5)

#define EVENT_F1_OFF    (1 << 8)
#define EVENT_F2_OFF    (1 << 9)
#define EVENT_F3_OFF    (1 << 10)
#define EVENT_F4_OFF    (1 << 11)
#define EVENT_RC_OFF    (1 << 12)
#define EVENT_AC_OFF    (1 << 13)

#define AL_DELAY(VALOR) ((TickType_t)((VALOR * 0.005) / portTICK_PERIOD_MS))
#define INT_POR_SEG     1000
#define TICKS_POR_SEG   10

/*==================[internal data declaration]==============================*/
typedef void (*cambiar_estado_t)(void);

typedef struct mef_s {
    uint32_t key;
} * mef_s;

typedef struct boton_s {
    uint8_t          posicion;
    uint32_t         key;
    uint32_t         key_end;
    modo_t           estado;
    cambiar_estado_t funcion;
} * boton_s;
/*==================[internal functions declaration]=========================*/
static void StopByError(board_t board, uint8_t codigo);
static void CambiarModo(modo_t estado);
static void CambiarMinActual(void);
static void CambiarMinAlarma(void);

static void IncrementTask(void * object);
static void KeyTask(void * object);
static void RefreshTask(void * object);
static void ConfigUserTask(void * object);
static void StartContadorTask(void * object);
static void CountTask(void * object);
static void FinishContadorTask(void * object);
/*==================[internal data definition]===============================*/
static board_t       edu_cia;
static clock_t       reloj;
static modo_t        mef_estado;
static uint8_t       hora_aux[CANTIDAD_DIGITOS];
static uint32_t      contador_pulsos[3] = {0};

static const uint8_t LIMITE_MINUTOS[]   = {5, 9};
static const uint8_t LIMITE_HORAS[]     = {2, 3};
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
static void CambiarModo(modo_t estado) {
    mef_estado = estado;
    switch (mef_estado) {
        case SIN_CONFIGURAR:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){1, 1, 1, 1}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 1, 0, 0}, 50);
            break;
        case MOSTRANDO_HORA:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 1, 0, 0}, 50);

            break;
        case AJUSTAR_MINUTOS_ACTUAL:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){0, 0, 1, 1}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 0);
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){1, 1, 0, 0}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 0);
            break;
        case AJUSTAR_MINUTOS_ALARMA:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){0, 0, 1, 1}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 1, 1}, 50);
            break;
        case AJUSTAR_HORAS_ALARMA:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){1, 1, 0, 0}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){1, 1, 0, 0}, 50);
            break;
        default:
            break;
    }
}
static void StopByError(board_t board, uint8_t codigo) {
    switch (codigo) {
        case 1:
            DigitalOutput_Activate(board->led_1);
            DigitalOutput_Activate(board->led_2);
            DigitalOutput_Activate(board->led_3);
            break;
        case 2:
            DigitalOutput_Activate(board->led_1);
            DigitalOutput_Activate(board->led_3);
            break;
        case 3:
            DigitalOutput_Activate(board->led_1);
            DigitalOutput_Activate(board->led_2);
            break;
        default:
            // Si el código no coincide con ninguno de los casos anteriores, no se activa ningún LED
            // adicional, pero se sale del switch.
            break;
    }
    while (true) {
    }
}

/* TAREAS */

static void IncrementTask(void * object) {
    TickType_t last_value = xTaskGetTickCount();
    uint8_t    hora[CANTIDAD_DIGITOS];
    bool       var;
    while (true) {
        var = ClockTick(reloj);
        if (mef_estado <= MOSTRANDO_HORA) {
            if (var) {
                ClockGetTime(reloj, hora, CANTIDAD_DIGITOS);
                DisplayWriteBCD(edu_cia->display, hora, CANTIDAD_DIGITOS);
            }
        }
        // vTaskDelay(pdMS_TO_TICKS(1));
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
    mef_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        if (DigitalInput_GetState(edu_cia->aceptar)) {
            if (mef_estado == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(edu_cia->buz)) {
                    ClockPosponerAlarma(reloj, TIME_POST);
                } else if (!ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(edu_cia->display, true);
                }
            } else if (mef_estado == AJUSTAR_MINUTOS_ACTUAL) {
                CambiarModo(AJUSTAR_HORAS_ACTUAL);
            } else if (mef_estado == AJUSTAR_HORAS_ACTUAL) {
                ClockSetTime(reloj, hora_aux, sizeof(hora_aux));
                CambiarModo(MOSTRANDO_HORA);
            } else if (mef_estado == AJUSTAR_MINUTOS_ALARMA) {
                CambiarModo(AJUSTAR_HORAS_ALARMA);
            } else if (mef_estado == AJUSTAR_HORAS_ALARMA) {
                ClockSetAlarma(reloj, hora_aux, sizeof(hora_aux));
                CambiarModo(MOSTRANDO_HORA);
                DisplayPuntoAlarma(edu_cia->display, true);
            }
        }

        if (DigitalInput_GetState(edu_cia->rechazar)) {
            if (mef_estado == MOSTRANDO_HORA) {
                if (DigitalOutput_GetState(edu_cia->buz)) {
                    ClockCancelarAlarma(reloj);
                } else if (ClockGetAlarmaHabilitada(reloj)) {
                    ClockToggleAlarma(reloj);
                    DisplayPuntoAlarma(edu_cia->display, false);
                    ClockCancelarAlarma(reloj);
                }
            } else {
                if (ClockGetTime(reloj, hora_aux, sizeof(hora_aux))) {
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        }
        if (DigitalInput_GetState(edu_cia->f3)) {
            if ((mef_estado == AJUSTAR_MINUTOS_ACTUAL) || (mef_estado == AJUSTAR_MINUTOS_ALARMA)) {
                DecrementarBCD(&hora_aux[2], LIMITE_MINUTOS);
            } else if ((mef_estado == AJUSTAR_HORAS_ACTUAL) ||
                       (mef_estado == AJUSTAR_HORAS_ALARMA)) {
                DecrementarBCD(&hora_aux[0], LIMITE_HORAS);
            }
            if ((mef_estado == AJUSTAR_MINUTOS_ACTUAL) || (mef_estado == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            } else if (((mef_estado == AJUSTAR_MINUTOS_ALARMA) ||
                        (mef_estado == AJUSTAR_HORAS_ALARMA))) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            }
        }

        if (DigitalInput_GetState(edu_cia->f4)) {
            if ((mef_estado == AJUSTAR_MINUTOS_ACTUAL) || (mef_estado == AJUSTAR_MINUTOS_ALARMA)) {
                IncrementarBCD(&hora_aux[2], LIMITE_MINUTOS);
            } else if ((mef_estado == AJUSTAR_HORAS_ACTUAL) ||
                       (mef_estado == AJUSTAR_HORAS_ALARMA)) {
                IncrementarBCD(&hora_aux[0], LIMITE_HORAS);
            }
            if ((mef_estado == AJUSTAR_MINUTOS_ACTUAL) || (mef_estado == AJUSTAR_HORAS_ACTUAL)) {
                DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
            } else if (((mef_estado == AJUSTAR_MINUTOS_ALARMA) ||
                        (mef_estado == AJUSTAR_HORAS_ALARMA))) {
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

static void StartContadorTask(void * object) {
    boton_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        contador_pulsos[options->posicion] = 1;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
static void CountTask(void * object) {
    boton_s options = object;
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1));
        if (contador_pulsos[options->posicion] > 0) {
            contador_pulsos[options->posicion]++;
        }
    }
}
static void FinishContadorTask(void * object) {
    boton_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key_end, TRUE, FALSE, portMAX_DELAY);
        if (contador_pulsos[options->posicion] > 3000) {
            CambiarModo(options->estado);
            options->funcion();
        }
        contador_pulsos[options->posicion] = 0;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

/*==================[external functions definition]==========================*/

int main(void) {

    static struct boton_s boton[3];
    static struct mef_s   mef[1];

    edu_cia = board_Create();
    reloj   = ClockCreate(TICKS_POR_SEG, AlarmaToggle);
    SysTickConfig(INT_POR_SEG);

    /* El reloj comienza sin configurar.*/

    CambiarModo(SIN_CONFIGURAR);

    /* Configuramos los struct de las tareas */
    boton[0].key      = EVENT_F1_ON;
    boton[0].posicion = 0;
    boton[0].estado   = AJUSTAR_MINUTOS_ACTUAL;
    boton[0].key_end  = EVENT_F1_OFF;
    boton[0].funcion  = CambiarMinActual;

    boton[1].key      = EVENT_F2_ON;
    boton[1].posicion = 1;
    boton[1].estado   = AJUSTAR_MINUTOS_ALARMA;
    boton[1].key_end  = EVENT_F2_OFF;
    boton[1].funcion  = CambiarMinAlarma;

    mef[0].key        = EVENT_F3_ON | EVENT_F4_ON | EVENT_AC_ON | EVENT_RC_ON;

    /* Comenzamos con la creacion de las tareas.*/

    key_events = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(edu_cia, 1);
    }
    xTaskCreate(RefreshTask, "Refresco", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(KeyTask, "Keys", 256, (void *)edu_cia, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(ConfigUserTask, "ConfigurarUsuario", 256, &mef[0], tskIDLE_PRIORITY + 2, NULL);

    xTaskCreate(StartContadorTask, "ContadorF1", 256, &boton[0], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(CountTask, "ConteoF1", 256, &boton[0], tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(FinishContadorTask, "ContadorFinalF1", 256, &boton[0], tskIDLE_PRIORITY + 2, NULL);

    xTaskCreate(StartContadorTask, "ContadorF2", 256, &boton[1], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(CountTask, "ConteoF2", 256, &boton[1], tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(FinishContadorTask, "ContadorFinalF2", 256, &boton[1], tskIDLE_PRIORITY + 2, NULL);

    xTaskCreate(IncrementTask, "ClockTick", 256, NULL, tskIDLE_PRIORITY + 4, NULL);

    vTaskStartScheduler();

    while (true) {
    };
    return 0;
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
