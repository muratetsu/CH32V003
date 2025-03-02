// CH32V003F4P6 - NP2402 LED Control
//
// January 2, 2025
// Tetsu Nishimura
//
//
// x 基板との接続はGND, 5V, SWDIOの3本
// x スイッチは押しっぱなしにするか，用意してあるスルーホールをジャンパーでつなぐ

#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "peripheral.h"

//#define NUM_LEDS    1
#define NUM_LEDS    4
//#define NUM_LEDS    6

// ADC value:
// Button pressed:     519~
// Button not pressed: 942~
#define BUTTON_PRESS_THRESHOLD   ((519 + 942) / 2)


typedef enum {
  FADEIN,
  FADEOUT,
  TOGGLE
} fadeMode_t;


void fadein(int num)
{
    for (int n = 0; n < 256; n++) {
        setPwm(num, n);
        Delay_Ms(8);
    }
}

void fadeout(int num)
{
    for (int n = 255; n >= 0; n--) {
        setPwm(num, n);
        Delay_Ms(12);
    }
}

void setRandomSeed(void)
{
    unsigned long seed = 0;

    for (int n = 0; n < 4; n++) {
        seed <<= 8;
        seed |= Get_ADC_Val(ADC_Channel_0) & 0xff;
        // Wait a while to get a different value from previous one
        Delay_Ms(10);
    }

    srand(seed);
}

bool selectRandom(fadeMode_t mode)
{
    static int state = 0;
    bool ret = false;

    int num = rand() % NUM_LEDS;
    int bitMask = 1 << num;

    switch (mode) {
    case FADEIN:
        if ((state & bitMask) == 0) {
            fadein(num);
            state ^= bitMask;
            // If all bits are set
            if (state >= (1 << NUM_LEDS) - 1) {
                ret = true;
            }
        }
        break;

    case FADEOUT:
        if ((state & bitMask) != 0) {
            fadeout(num);
            state ^= bitMask;
            // If all bits are cleared
            if (state == 0) {
                ret = true;
            }
        }
        break;

    case TOGGLE:
        if ((state & bitMask) == 0) {
            fadein(num);
            state ^= bitMask;
        }
        else {
            // Avoid all LEDs to be cleared
            if ((state ^ bitMask) != 0) {
                fadeout(num);
                state ^= bitMask;
            }
        }
        break;
    }

    return ret;
}

// Button state can not be achieve in NP2402
bool buttonPressed(void)
{
    if (Get_ADC_Val(ADC_Channel_0) < BUTTON_PRESS_THRESHOLD){
        return true;
    }
    else {
        return false;
    }
}

int main(void)
{
    systemInit();
    powerEnbale(true);
    Delay_Ms(1000);
    setRandomSeed();

    // Button state can not be achieve in NP2402
    bool btnPressed = false;

    // Fade in LEDs
    while (selectRandom(FADEIN) == false) {
        Delay_Ms(500);
    }

    if (btnPressed) {
        // Toggle LEDs (50 x 6s(avg) = 5min(appx))
        for (int n = 0; n < 50; n++) {
            selectRandom(TOGGLE);
            uint32_t delay = rand() % 12000;    // random delay from 0 to 12s
            Delay_Ms(delay);
        }
    }
    else {
        Delay_Ms(2000);
    }

    // Fade out LEDs
    while (selectRandom(FADEOUT) == false) {
        Delay_Ms(500);
    }

    Delay_Ms(1000);
    powerEnbale(false);

    while(1) {
    }
}
