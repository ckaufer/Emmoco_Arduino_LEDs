
#include "Hal.h"
static Blinker_cmd_t cmdVal = Blinker_START_CMD;
static Blinker_count_t countVal = 13;
static Blinker_delay_t delayVal = 1.2 * Blinker_delay_scale;

#define FOREVER -1
//uint8_t ledArray[12]={0,0,0,0,0,5,6,7,8,9,10,11,12};
static Blinker_count_t curCount = FOREVER;
static Blinker_delay_t curTime = 0;
void loop(void)
{
    Hal_User_ledOn(countVal);
}