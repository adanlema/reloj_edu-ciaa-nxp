// Copyright 2022, Adan Lema <adanlema@hotmail.com>

#ifndef AL_CONFIG_PIN_H
#define AL_CONFIG_PIN_H

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef struct DigitalPin * DigitalPin_pt;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief Digital Pin Create
 * Crea un pin digital, en el cual debemos pasar su configuraciones para
 * describir su comportamiento.
 * @param port Puerto
 * @param pin Pin
 * @param modo configurar en modo GPIO,UART,PWM,etc
 * @return DigitalPin_pt Puntero a la estructura.
 */
DigitalPin_pt DigitalPin_Create(uint8_t port, uint8_t pin, uint32_t modo);
/**
 * @brief Digital Pin GPIO
 * Configura el pin proporcionado como GPIO.
 * @param AL Puntero al pin proporcionado.
 */
void DigitalPin_GPIO(DigitalPin_pt AL);

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/
#endif