// Copyright 2022, Adan Lema <adanlema@hotmail.com>

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <string.h>
#include "chip.h"

#include "al_gpio.h"
#include "al_bsp.h"
#include "ciaa.h"
#include "al_display.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
static struct board_s board = {0};
/*==================[internal functions declaration]=========================*/
void ScreenTurnOff(void);
void ScreenTurnOn(uint8_t segments);
void DigitTurnOn(uint8_t digit);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void ScreenTurnOff(void) {
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false);
}

void ScreenTurnOn(uint8_t segments) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, (segments)&SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (segments & SEGMENTO_P));
}

void DigitTurnOn(uint8_t digit) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, DIGITS_GPIO, (1 << digit) & DIGITS_MASK);
}

/*==================[external functions definition]==========================*/
board_pt BoardptCreate(void) {
    /*  Configuracion de pines*/
    configpin_display();
    configpin_keys();
    /*  Entradas  */
    board.f1 = DigitalInput_Create(KEY_F1_GPIO, KEY_F1_BIT, true);
    board.f2 = DigitalInput_Create(KEY_F2_GPIO, KEY_F2_BIT, true);
    board.f3 = DigitalInput_Create(KEY_F3_GPIO, KEY_F3_BIT, true);
    board.f4 = DigitalInput_Create(KEY_F4_GPIO, KEY_F4_BIT, true);
    /*  Salidas  */
    board.buz     = DigitalOutput_Create(LED_3_GPIO, LED_3_BIT);
    board.display = DisplayCreate(4, &(struct display_driver_s){
                                         .ScreenTurnOff = ScreenTurnOff,
                                         .ScreenTurnOn  = ScreenTurnOn,
                                         .DigitTurnOn   = DigitTurnOn,
                                     });
    return &board;
}
/**  doxygen end group definition */
/*==================[end of file]============================================*/