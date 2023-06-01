/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "chip.h"

#include "al_display.h"
#include "al_gpio.h"
#include "al_config_pin.h"
#include "ciaa.h"

/*==================[macros and definitions]=================================*/
#ifndef CANTIDAD_DIGITOS_MAXIMA 7
#define CANTIDAD_DIGITOS_MAXIMA 7
#endif
/*==================[internal data declaration]==============================*/
struct display_s {
    uint8_t                 digitos;
    uint8_t                 digito_activo;
    uint8_t                 memoria[CANTIDAD_DIGITOS_MAXIMA];
    struct display_driver_s driver[1];
};

static const uint8_t IMAGENES[] = {
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_C | SEGMENTO_D | SEGMENTO_E | SEGMENTO_F,              //! 0
    SEGMENTO_B | SEGMENTO_C,                                                                  //! 1
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_D | SEGMENTO_E | SEGMENTO_G,                           //! 2
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_C | SEGMENTO_D | SEGMENTO_G,                           //! 3
    SEGMENTO_B | SEGMENTO_C || SEGMENTO_F | SEGMENTO_G,                                       //! 4
    SEGMENTO_A | SEGMENTO_C | SEGMENTO_D | SEGMENTO_F | SEGMENTO_G,                           //! 5
    SEGMENTO_A | SEGMENTO_C | SEGMENTO_D | SEGMENTO_E | SEGMENTO_F | SEGMENTO_G,              //! 6
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_C,                                                     //! 7
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_C | SEGMENTO_D | SEGMENTO_E | SEGMENTO_F | SEGMENTO_G, //! 8
    SEGMENTO_A | SEGMENTO_B | SEGMENTO_C | SEGMENTO_F | SEGMENTO_G                            //! 9
};
/*==================[internal functions declaration]=========================*/
static display_t DisplayAllocate(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static display_t DisplayAllocate(void) {
    static struct display_s instances[1] = {0};
    return &instances[0];
}

// static void configpin_display(display_t display_al) {
//     DigitalPin_pt pin_dig1      = DigitalPin_Create(DIGIT_1_PORT, DIGIT_1_PIN, DIGIT_1_FUNC);
//     DigitalPin_pt pin_dig2      = DigitalPin_Create(DIGIT_2_PORT, DIGIT_2_PIN, DIGIT_2_FUNC);
//     DigitalPin_pt pin_dig3      = DigitalPin_Create(DIGIT_3_PORT, DIGIT_3_PIN, DIGIT_3_FUNC);
//     DigitalPin_pt pin_dig4      = DigitalPin_Create(DIGIT_4_PORT, DIGIT_4_PIN, DIGIT_4_FUNC);

//     DigitalPin_pt pin_seg_a     = DigitalPin_Create(SEGMENT_A_PORT, SEGMENT_A_PIN,
//     SEGMENT_A_FUNC); DigitalPin_pt pin_seg_b     = DigitalPin_Create(SEGMENT_B_PORT,
//     SEGMENT_B_PIN, SEGMENT_B_FUNC); DigitalPin_pt pin_seg_c     =
//     DigitalPin_Create(SEGMENT_C_PORT, SEGMENT_C_PIN, SEGMENT_C_FUNC); DigitalPin_pt pin_seg_d =
//     DigitalPin_Create(SEGMENT_D_PORT, SEGMENT_D_PIN, SEGMENT_D_FUNC); DigitalPin_pt pin_seg_e =
//     DigitalPin_Create(SEGMENT_E_PORT, SEGMENT_E_PIN, SEGMENT_E_FUNC); DigitalPin_pt pin_seg_f =
//     DigitalPin_Create(SEGMENT_F_PORT, SEGMENT_F_PIN, SEGMENT_F_FUNC); DigitalPin_pt pin_seg_g =
//     DigitalPin_Create(SEGMENT_G_PORT, SEGMENT_G_PIN, SEGMENT_G_FUNC); DigitalPin_pt pin_seg_punto
//     = DigitalPin_Create(SEGMENT_P_PORT, SEGMENT_P_PIN, SEGMENT_P_FUNC);
//     DigitalPin_GPIO(pin_dig1);
//     DigitalPin_GPIO(pin_dig2);
//     DigitalPin_GPIO(pin_dig3);
//     DigitalPin_GPIO(pin_dig4);
//     DigitalPin_GPIO(pin_seg_a);
//     DigitalPin_GPIO(pin_seg_b);
//     DigitalPin_GPIO(pin_seg_c);
//     DigitalPin_GPIO(pin_seg_d);
//     DigitalPin_GPIO(pin_seg_e);
//     DigitalPin_GPIO(pin_seg_f);
//     DigitalPin_GPIO(pin_seg_g);
//     DigitalPin_GPIO(pin_seg_punto);

//     display_al->segment[0] = DigitalOutput_Create(SEGMENT_A_GPIO, SEGMENT_A_BIT);
//     display_al->segment[1] = DigitalOutput_Create(SEGMENT_B_GPIO, SEGMENT_B_BIT);
//     display_al->segment[2] = DigitalOutput_Create(SEGMENT_C_GPIO, SEGMENT_C_BIT);
//     display_al->segment[3] = DigitalOutput_Create(SEGMENT_D_GPIO, SEGMENT_D_BIT);
//     display_al->segment[4] = DigitalOutput_Create(SEGMENT_E_GPIO, SEGMENT_E_BIT);
//     display_al->segment[5] = DigitalOutput_Create(SEGMENT_F_GPIO, SEGMENT_F_BIT);
//     display_al->segment[6] = DigitalOutput_Create(SEGMENT_G_GPIO, SEGMENT_G_BIT);
//     display_al->seg_punto  = DigitalOutput_Create(SEGMENT_P_GPIO, SEGMENT_P_BIT);
//     display_al->digito_1   = DigitalOutput_Create(DIGIT_1_GPIO, DIGIT_1_BIT);
//     display_al->digito_2   = DigitalOutput_Create(DIGIT_2_GPIO, DIGIT_2_BIT);
//     display_al->digito_3   = DigitalOutput_Create(DIGIT_3_GPIO, DIGIT_3_BIT);
//     display_al->digito_4   = DigitalOutput_Create(DIGIT_4_GPIO, DIGIT_4_BIT);
// }
/*==================[external functions definition]==========================*/

display_t DisplayCreate(uint8_t digitos, display_driver_t driver) {
    display_t display = DisplayAllocate();
    if (display) {
        display->digitos       = digitos;
        display->digito_activo = digitos - 1;
        memcpy(display->driver, driver, sizeof(display->driver));
        memset(display->memoria, 0, sizeof(display->memoria));
        display->driver->ScreenTurnOff();
    }
    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size) {
    memset(display->memoria, 0, sizeof(display->memoria));
    for (int indice = 0; indice < size; indice++) {
        if (index >= display->digitos)
            break;
        display->memoria[indice] = IMAGES[number[indice]];
    }
}

void DisplayRefresh(display_t display) {
    display->driver->ScreenTurnOff;
    display->digito_activo = (display->digito_activo + 1) % display->digitos;
    display->driver->ScreenTurnOn(display->memoria[display->digito_activo]);
    display->driver->DigitTurnOn(display->digito_activo);
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/