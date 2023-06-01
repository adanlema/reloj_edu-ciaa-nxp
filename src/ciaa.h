/* Copyright 2023, Adan Lema <adanlema@hotmail.com> */

#ifndef TEMPLATE_H
#define TEMPLATE_H

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
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

#define SEGMENTS_MASK                                                                              \
    (SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK |          \
     SEGMENT_F_MASK | SEGMENT_G_MASK)

#define SEGMENT_P_PORT 6
#define SEGMENT_P_PIN  8
#define SEGMENT_P_FUNC SCU_MODE_FUNC4
#define SEGMENT_P_GPIO 5
#define SEGMENT_P_BIT  16

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif