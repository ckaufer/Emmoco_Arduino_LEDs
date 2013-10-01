#include "Blinker.h"
#include "Hal.h"

static void tickHandler(void);

static Blinker_cmd_t cmdVal = Blinker_START_CMD;
static Blinker_count_t countVal = 0;
static Blinker_delay_t delayVal = 1.2 * Blinker_delay_scale;

#define FOREVER -1

static Blinker_count_t curCount = FOREVER;
static Blinker_delay_t curTime = 0;

void main() {
    Hal_init();
    Hal_tickStart(Blinker_delay_step, tickHandler);
    Blinker_start();
    Hal_idleLoop();
}

static void tickHandler(void) {

    if (cmdVal == Blinker_STOP_CMD) {
        return;
    }
    
    if (curTime < delayVal) {
        curTime += Blinker_delay_step;
        return;
    }

    if (curCount == FOREVER || curCount-- > 0) {
        Hal_ledToggle();
    }
    else {
        cmdVal = Blinker_STOP_CMD;
        Hal_ledOff();
    }
    curTime = 0;
    Blinker_ledState_indicate();
}

/* -------- SCHEMA CALLBACKS -------- */

void Blinker_connectHandler(void) {
    Hal_connected();
}

void Blinker_disconnectHandler(void) {
    Hal_disconnected();
}

void Blinker_cmd_store(Blinker_cmd_t* input) {
    cmdVal = *input;
    switch (cmdVal) {
        case Blinker_START_CMD:
            curCount = countVal > 0 ? countVal * 2 : FOREVER;
            curTime = 0;
            break;
        case Blinker_STOP_CMD:
            Hal_ledOff();
            break;
    }
}

void Blinker_count_fetch(Blinker_count_t* output) {
    *output = countVal;
}

void Blinker_count_store(Blinker_count_t* input) {
    countVal = *input;
}

void Blinker_delay_fetch(Blinker_delay_t* output) {
    *output = delayVal;
}

void Blinker_delay_store(Blinker_delay_t* input) {
    delayVal = *input;
}

void Blinker_ledState_fetch(Blinker_ledState_t* output) {
    *output = Hal_ledRead() ? Blinker_LED_ON : Blinker_LED_OFF;
}

