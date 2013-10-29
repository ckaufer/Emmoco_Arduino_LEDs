#include "Blinker.h"
#include "Hal.h"
#include "avr/interrupt.h"
#include "avr/io.h"
static void tickHandler(void);

static Blinker_cmd_t cmdVal = Blinker_START_CMD;
static Blinker_count_t countVal = 0;
static Blinker_delay_t delayVal = (1.2 * Blinker_delay_scale)/10; //=1000, delayVal = 1200

#define TICK_RATE  (Blinker_delay_scale/10)

#define FOREVER -1
static Blinker_delay_t curTime = 0;

#define MIN_LED_NUM 3
#define MAX_LED_NUM 15
#define LED_NONE 0
#define LED_ALL 0xFFFF
#define LED3 (1 << 3)
#define LED4 (1 << 4)
#define LED5 (1 << 5)
#define LED6 (1 << 6)
#define LED7 (1 << 7)
#define LED8 (1 << 0)
#define LED9 (1 << 1)
#define LED10 ( 1 << 10 )
#define LED11 (1 << 11)
#define LED12 (1 << 12)
#define LED13 (1 << 13)
#define LED14 (1 << 14)
#define LED15 (1 << 15)

#define NUM_PATTERNS 3
#define PATTERN_LEN 3


const uint16_t ledArray1[PATTERN_LEN]={LED3|LED6, LED4|LED7, LED5|LED7|LED8};
static uint16_t ledArray2[PATTERN_LEN]={LED8, LED10, LED8|LED10};
static uint16_t ledArray3[PATTERN_LEN]={LED_ALL, LED_NONE, LED3};

static uint8_t curLedIdx;

typedef uint8_t* Pattern;
Pattern patterns[NUM_PATTERNS] = {ledArray1, ledArray2, ledArray3};
static Pattern curPattern = 0;

void main() {
    Hal_init();
    Hal_tickStart(TICK_RATE, tickHandler);
    Blinker_start();
    Hal_idleLoop();
    
}
static void tickHandler(void) {

    if (cmdVal == Blinker_STOP_CMD) {
        return;
    }
    
    if (curTime < delayVal) {
        curTime += TICK_RATE;
        return;
    }

    if (curPattern == 0) {
        Hal_ledToggle();
    }
    else {
        uint16_t mask = curPattern[curLedIdx++];
        if (curLedIdx == PATTERN_LEN) {
            curLedIdx = 0;
        }
        uint16_t i;
        for (i = MIN_LED_NUM; i <= MAX_LED_NUM; i++) {
            if (mask & (1 << i)) {
                Hal_User_ledOn(i);
            }
            else {
                Hal_User_ledOff(i);
            }
        }
    }
    
  //Hal_ledToggle();
         
   
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
    uint8_t i;
    switch (cmdVal) {
        case Blinker_START_CMD:
            curTime = 0;
            countVal = 0;
            curLedIdx = 0;
            break;
        case Blinker_STOP_CMD:
            for (i = MIN_LED_NUM; i <= MAX_LED_NUM; i++) {
                Hal_User_ledOff(i);
            }
            break;
    }
}

void Blinker_count_fetch(Blinker_count_t* output) {
    *output = countVal;
}

void Blinker_count_store(Blinker_count_t* input) {
    countVal = *input;
    curPattern = countVal == 0 ? 0 : patterns[(countVal - 1) % NUM_PATTERNS];
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
