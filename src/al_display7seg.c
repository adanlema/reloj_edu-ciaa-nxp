/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include "al_display7seg.h"
#include "al_gpio.h"
#include "al_config_pin.h"
#include <stdint.h>
#include "chip.h"
#include <string.h>
/*==================[macros and definitions]=================================*/
#define CANTIDAD_DIGITOS 4

struct display_s {
    DigitalOutput_pt segmentos[7];
    DigitalOutput_pt seg_a;
    DigitalOutput_pt seg_b;
    DigitalOutput_pt seg_c;
    DigitalOutput_pt seg_d;
    DigitalOutput_pt seg_e;
    DigitalOutput_pt seg_f;
    DigitalOutput_pt seg_g;
    DigitalOutput_pt seg_punto;
    DigitalOutput_pt digitos[CANTIDAD_DIGITOS - 1];
    funcion_pt       funcion;
};

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
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void configpin_display(display_pt dis);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void configpin_display(display_pt dis) {
    DigitalPin_pt pin_dig1 =
        DigitalPin_Create(DIGIT_1_PORT, DIGIT_1_PIN, DIGIT_1_FUNC);
    DigitalPin_pt pin_dig2 =
        DigitalPin_Create(DIGIT_2_PORT, DIGIT_2_PIN, DIGIT_2_FUNC);
    DigitalPin_pt pin_dig3 =
        DigitalPin_Create(DIGIT_3_PORT, DIGIT_3_PIN, DIGIT_3_FUNC);
    DigitalPin_pt pin_dig4 =
        DigitalPin_Create(DIGIT_4_PORT, DIGIT_4_PIN, DIGIT_4_FUNC);

    DigitalPin_pt pin_seg_a =
        DigitalPin_Create(SEGMENT_A_PORT, SEGMENT_A_PIN, SEGMENT_A_FUNC);
    DigitalPin_pt pin_seg_b =
        DigitalPin_Create(SEGMENT_B_PORT, SEGMENT_B_PIN, SEGMENT_B_FUNC);
    DigitalPin_pt pin_seg_c =
        DigitalPin_Create(SEGMENT_C_PORT, SEGMENT_C_PIN, SEGMENT_C_FUNC);
    DigitalPin_pt pin_seg_d =
        DigitalPin_Create(SEGMENT_D_PORT, SEGMENT_D_PIN, SEGMENT_D_FUNC);
    DigitalPin_pt pin_seg_e =
        DigitalPin_Create(SEGMENT_E_PORT, SEGMENT_E_PIN, SEGMENT_E_FUNC);
    DigitalPin_pt pin_seg_f =
        DigitalPin_Create(SEGMENT_F_PORT, SEGMENT_F_PIN, SEGMENT_F_FUNC);
    DigitalPin_pt pin_seg_g =
        DigitalPin_Create(SEGMENT_G_PORT, SEGMENT_G_PIN, SEGMENT_G_FUNC);
    DigitalPin_pt pin_seg_punto =
        DigitalPin_Create(SEGMENT_P_PORT, SEGMENT_P_PIN, SEGMENT_P_FUNC);
    DigitalPin_GPIO(pin_dig1);
    DigitalPin_GPIO(pin_dig2);
    DigitalPin_GPIO(pin_dig3);
    DigitalPin_GPIO(pin_dig4);
    DigitalPin_GPIO(pin_seg_a);
    DigitalPin_GPIO(pin_seg_b);
    DigitalPin_GPIO(pin_seg_c);
    DigitalPin_GPIO(pin_seg_d);
    DigitalPin_GPIO(pin_seg_e);
    DigitalPin_GPIO(pin_seg_f);
    DigitalPin_GPIO(pin_seg_g);
    DigitalPin_GPIO(pin_seg_punto);

    dis->seg_a      = DigitalOutput_Create(SEGMENT_A_GPIO, SEGMENT_A_BIT);
    dis->seg_b      = DigitalOutput_Create(SEGMENT_B_GPIO, SEGMENT_B_BIT);
    dis->seg_c      = DigitalOutput_Create(SEGMENT_C_GPIO, SEGMENT_C_BIT);
    dis->seg_d      = DigitalOutput_Create(SEGMENT_D_GPIO, SEGMENT_D_BIT);
    dis->seg_e      = DigitalOutput_Create(SEGMENT_E_GPIO, SEGMENT_E_BIT);
    dis->seg_f      = DigitalOutput_Create(SEGMENT_F_GPIO, SEGMENT_F_BIT);
    dis->seg_g      = DigitalOutput_Create(SEGMENT_G_GPIO, SEGMENT_G_BIT);
    dis->seg_punto  = DigitalOutput_Create(SEGMENT_P_GPIO, SEGMENT_P_BIT);
    dis->digitos[0] = DigitalOutput_Create(DIGIT_1_GPIO, DIGIT_1_BIT);
    dis->digitos[1] = DigitalOutput_Create(DIGIT_2_GPIO, DIGIT_2_BIT);
    dis->digitos[2] = DigitalOutput_Create(DIGIT_3_GPIO, DIGIT_3_BIT);
    dis->digitos[3] = DigitalOutput_Create(DIGIT_4_GPIO, DIGIT_4_BIT);
}
/*==================[external functions definition]==========================*/
display_pt DisplayCreate(void) {
    static struct display_s self[1];
    memset(self, 0, sizeof(self));
    configpin_display(self);
    return self;
}

void DisplayWrite(display_pt AL, DigitalOutput_pt digito, char * cadena,
                  uint8_t size) {
    DigitalOutput_Activate(digito);
    for (int index = 0; index < 7; index++) {
        if (cadena[index]) {
            DigitalOutput_Activate(AL->segmentos[index]);
        }
    }
    DigitalOutput_Desactivate(digito);
}

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/