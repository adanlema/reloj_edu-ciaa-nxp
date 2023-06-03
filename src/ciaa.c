/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

/*==================[inclusions]=============================================*/
#include "ciaa.h"
#include "al_config_pin.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static void config_pin_auxiliar(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void config_pin_auxiliar(void) {
    DigitalPin_pt pin_led1 = DigitalPin_Create(LED_1_PORT, LED_1_PIN, LED_3_FUNC);
    DigitalPin_pt pin_led2 = DigitalPin_Create(LED_2_PORT, LED_2_PIN, LED_3_FUNC);
    DigitalPin_pt pin_ledr = DigitalPin_Create(LED_R_PORT, LED_R_PIN, LED_R_FUNC);
    DigitalPin_pt pin_ledg = DigitalPin_Create(LED_G_PORT, LED_G_PIN, LED_R_FUNC);
    DigitalPin_pt pin_ledb = DigitalPin_Create(LED_B_PORT, LED_B_PIN, LED_R_FUNC);

    DigitalPin_GPIO(pin_led1);
    DigitalPin_GPIO(pin_led2);
    DigitalPin_GPIO(pin_ledr);
    DigitalPin_GPIO(pin_ledg);
    DigitalPin_GPIO(pin_ledb);
}
/*==================[external functions definition]==========================*/
void configpin_display(void) {
    DigitalPin_pt pin_dig1      = DigitalPin_Create(DIGIT_1_PORT, DIGIT_1_PIN, DIGIT_1_FUNC);
    DigitalPin_pt pin_dig2      = DigitalPin_Create(DIGIT_2_PORT, DIGIT_2_PIN, DIGIT_2_FUNC);
    DigitalPin_pt pin_dig3      = DigitalPin_Create(DIGIT_3_PORT, DIGIT_3_PIN, DIGIT_3_FUNC);
    DigitalPin_pt pin_dig4      = DigitalPin_Create(DIGIT_4_PORT, DIGIT_4_PIN, DIGIT_4_FUNC);

    DigitalPin_pt pin_seg_a     = DigitalPin_Create(SEGMENT_A_PORT, SEGMENT_A_PIN, SEGMENT_A_FUNC);
    DigitalPin_pt pin_seg_b     = DigitalPin_Create(SEGMENT_B_PORT, SEGMENT_B_PIN, SEGMENT_B_FUNC);
    DigitalPin_pt pin_seg_c     = DigitalPin_Create(SEGMENT_C_PORT, SEGMENT_C_PIN, SEGMENT_C_FUNC);
    DigitalPin_pt pin_seg_d     = DigitalPin_Create(SEGMENT_D_PORT, SEGMENT_D_PIN, SEGMENT_D_FUNC);
    DigitalPin_pt pin_seg_e     = DigitalPin_Create(SEGMENT_E_PORT, SEGMENT_E_PIN, SEGMENT_E_FUNC);
    DigitalPin_pt pin_seg_f     = DigitalPin_Create(SEGMENT_F_PORT, SEGMENT_F_PIN, SEGMENT_F_FUNC);
    DigitalPin_pt pin_seg_g     = DigitalPin_Create(SEGMENT_G_PORT, SEGMENT_G_PIN, SEGMENT_G_FUNC);
    DigitalPin_pt pin_seg_punto = DigitalPin_Create(SEGMENT_P_PORT, SEGMENT_P_PIN, SEGMENT_P_FUNC);

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
}
void configpin_keys(void) {
    config_pin_auxiliar();
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
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/