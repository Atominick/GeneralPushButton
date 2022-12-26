# GeneralPushButton

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/atominick/library/GeneralPushButton.svg)](https://registry.platformio.org/libraries/atominick/GeneralPushButton)

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
__(See examples)__

### Getting Started
After installing this library in your project (use namespace `general_button`)

```cpp
#include "keyboards.h"

using namespace general_button;
```

### Initialize a `Button` object
```cpp
Button button0(&(GPIOA->IDR), PIN_1, false);
```

### Process events
```cpp
while(1) {
    Event e = button0.getEvent();
    if(e) {
        if(e == CLICKED) {
            // "ok" click action
        }
    }

    // ...
}
```

### Don't forget to `tick()`!
In order for `Button` to work correctly, you must call `tick()` on __each button instance__ or keyboard. If you're not getting any button events, this is probably why.

```cpp
extern "C" void SysTick_Handler() {
    button0.tick();

    // Do other things...
}
```

### Other
- Set your own ticking frequence `#define BUTTON_TICKING_FREQUENCY TickRate` for correct timing
- Time intervals can be redefined in your program: `#define CLICK_DELAY xx`
- Button polarity inversing is available as third param in `Button` constructor


---
### The lib is developing depending on my personal needs or user`s requests. Feel free to contact me with any questions and suggestions.
