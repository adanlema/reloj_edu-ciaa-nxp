// Copyright 2022, Adan Lema <adanlema@hotmail.com>

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <string.h>
#include "chip.h"

#include "al_gpio.h"
#include "al_bsp.h"
#include "al_config_pin.h"
#include "al_display.h"
/*==================[macros and definitions]=================================*/

// Definiciones de los recursos asociados a las teclas del puncho
#define KEY_F1_PORT     4
#define KEY_F1_PIN      8
#define KEY_F1_FUNC     (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_F1_GPIO     5
#define KEY_F1_BIT      12

#define KEY_F2_PORT     4
#define KEY_F2_PIN      9
#define KEY_F2_FUNC     (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_F2_GPIO     5
#define KEY_F2_BIT      13

#define KEY_F3_PORT     4
#define KEY_F3_PIN      10
#define KEY_F3_FUNC     (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_F3_GPIO     5
#define KEY_F3_BIT      14

#define KEY_F4_PORT     6
#define KEY_F4_PIN      7
#define KEY_F4_FUNC     (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_F4_GPIO     5
#define KEY_F4_BIT      15

#define KEY_ACCEPT_PIN  2
#define KEY_ACCEPT_PORT 3
#define KEY_ACCEPT_FUNC (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_ACCEPT_GPIO 5
#define KEY_ACCEPT_BIT  9

#define KEY_CANCEL_PORT 3
#define KEY_CANCEL_PIN  1
#define KEY_CANCEL_FUNC (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP)
#define KEY_CANCEL_GPIO 5
#define KEY_CANCEL_BIT  8

// Definiciones de los recursos asociados al zumbador
#define LED_3_PORT 2
#define LED_3_PIN  12
#define LED_3_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define LED_3_GPIO 5
#define LED_3_BIT  2

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
board_pt BoardptCreate(void) {

    static struct board_s self[1];
    memset(self, 0, sizeof(self));
    /*  Configuracion de pines*/

    DigitalPin_pt pin_f1      = DigitalPin_Create(KEY_F1_PORT, KEY_F1_PIN, KEY_F1_FUNC);
    DigitalPin_pt pin_f2      = DigitalPin_Create(KEY_F2_PORT, KEY_F2_PIN, KEY_F2_FUNC);
    DigitalPin_pt pin_f3      = DigitalPin_Create(KEY_F3_PORT, KEY_F3_PIN, KEY_F3_FUNC);
    DigitalPin_pt pin_f4      = DigitalPin_Create(KEY_F4_PORT, KEY_F4_PIN, KEY_F4_FUNC);
    DigitalPin_pt pin_aceptar = DigitalPin_Create(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, KEY_ACCEPT_FUNC);
    DigitalPin_pt pin_rechazar =
        DigitalPin_Create(KEY_CANCEL_PORT, KEY_CANCEL_PIN, KEY_CANCEL_FUNC);
    DigitalPin_pt pin_buzzer = DigitalPin_Create(LED_3_PORT, LED_3_PIN, LED_3_FUNC);

    DigitalPin_GPIO(pin_f1);
    DigitalPin_GPIO(pin_f2);
    DigitalPin_GPIO(pin_f3);
    DigitalPin_GPIO(pin_f4);
    DigitalPin_GPIO(pin_aceptar);
    DigitalPin_GPIO(pin_rechazar);
    DigitalPin_GPIO(pin_buzzer);

    /*  Entradas  */
    self->f1 = DigitalInput_Create(KEY_F1_GPIO, KEY_F1_BIT, true);
    self->f2 = DigitalInput_Create(KEY_F2_GPIO, KEY_F2_BIT, true);
    self->f3 = DigitalInput_Create(KEY_F3_GPIO, KEY_F3_BIT, true);
    self->f4 = DigitalInput_Create(KEY_F4_GPIO, KEY_F4_BIT, true);

    /*  Salidas  */
    self->buz     = DigitalOutput_Create(LED_3_GPIO, LED_3_BIT);
    self->display = DisplayCreate();
    return self;
}

/**  doxygen end group definition */
/*==================[end of file]============================================*/