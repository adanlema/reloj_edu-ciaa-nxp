/*==================[inclusions]=============================================*/
#include <string.h>
#include "reloj.h"
/*==================[macros and definitions]=================================*/

struct alarma_s {
    uint8_t  time[TIME_SIZE];
    uint8_t  time_pos[TIME_SIZE];
    bool     estado;
    bool     postergada;
    evento_t toggle;
};

struct clock_s {
    uint8_t   time[TIME_SIZE];
    uint32_t  ticks;
    uint32_t  ticks_por_seg;
    bool      valida;
    alarma_pt alarma;
};

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
static bool ClockIncrement_seg(clock_t reloj);
static void ClockIncrement_day(clock_t reloj);
static void ClockIncrement(clock_t reloj, uint8_t indice, uint8_t valor);
/*==================[internal data definition]===============================*/
static struct clock_s  self[1];
static struct alarma_s al_reloj[1];
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static bool ClockIncrement_seg(clock_t reloj) {
    bool estado = false;
    if (reloj->ticks == reloj->ticks_por_seg) {
        reloj->ticks = 0;
        reloj->time[UNIDAD_SEG]++;
        estado = true;
    }
    return estado;
}

static void ClockIncrement_day(clock_t reloj) {
    if ((reloj->time[DECENA_HOR] == 2) && (reloj->time[UNIDAD_HOR] == 4)) {
        reloj->time[DECENA_HOR] = 0;
        reloj->time[UNIDAD_HOR] = 0;
    }
}

static void ClockIncrement(clock_t reloj, uint8_t indice, uint8_t valor) {
    if (reloj->time[indice] >= valor) {
        reloj->time[indice] = 0;
        reloj->time[indice - 1]++;
    }
}

/*==================[external functions definition]==========================*/
clock_t ClockCreate(int tics_por_seg, evento_t toggleAlarma) {
    memset(self, 0, sizeof(self));
    memset(al_reloj, 0, sizeof(al_reloj));

    self->ticks_por_seg  = tics_por_seg;
    self->alarma         = al_reloj;
    self->alarma->toggle = toggleAlarma;
    return self;
}
bool ClockGetTime(clock_t reloj, uint8_t * hora, int size) {
    memcpy(hora, reloj->time, size);
    return reloj->valida;
}

bool ClockSetTime(clock_t reloj, const uint8_t * hora, int size) {
    memcpy(reloj->time, hora, size);
    reloj->valida = true;
    return true;
}

bool ClockTick(clock_t reloj) {
    bool estado;
    reloj->ticks++;
    estado = ClockIncrement_seg(reloj);
    ClockIncrement(reloj, UNIDAD_SEG, UNIDAD_TIME); // INCREMENTAR_DECENAS_SEG
    ClockIncrement(reloj, DECENA_SEG, DECENA_TIME); // INCREMENTAR_MINUTOS_UNIDAD
    ClockIncrement(reloj, UNIDAD_MIN, UNIDAD_TIME); // INCREMENTAR_MINUTOS_DECENAS
    ClockIncrement(reloj, DECENA_MIN, DECENA_TIME); // INCREMENTAR_HORAS_UNIDAD
    ClockIncrement(reloj, UNIDAD_HOR, UNIDAD_TIME); // INCREMENTAR_HORAS_DECENAS
    ClockIncrement_day(reloj);

    if (reloj->alarma->estado) {
        if ((memcmp(reloj->alarma->time, reloj->time, TIME_SIZE)) == 0) {
            reloj->alarma->toggle(true);
        }
    }
    return estado;
}

void ClockSetAlarma(clock_t reloj, const uint8_t * hora, int size) {
    memcpy(reloj->alarma->time, hora, size);
    memcpy(reloj->alarma->time_pos, hora, size);
    reloj->alarma->estado = true;
}
bool ClockGetAlarma(clock_t reloj, uint8_t * hora, int size) {
    if (reloj->alarma->estado) {
        memcpy(hora, reloj->alarma->time, size);
    }
    return (reloj->alarma->estado);
}

bool ClockGetAlarmaHabilitada(clock_t reloj) {
    return (reloj->alarma->estado);
}

void ClockToggleAlarma(clock_t reloj) {
    reloj->alarma->estado ^= true;
}

void ClockCancelarAlarma(clock_t reloj) {
    reloj->alarma->toggle(false);
    if (reloj->alarma->postergada) {
        memcpy(reloj->alarma->time, reloj->alarma->time_pos, TIME_SIZE);
        reloj->alarma->postergada = false;
    }
}

void ClockPosponerAlarma(clock_t reloj, uint8_t time_post) {
    if ((reloj->alarma->postergada) == false) {
        reloj->alarma->toggle(false);
        memcpy(reloj->alarma->time_pos, reloj->alarma->time, TIME_SIZE);
        reloj->alarma->postergada = true;
        reloj->alarma->time[UNIDAD_MIN] += time_post;
        CONTROLAR_REBALSE_MIN(reloj->alarma->time[DECENA_MIN], reloj->alarma->time[UNIDAD_MIN],
                              reloj->alarma->time[UNIDAD_HOR]);
        CONTROLAR_REBALSE_HOR(reloj->alarma->time[DECENA_HOR], reloj->alarma->time[UNIDAD_HOR]);
        if ((memcmp(reloj->time, reloj->alarma->time, 4)) > 0) {
            ClockCancelarAlarma(reloj);
        }
    } else {
        ClockCancelarAlarma(reloj);
    }
}
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/** @ doxygen end group definition */
/*==================[end of file]============================================*/