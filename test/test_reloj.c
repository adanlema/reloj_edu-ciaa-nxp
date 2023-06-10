/*
Pruebas
‣ Al inicializar el reloj está en 00:00 y con hora invalida.
‣ Al ajustar la hora el reloj queda en hora y es válida.
‣ Probar horas invalidas y verificar que las rechaza.
‣ Después de n ciclos de reloj la hora avanza un segundo, diez segundos, un minutos, diez minutos,
una hora, diez horas y un día completo.
‣ Fijar la hora de la alarma y consultarla.
‣ Fijar la alarma y avanzar el reloj para que suene.
‣ Fijar la alarma, deshabilitarla y avanzar el reloj para no suene.
‣ Hacer sonar la alarma y posponerla.
‣ Hacer sonar la alarma y cancelarla hasta el otro dia..

    Test pendientes...
- Verificar el ingrego de una hora falsa.
*/

/*==================[inclusions]=============================================*/
#include "reloj.h"
#include <string.h>
#include "unity.h"
/*==================[macros and definitions]=================================*/
#define TICKS_POR_SEG 5
#define SIMULAR_SEGUNDOS(VALOR, FUNCION)                                                           \
    for (int count = 0; count < VALOR; count++) {                                                  \
        for (int indice = 0; indice < TICKS_POR_SEG; indice++) {                                   \
            FUNCION;                                                                               \
        }                                                                                          \
    }

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void CaptureAlarmEvent(void);
void CancelarAlarma(void);
/*==================[internal data definition]===============================*/
static bool          alarm_event_fired;
static clock_t       reloj;
static uint8_t       hora[TIME_SIZE];
static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

//! Funcion para simular que se encendio la alarma.
void CaptureAlarmEvent(void) {
    alarm_event_fired = true;
}
//! Funcion para simular que la alarma dejo de sonar.
void CancelarAlarma(void) {
    alarm_event_fired = false;
}
/*==================[external functions definition]==========================*/

void setUp(void) {
    alarm_event_fired = false;
    reloj             = ClockCreate(TICKS_POR_SEG, CaptureAlarmEvent);
    ClockSetTime(reloj, INICIAL, sizeof(INICIAL));
}

//‣ Al inicializar el reloj está en 00:00 y con hora invalida.
void test_start_up(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    hora[0]                         = 1;
    clock_t reloj                   = ClockCreate(TICKS_POR_SEG, CaptureAlarmEvent);

    TEST_ASSERT_FALSE(ClockGetTime(reloj, hora, 6));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, 6);
}

//‣ Al ajustar la hora el reloj queda en hora y es válida.
void test_ajustar_hora(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    clock_t              reloj      = ClockCreate(TICKS_POR_SEG, CaptureAlarmEvent);

    TEST_ASSERT_TRUE(ClockSetTime(reloj, ESPERADO, 4));
    TEST_ASSERT_TRUE(ClockGetTime(reloj, hora, 6));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, 6);
}

//‣ Después de n ciclos de reloj la hora avanza un segundo, diez segundos, un minutos, diez minutos,
// una hora, diez horas y un día completo.
void test_increment_seg_unidad(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
    // uint8_t              hora[TIME_SIZE];

    SIMULAR_SEGUNDOS(1, ClockTick(reloj));
    ClockGetTime(reloj, hora, 6);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, 6);
}

void test_increment_seg_decena(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};

    SIMULAR_SEGUNDOS(10, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_increment_min_unidad(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};

    SIMULAR_SEGUNDOS(60, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_increment_min_decena(void) {
    static const uint8_t ESPERADO[] = {1, 2, 4, 4, 0, 0};

    SIMULAR_SEGUNDOS(10 * 60, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_increment_hora_unidad(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};

    SIMULAR_SEGUNDOS(60 * 60, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_increment_hora_decena(void) {
    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};

    SIMULAR_SEGUNDOS(10 * 60 * 60, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_increment_dia(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};

    SIMULAR_SEGUNDOS(24 * 60 * 60, ClockTick(reloj));
    ClockGetTime(reloj, hora, TIME_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Fijar la hora de la alarma y consultarla. */

void test_ajustar_alarma(void) {
    static const uint8_t ESPERADO[]             = {1, 2, 3, 4, 0, 0};
    uint8_t              hora_alarma[TIME_SIZE] = {0xFF};

    ClockSetAlarma(reloj, ESPERADO, 4);
    TEST_ASSERT_TRUE(ClockGetAlarma(reloj, hora_alarma, 6));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora_alarma, 6);
}

/* Fijar la alarma y avanzar el reloj para que suene. */

void test_alarma_activada(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
    ClockSetAlarma(reloj, ESPERADO, TIME_SIZE);
    SIMULAR_SEGUNDOS(60 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);
}

/* Habilitar la alarma */
void test_habilitar_alarma(void) {
    TEST_ASSERT_FALSE(ClockGetAlarmaHabilitada(reloj));
    ClockToggleAlarma(reloj);
    TEST_ASSERT_TRUE(ClockGetAlarmaHabilitada(reloj));
}

/* Fijar la alarma, deshabilitarla y avanzar el reloj para no suene. */
void test_desactivar_alarma(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
    ClockSetAlarma(reloj, ESPERADO, TIME_SIZE);

    ClockToggleAlarma(reloj);
    SIMULAR_SEGUNDOS(60 * 60, ClockTick(reloj));
    TEST_ASSERT_FALSE(alarm_event_fired);
}

/* Hacer sonar la alarma y cancelarla hasta el otro dia.. */

void test_cancelar_alarma(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};

    ClockSetAlarma(reloj, ESPERADO, TIME_SIZE);
    SIMULAR_SEGUNDOS(60 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);

    SIMULAR_SEGUNDOS(1, ClockTick(reloj));
    CancelarAlarma();
    TEST_ASSERT_FALSE(alarm_event_fired);

    SIMULAR_SEGUNDOS(24 * 60 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);
}

/* Hacer sonar la alarma, posponerla y luego verificar que suene. Verificar que al dia siguiente
 * suene en la hora programada, no la postergada. */
void test_posponer_alarma(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};

    ClockSetAlarma(reloj, ESPERADO, TIME_SIZE);
    SIMULAR_SEGUNDOS(60 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);

    SIMULAR_SEGUNDOS(1, ClockTick(reloj));
    ClockPosponerAlarma(reloj, TIME_POST);
    CancelarAlarma();

    SIMULAR_SEGUNDOS(TIME_POST * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);
    ClockCancelarAlarma(reloj);

    SIMULAR_SEGUNDOS(23 * 55 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(alarm_event_fired);
    CancelarAlarma();
}

/*
BUSCAR: WFI_INTERRUPCCION
*/

/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/