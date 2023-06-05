/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef CIAA_H
#define CIAA_H

/*==================[inclusions]=============================================*/
#include "chip.h"
/*==================[macros]=================================================*/
#define DIGITS_GPIO  0

#define DIGIT_1_PORT 0
#define DIGIT_1_PIN  0
#define DIGIT_1_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define DIGIT_1_GPIO DIGITS_GPIO
#define DIGIT_1_BIT  0
#define DIGIT_1_MASK (1 << DIGIT_1_BIT)

#define DIGIT_2_PORT 0
#define DIGIT_2_PIN  1
#define DIGIT_2_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define DIGIT_2_GPIO DIGITS_GPIO
#define DIGIT_2_BIT  1
#define DIGIT_2_MASK (1 << DIGIT_2_BIT)

#define DIGIT_3_PORT 1
#define DIGIT_3_PIN  15
#define DIGIT_3_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define DIGIT_3_GPIO DIGITS_GPIO
#define DIGIT_3_BIT  2
#define DIGIT_3_MASK (1 << DIGIT_3_BIT)

#define DIGIT_4_PORT 1
#define DIGIT_4_PIN  17
#define DIGIT_4_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define DIGIT_4_GPIO DIGITS_GPIO
#define DIGIT_4_BIT  3
#define DIGIT_4_MASK (1 << DIGIT_4_BIT)

#define DIGITS_MASK  (DIGIT_1_MASK | DIGIT_2_MASK | DIGIT_3_MASK | DIGIT_4_MASK)

// Definiciones de los recursos asociados a los SEGMENTs de la pantalla
#define SEGMENTS_GPIO  2

#define SEGMENT_A_PORT 4
#define SEGMENT_A_PIN  0
#define SEGMENT_A_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define SEGMENT_A_GPIO SEGMENTS_GPIO
#define SEGMENT_A_BIT  0
#define SEGMENT_A_MASK (1 << SEGMENT_A_BIT)

#define SEGMENT_B_PORT 4
#define SEGMENT_B_PIN  1
#define SEGMENT_B_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
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
#define SEGMENT_D_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define SEGMENT_D_GPIO SEGMENTS_GPIO
#define SEGMENT_D_BIT  3
#define SEGMENT_D_MASK (1 << SEGMENT_D_BIT)

#define SEGMENT_E_PORT 4
#define SEGMENT_E_PIN  4
#define SEGMENT_E_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define SEGMENT_E_GPIO SEGMENTS_GPIO
#define SEGMENT_E_BIT  4
#define SEGMENT_E_MASK (1 << SEGMENT_E_BIT)

#define SEGMENT_F_PORT 4
#define SEGMENT_F_PIN  5
#define SEGMENT_F_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define SEGMENT_F_GPIO SEGMENTS_GPIO
#define SEGMENT_F_BIT  5
#define SEGMENT_F_MASK (1 << SEGMENT_F_BIT)

#define SEGMENT_G_PORT 4
#define SEGMENT_G_PIN  6
#define SEGMENT_G_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define SEGMENT_G_GPIO SEGMENTS_GPIO
#define SEGMENT_G_BIT  6
#define SEGMENT_G_MASK (1 << SEGMENT_G_BIT)

#define SEGMENTS_MASK                                                                              \
    (SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK |          \
     SEGMENT_F_MASK | SEGMENT_G_MASK)

#define SEGMENT_P_PORT 6
#define SEGMENT_P_PIN  8
#define SEGMENT_P_FUNC (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
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
#define LED_1_PORT 2
#define LED_1_PIN  10
#define LED_2_PORT 2
#define LED_2_PIN  11

#define LED_R_PORT 2
#define LED_R_PIN  0
#define LED_R_FUNC (SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define LED_G_PORT 2
#define LED_G_PIN  1
#define LED_B_PORT 2
#define LED_B_PIN  2

#define LED_3_PORT 2
#define LED_3_PIN  12
#define LED_3_FUNC (SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_INACT)
#define LED_3_GPIO 1
#define LED_3_BIT  12

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif