/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "al_bsp.h"
#include "al_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/*==================[macros and definitions]=================================*/

#define EVENT_F1_ON     (1 << 0)
#define EVENT_F2_ON     (1 << 1)
#define EVENT_F3_ON     (1 << 2)
#define EVENT_F4_ON     (1 << 3)

#define EVENT_F1_OFF    (1 << 4)
#define EVENT_F2_OFF    (1 << 5)
#define EVENT_F3_OFF    (1 << 6)
#define EVENT_F4_OFF    (1 << 7)

#define AL_DELAY(VALOR) ((TickType_t)((VALOR * 0.005) / portTICK_PERIOD_MS))
/*==================[internal data declaration]==============================*/
typedef struct flash_s {
    uint32_t         key;
    DigitalOutput_pt led;
    uint32_t         delay;
} * flash_s;
// typedef struct flash_s {
//     DigitalOutput_pt led;
//     uint32_t         delay;
// } * flash_s;

/*==================[internal functions declaration]=========================*/
static void FlashTask(void * object);
static void KeyTask(void * object);
/*==================[internal data definition]===============================*/
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

static void FlashTask(void * object) {
    flash_s options = object;

    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);

        DigitalOutput_Activate(options->led);
        vTaskDelay(AL_DELAY(options->delay));
        DigitalOutput_Desactivate(options->led);
        vTaskDelay(AL_DELAY(options->delay));
    };
}

static void KeyTask(void * object) {
    board_t  placa = object;
    uint32_t last_state, current_state, changes, events;

    while (true) {
        vTaskDelay(AL_DELAY(200));

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
        // if (!DigitalInput_GetState(placa->rechazar)) {
        //     current_state |= EVENT_RE_ON;
        // }
        // if (!DigitalInput_GetState(placa->aceptar)) {
        //     current_state |= EVENT_AC_ON;
        // }

        changes    = current_state ^ last_state;
        last_state = current_state;
        events     = ((changes & !current_state) << 4) | (changes & current_state);

        xEventGroupSetBits(key_events, events);
    }
}

// static void FlashTask(void * object) {
//     flash_s    options    = object;
//     TickType_t last_value = xTaskGetTickCount();
//     while (true) {
//         // xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
//         DigitalOutput_Activate(options->led);
//         vTaskDelay(AL_DELAY(options->delay));
//         DigitalOutput_Desactivate(options->led);
//         xTaskDelayUntil(&last_value, AL_DELAY(1000));
//         //   vTaskDelay(AL_DELAY(options->delay)));
//     }
// }

/*==================[external functions definition]==========================*/

int main(void) {

    static struct flash_s flash[3];
    board_t               edu_cia = board_Create();

    /* Configuramos las entradas para encender los leds*/
    flash[0].key   = EVENT_F1_ON;
    flash[0].led   = edu_cia->led_1;
    flash[0].delay = 500;
    flash[1].key   = EVENT_F2_ON;
    flash[1].led   = edu_cia->led_2;
    flash[1].delay = 500;
    flash[2].key   = EVENT_F3_ON | EVENT_F1_OFF;
    flash[2].led   = edu_cia->led_3;
    flash[2].delay = 500;

    key_events     = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(edu_cia, 1);
    }
    if (xTaskCreate(KeyTask, "Keys", 256, (void *)edu_cia, tskIDLE_PRIORITY + 3, NULL) != pdPASS) {
        StopByError(edu_cia, 1);
    }

    if (xTaskCreate(FlashTask, "RedTask", 256, &flash[0], tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(edu_cia, 2);
    }
    if (xTaskCreate(FlashTask, "YellowTask", 256, &flash[1], tskIDLE_PRIORITY + 1, NULL) !=
        pdPASS) {
        StopByError(edu_cia, 2);
    }
    if (xTaskCreate(FlashTask, "BlueTask", 256, &flash[2], tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(edu_cia, 3);
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