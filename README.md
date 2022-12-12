# GeneralPushButton
This lib will help to <span style="color:#8ae234">**process**</span> push buttons. It supports <span style="color:#8ae234">**multiple types of pressing**</span> and several types of keyboards.

![Animation with demonstration of work](https://gist.githubusercontent.com/Atominick/dfa3238cec5a6b6e55644635edfbaa78/raw/89d6af2e5e921c44c0fec06665a4ffb03f554c9e/GeneralPushButtonDemonstration.gif)

*******
## Features:
- 3 different types of pressing and releasing
- time interval settings in miliseconds
- simple integration to any platform
- flexible keyboards configuration
- optimized resources using
- no dependecies needed

## Where to use?
Any system where push buttons or their functional analogs are used and different press types should be supported. The library was developed <span style="color:#8ae234">**for embedded systems**</span>.

## How to use?

### General example (STM32):

```cpp
#include <stm32f0xx.h>
#include "keyboards.h"

#define TickRate 1000
#define BUTTON_TICKING_FREQUENCY TickRate

using namespace button_lib;


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
    buttons.update();
}
```

### Other
- Time intervals can be redefined in your program (example: [`#define CLICK_DELAY 60`](https://github.com/Atominick/GeneralPushButton/blob/72abbfbeb159f3e31549ccf65802fc4856633518/button.h#L13) )
- Button polarity inversing is available as third param in `Button` constructor.

---
### The lib is developing depending on my personal needs or user`s requests. Feel free to contact me with any questions and suggestions.
