// Copyright 2022, Adan Lema <adanlema@hotmail.com>

/*==================[inclusions]=============================================*/
#include "chip.h"
#include "al_bsp.h"
#include "al_config_pin.h"
#include "al_gpio.h"
#include <stdbool.h>

/*==================[macros and definitions]=================================*/

// Definiciones de los recursos asociados a los DIGITs de la pantalla

#define DIGITS_GPIO  0

#define DIGIT_1_PORT 0
#define DIGIT_1_PIN  0
#define DIGIT_1_FUNC SCU_MODE_FUNC0
#define DIGIT_1_GPIO DIGITS_GPIO
#define DIGIT_1_BIT  0
#define DIGIT_1_MASK (1 << DIGIT_1_BIT)

#define DIGIT_2_PORT 0
#define DIGIT_2_PIN  1
#define DIGIT_2_FUNC SCU_MODE_FUNC0
#define DIGIT_2_GPIO DIGITS_GPIO
#define DIGIT_2_BIT  1
#define DIGIT_2_MASK (1 << DIGIT_2_BIT)

#define DIGIT_3_PORT 1
#define DIGIT_3_PIN  15
#define DIGIT_3_FUNC SCU_MODE_FUNC0
#define DIGIT_3_GPIO DIGITS_GPIO
#define DIGIT_3_BIT  2
#define DIGIT_3_MASK (1 << DIGIT_3_BIT)

#define DIGIT_4_PORT 1
#define DIGIT_4_PIN  17
#define DIGIT_4_FUNC SCU_MODE_FUNC0
#define DIGIT_4_GPIO DIGITS_GPIO
#define DIGIT_4_BIT  3
#define DIGIT_4_MASK (1 << DIGIT_4_BIT)

#define DIGITS_MASK  (DIGIT_1_MASK | DIGIT_2_MASK | DIGIT_3_MASK | DIGIT_4_MASK)

// Definiciones de los recursos asociados a los SEGMENTs de la pantalla
#define SEGMENTS_GPIO  2

#define SEGMENT_A_PORT 4
#define SEGMENT_A_PIN  0
#define SEGMENT_A_FUNC SCU_MODE_FUNC0
#define SEGMENT_A_GPIO SEGMENTS_GPIO
#define SEGMENT_A_BIT  0
#define SEGMENT_A_MASK (1 << SEGMENT_A_BIT)

#define SEGMENT_B_PORT 4
#define SEGMENT_B_PIN  1
#define SEGMENT_B_FUNC SCU_MODE_FUNC0
#define SEGMENT_B_GPIO SEGMENTS_GPIO
#define SEGMENT_B_BIT  1
#define SEGMENT_B_MASK (1 << SEGMENT_B_BIT)

#define SEGMENT_C_PORT 4
#define SEGMENT_C_PIN  2
#define SEGMENT_C_FUNC SCU_MODE_FUNC0
#define SEGMENT_C_GPIO SEGMENTS_GPIO
#define SEGMENT_C_BIT  2
#define SEGMENT_C_MASK (1 << SEGMENT_C_BIT)

#define SEGMENT_D_PORT 4
#define SEGMENT_D_PIN  3
#define SEGMENT_D_FUNC SCU_MODE_FUNC0
#define SEGMENT_D_GPIO SEGMENTS_GPIO
#define SEGMENT_D_BIT  3
#define SEGMENT_D_MASK (1 << SEGMENT_D_BIT)

#define SEGMENT_E_PORT 4
#define SEGMENT_E_PIN  4
#define SEGMENT_E_FUNC SCU_MODE_FUNC0
#define SEGMENT_E_GPIO SEGMENTS_GPIO
#define SEGMENT_E_BIT  4
#define SEGMENT_E_MASK (1 << SEGMENT_E_BIT)

#define SEGMENT_F_PORT 4
#define SEGMENT_F_PIN  5
#define SEGMENT_F_FUNC SCU_MODE_FUNC0
#define SEGMENT_F_GPIO SEGMENTS_GPIO
#define SEGMENT_F_BIT  5
#define SEGMENT_F_MASK (1 << SEGMENT_F_BIT)

#define SEGMENT_G_PORT 4
#define SEGMENT_G_PIN  6
#define SEGMENT_G_FUNC SCU_MODE_FUNC0
#define SEGMENT_G_GPIO SEGMENTS_GPIO
#define SEGMENT_G_BIT  6
#define SEGMENT_G_MASK (1 << SEGMENT_G_BIT)

#define SEGMENTS_MASK                                                          \
    (SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK |       \
     SEGMENT_E_MASK | SEGMENT_F_MASK | SEGMENT_G_MASK)

#define SEGMENT_P_PORT 6
#define SEGMENT_P_PIN  8
#define SEGMENT_P_FUNC SCU_MODE_FUNC4
#define SEGMENT_P_GPIO 5
#define SEGMENT_P_BIT  16

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
#define BUZZER_PORT 2
#define BUZZER_PIN  2
#define BUZZER_FUNC SCU_MODE_FUNC4
#define BUZZER_GPIO 5
#define BUZZER_BIT  2
/*==================[internal data declaration]==============================*/
static struct board_s AL = {0};
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
board_pt BoardptCreate(void) {

    /*  Configuracion de pines*/

    DigitalPin_pt pin_f1 =
        DigitalPin_Create(KEY_F1_PORT, KEY_F1_PIN, KEY_F1_FUNC);
    DigitalPin_pt pin_f2 =
        DigitalPin_Create(KEY_F2_PORT, KEY_F2_PIN, KEY_F2_FUNC);
    DigitalPin_pt pin_f3 =
        DigitalPin_Create(KEY_F3_PORT, KEY_F3_PIN, KEY_F3_FUNC);
    DigitalPin_pt pin_f4 =
        DigitalPin_Create(KEY_F4_PORT, KEY_F4_PIN, KEY_F4_FUNC);
    DigitalPin_pt pin_aceptar =
        DigitalPin_Create(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, KEY_ACCEPT_FUNC);
    DigitalPin_pt pin_rechazar =
        DigitalPin_Create(KEY_CANCEL_PORT, KEY_CANCEL_PIN, KEY_CANCEL_FUNC);
    DigitalPin_pt pin_buzzer =
        DigitalPin_Create(BUZZER_PORT, BUZZER_PIN,
                          SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | BUZZER_FUNC);

    DigitalPin_GPIO(pin_f1);
    DigitalPin_GPIO(pin_f2);
    DigitalPin_GPIO(pin_f3);
    DigitalPin_GPIO(pin_f4);
    DigitalPin_GPIO(pin_aceptar);
    DigitalPin_GPIO(pin_rechazar);
    DigitalPin_GPIO(pin_buzzer);

    /*  Entradas  */
    AL.f1 = DigitalInput_Create(KEY_F1_GPIO, KEY_F1_BIT, true);
    AL.f2 = DigitalInput_Create(KEY_F2_GPIO, KEY_F2_BIT, true);
    AL.f3 = DigitalInput_Create(KEY_F3_GPIO, KEY_F3_BIT, true);
    AL.f4 = DigitalInput_Create(KEY_F4_GPIO, KEY_F4_BIT, true);

    /*  Salidas  */
    AL.buz = DigitalOutput_Create(BUZZER_GPIO, BUZZER_BIT);

    return &AL;
}

/**  doxygen end group definition */
/*==================[end of file]============================================*/