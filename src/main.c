/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "al_bsp.h"
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

#define EVENT_REFRESH   (1 << 20)

#define AL_DELAY(VALOR) ((TickType_t)((VALOR * 0.005) / portTICK_PERIOD_MS))
#define INT_POR_SEG     1000
#define TICKS_POR_SEG   10

/*==================[internal data declaration]==============================*/
typedef struct flash_s {
    uint32_t         key;
    DigitalOutput_pt led;
    uint32_t         delay;
} * flash_s;

typedef struct refresh_s {
    uint32_t  key;
    display_t dis;
    uint32_t  delay;
} * refresh_s;

typedef struct mef_s {
    uint32_t key;
    modo_t   modo;
} * mef_s;

typedef struct prueba_s {
    uint32_t  key;
    display_t dis;
    uint32_t  delay;
} * prueba_s;
/*==================[internal functions declaration]=========================*/
static void IncrementTask(void * object);
static void FlashTask(void * object);
static void KeyTask(void * object);
static void RefreshTask(void * object);
/*==================[internal data definition]===============================*/
static board_t edu_cia;
static clock_t reloj;
static uint8_t hora_auxiliar[CANTIDAD_DIGITOS];
// static uint16_t      contador_pulsos[3] = {0};

// static const uint8_t LIMITE_MINUTOS[]   = {5, 9};
// static const uint8_t LIMITE_HORAS[]     = {2, 3};
EventGroupHandle_t key_events;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void StopByError(board_t board, uint8_t codigo) {
    if (codigo == 1) {
        DigitalOutput_Activate(board->led_1);
    }
    if (codigo == 2) {
        DigitalOutput_Activate(board->led_2);
    }
    if (codigo == 3) {
        DigitalOutput_Activate(board->led_3);
    }
    while (true) {
    };
}

static void IncrementTask(void * object) {
    uint8_t hora[CANTIDAD_DIGITOS];
    while (true) {
        if (ClockTick(reloj)) {
            ClockGetTime(reloj, hora, CANTIDAD_DIGITOS);
            DisplayWriteBCD(edu_cia->display, hora, CANTIDAD_DIGITOS);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

static void RefreshTask(void * object) {
    refresh_s options = object;

    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        DisplayRefresh(options->dis);
        vTaskDelay(pdMS_TO_TICKS(options->delay));
    };
}

static void Task1(void * object) {
    prueba_s options = object;
    uint8_t  hora[CANTIDAD_DIGITOS];
    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        ClockGetTime(reloj, hora, CANTIDAD_DIGITOS);
        DisplayWriteBCD(options->dis, hora, CANTIDAD_DIGITOS);
        // DisplayWriteBCD(options->dis, (uint8_t[]){0, 1, 1, 0}, CANTIDAD_DIGITOS);
    };
}

static void FlashTask(void * object) {
    flash_s options = object;

    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);

        DigitalOutput_Activate(options->led);
        vTaskDelay(pdMS_TO_TICKS(options->delay));
        DigitalOutput_Desactivate(options->led);
        vTaskDelay(pdMS_TO_TICKS(options->delay));
    };
}

static void KeyTask(void * object) {
    board_t  placa = object;
    uint32_t last_state, current_state, changes, events;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(5));

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
        events |= EVENT_REFRESH;

        xEventGroupSetBits(key_events, events);
    }
}

/*==================[external functions definition]==========================*/

int main(void) {

    static struct flash_s   flash[3];
    static struct refresh_s refresh[1];
    static struct prueba_s  prueba[1];

    edu_cia = board_Create();
    reloj   = ClockCreate(TICKS_POR_SEG, AlarmaToggle);
    SysTickConfig(INT_POR_SEG);

    /* Configuramos las entradas para encender los leds*/

    flash[0].key     = EVENT_F1_ON;
    flash[0].led     = edu_cia->led_1;
    flash[0].delay   = 500;
    flash[1].key     = EVENT_F2_ON;
    flash[1].led     = edu_cia->led_2;
    flash[1].delay   = 500;
    flash[2].key     = EVENT_F3_ON;
    flash[2].led     = edu_cia->led_3;
    flash[2].delay   = 500;
    refresh[0].key   = EVENT_REFRESH;
    refresh[0].dis   = edu_cia->display;
    refresh[0].delay = 1;
    prueba[0].key    = EVENT_AC_ON;
    prueba[0].dis    = edu_cia->display;
    prueba[0].delay  = 100;

    /* Inicializamos la pantalla del LCD.*/
    ClockSetTime(reloj, hora_auxiliar, CANTIDAD_DIGITOS);
    DisplayWriteBCD(edu_cia->display, hora_auxiliar, CANTIDAD_DIGITOS);

    /* Comenzamos con la creacion de las tareas.*/

    key_events = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(edu_cia, 1);
    }
    if (xTaskCreate(KeyTask, "Keys", 256, (void *)edu_cia, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(edu_cia, 1);
    }
    if (xTaskCreate(RefreshTask, "Refresco", 256, &refresh[0], tskIDLE_PRIORITY + 2, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 2);
    }
    xTaskCreate(Task1, "TareaPrueba", 256, &prueba[0], tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(IncrementTask, "ClockTick", 256, NULL, tskIDLE_PRIORITY + 4, NULL);

    if (xTaskCreate(FlashTask, "RedTask", 256, &flash[0], tskIDLE_PRIORITY + 0, NULL) != pdPASS) {
        StopByError(edu_cia, 3);
    }
    if (xTaskCreate(FlashTask, "YellowTask", 256, &flash[1], tskIDLE_PRIORITY + 0, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 3);
    }
    if (xTaskCreate(FlashTask, "BlueTask", 256, &flash[2], tskIDLE_PRIORITY + 0, NULL) != pdPASS) {
        StopByError(edu_cia, 3);
    }

    vTaskStartScheduler();

    while (true) {
    };
    return 0;
}

// void InterupEvent(board_t board, clock_t rj) {
//     uint8_t    hora[TIME_SIZE];
//     TickType_t last_value = xTaskGetTickCountFromISR();

//     DisplayRefresh(board->display);
//     ClockGetTime(rj, hora, 4);
//     DisplayWriteBCD(board->display, hora, 4);
//     xTaskDelayUntil(&last_value, AL_DELAY(200));
// }

// void SysTick_Handler(void) {
//     InterupEvent(edu_cia, reloj);
// }
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/