#include <stm32f0xx.h>

#include "keyboards.h"

#define TickRate 1000
#define BUTTON_TICKING_FREQUENCY TickRate

using namespace general_button;


SysTick_Config(F_CPU / TickRate);

enum ButtonsNames {
    LEFT_BTN  = 0,
    OK_BTN    = 1,
    RIGHT_BTN = 2,
};

Button buttons_array[] = {
    Button(&(GPIOA->IDR), PIN_1),
    Button(&(GPIOA->IDR), PIN_2),
    Button(&(GPIOB->IDR), PIN_1, 0),
};
Keyboard buttons(buttons_array, sizeof(buttons_array) / sizeof(Button));

while(1) {
    // Other user code

    // Events processing
    KeyboardEvent ke = buttons.getEvent();
    if(ke.event) {
        if(ke.index == LEFT_BTN) {
            if(ke.event == CLICKED or ke.event == CLAMPED) {
                // "minus" user action
            }
        } else if(ke.index == OK_BTN) {
            if(ke.event == CLICKED) {
                // "ok" user action
            } else if(ke.event == LONG_PRESS_RELEASED) {
                // "menu" user action
            }
        } else if(ke.index == RIGHT_BTN) {
            if(ke.event == CLICKED or ke.event == CLAMPED) {
                // "plus" user action
            }
        }
    }
}


extern "C" void SysTick_Handler() {
    buttons.tick();
}