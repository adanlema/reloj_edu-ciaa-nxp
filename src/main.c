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

#define EVENT_F1_OFF    (1 << 6)
#define EVENT_F2_OFF    (1 << 7)
#define EVENT_F3_OFF    (1 << 8)
#define EVENT_F4_OFF    (1 << 9)
#define EVENT_RC_OFF    (1 << 10)
#define EVENT_AC_OFF    (1 << 11)

#define EVENT_HORA      (1 << 15)
#define EVENT_ALARMA    (1 << 16)

#define AL_DELAY(VALOR) ((TickType_t)((VALOR * 0.005) / portTICK_PERIOD_MS))
#define INT_POR_SEG     1000
#define TICKS_POR_SEG   10

/*==================[internal data declaration]==============================*/
typedef struct mef_s {
    uint32_t key;
    modo_t   modo;
} * mef_s;

typedef struct boton_s {
    uint32_t key;
    // uint32_t fin;
    uint8_t posicion;
} * boton_s;
/*==================[internal functions declaration]=========================*/
static void StopByError(board_t board, uint8_t codigo);

static void IncrementTask(void * object);
static void KeyTask(void * object);
static void RefreshTask(void * object);
static void CambiarModoTask(void * object);
static void ConfigUserTask(void * object);
static void StartContadorTask(void * object);
static void FinishContadorTask(void * object);
/*==================[internal data definition]===============================*/
static board_t       edu_cia;
static clock_t       reloj;
static modo_t        mef_estado;
static uint8_t       hora_aux[CANTIDAD_DIGITOS];
static uint16_t      contador_pulsos[3] = {0};

static const uint8_t LIMITE_MINUTOS[]   = {5, 9};
static const uint8_t LIMITE_HORAS[]     = {2, 3};
EventGroupHandle_t   key_events;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void CambiarModo(modo_t estado) {
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
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 50);
            break;
        case AJUSTAR_HORAS_ACTUAL:
            DisplayNewParpadeoDigitos(edu_cia->display, (uint8_t[]){1, 1, 0, 0}, 50);
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 50);
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

static void IncrementTask(void * object) {
    uint8_t  hora[CANTIDAD_DIGITOS];
    uint32_t events = 0;
    bool     var;
    while (true) {
        var = ClockTick(reloj);
        if (mef_estado <= MOSTRANDO_HORA) {
            if (var) {
                ClockGetTime(reloj, hora, CANTIDAD_DIGITOS);
                DisplayWriteBCD(edu_cia->display, hora, CANTIDAD_DIGITOS);
            }
        }
        if (contador_pulsos[0] > 0) {
            contador_pulsos[0]++;
            if (contador_pulsos[0] = 3000) {
                events |= EVENT_HORA;
                contador_pulsos[0] = 0;
                xEventGroupSetBits(key_events, events);
            }
        }
        if (contador_pulsos[1] > 0) {
            contador_pulsos[1]++;
            if (contador_pulsos[1] = 3000) {
                events |= EVENT_ALARMA;
                contador_pulsos[1] = 0;
                xEventGroupSetBits(key_events, events);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

static void RefreshTask(void * object) {
    while (true) {
        DisplayRefresh(edu_cia->display);
        vTaskDelay(pdMS_TO_TICKS(5));
    };
}

static void CambiarModoTask(void * object) {
    mef_s options = object;
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        if (DigitalInput_GetState(edu_cia->f1)) {
            DisplayParpadeoPuntos(edu_cia->display, (uint8_t[]){0, 0, 0, 0}, 0);
            CambiarModo(AJUSTAR_MINUTOS_ACTUAL);
            ClockGetTime(reloj, hora_aux, sizeof(hora_aux));
            DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
        }
        if (DigitalInput_GetState(edu_cia->f2)) {
            DisplayPuntoAlarma(edu_cia->display, false);
            CambiarModo(AJUSTAR_MINUTOS_ALARMA);
            ClockGetAlarma(reloj, hora_aux, sizeof(hora_aux));
            DisplayWriteBCD(edu_cia->display, hora_aux, sizeof(hora_aux));
        }
        vTaskDelay(pdMS_TO_TICKS(200));
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
    uint32_t last_state, current_state, changes, events;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(100));

        current_state = 0;
        if (DigitalInput_GetState(placa->f1)) {
            current_state |= EVENT_F1_ON;
        }
        if (DigitalInput_GetState(placa->f2)) {
            current_state |= EVENT_F2_ON;
        }
        if (DigitalInput_GetState(placa->f3)) {
            current_state |= EVENT_F3_ON;
        }
        if (DigitalInput_GetState(placa->f4)) {
            current_state |= EVENT_F4_ON;
        }
        if (DigitalInput_GetState(placa->rechazar)) {
            current_state |= EVENT_RC_ON;
        }
        if (DigitalInput_GetState(placa->aceptar)) {
            current_state |= EVENT_AC_ON;
        }

        changes    = current_state ^ last_state;
        last_state = current_state;
        events     = ((changes & !current_state) << 6) | (changes & current_state);

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
// static void FinishContadorTask(void * object) {
//     boton_s options = object;
//     while (true) {
//         xEventGroupWaitBits(key_events, options->fin, TRUE, FALSE, portMAX_DELAY);
//         contador_pulsos[options->posicion] = 0;
//         vTaskDelay(pdMS_TO_TICKS(200));
//     }
// }

/*==================[external functions definition]==========================*/

int main(void) {

    static struct boton_s boton[2];
    static struct mef_s   mef[3];

    edu_cia = board_Create();
    reloj   = ClockCreate(TICKS_POR_SEG, AlarmaToggle);
    SysTickConfig(INT_POR_SEG);

    /* El reloj comienza sin configurar.*/

    CambiarModo(SIN_CONFIGURAR);

    /* Configuramos los struct de las tareas */
    boton[0].key      = EVENT_F1_ON;
    boton[0].posicion = 0;
    // boton[0].fin      = EVENT_F1_OFF;
    boton[1].key      = EVENT_F2_ON;
    boton[1].posicion = 1;
    // boton[1].fin      = EVENT_F2_OFF;

    mef[0].key  = EVENT_HORA;
    mef[0].modo = AJUSTAR_MINUTOS_ACTUAL;
    mef[1].key  = EVENT_ALARMA;
    mef[1].modo = AJUSTAR_MINUTOS_ALARMA;
    mef[2].key  = EVENT_F3_ON | EVENT_F4_ON | EVENT_AC_ON | EVENT_RC_ON;
    mef[2].modo = MOSTRANDO_HORA;

    /* Comenzamos con la creacion de las tareas.*/

    key_events = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(edu_cia, 1);
    }
    xTaskCreate(RefreshTask, "Refresco", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(KeyTask, "Keys", 256, (void *)edu_cia, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(CambiarModoTask, "ModificarHora", 256, &mef[0], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(CambiarModoTask, "ModificarAlarma", 256, &mef[1], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(ConfigUserTask, "ConfigurarUsuario", 256, &mef[2], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(StartContadorTask, "ContadorF1", 256, &boton[0], tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(StartContadorTask, "ContadorF2", 256, &boton[1], tskIDLE_PRIORITY + 2, NULL);
    // xTaskCreate(FinishContadorTask, "ContadorFinF1", 256, &boton[0], tskIDLE_PRIORITY + 2, NULL);
    // xTaskCreate(FinishContadorTask, "ContadorFinF2", 256, &boton[1], tskIDLE_PRIORITY + 2, NULL);
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