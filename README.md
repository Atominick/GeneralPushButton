# GeneralPushButton
This lib will help to <span style="color:#8ae234">**process**</span> push buttons. It supports <span style="color:#8ae234">**almost every type of pressing**</span> and several types of keyboards.

![Animation with demonstration of work](https://gist.githubusercontent.com/Atominick/dfa3238cec5a6b6e55644635edfbaa78/raw/89d6af2e5e921c44c0fec06665a4ffb03f554c9e/GeneralPushButtonDemonstration.gif)

*******
## Features
- time interval settings in miliseconds
- simple integration to any platform
- flexible keyboards configuration
- 3 different types of pressing
- optimized resources using 
- no dependecies needed

## Where to use?
Every system where push buttons or their functional analogs are used and different press types should be supported. The library was developed <span style="color:#8ae234">**for embedded systems**</span>.

## How to use?
### General instructions:
1. Add header (*unexpectedly*)
    ```cpp
   #include "button.h"
   // or 
   #include "keyboards.h"
    ```
1. Create objects of needed class.
    ```cpp
    // Simple button example
    Button button(&(GPIOx->InputDataRegister), GPIO_IDR_ID0);
    
    // Keyboard example
    uint16_t buttonsInputPins[] = { PIN_0, PIN_1 };
    uint16_t buttonsOutputPins[] = { PIN_2, PIN_3 };
    static const MatrixKeyboard<2, 2>::Config buttonsConfig = {
        .inputIORegister = &(GPIOx->InputDataRegister),
        .inputPinsMasks = buttonsInputPins, 
        .outputIORegister = &(GPIOx->OutputDataRegister),
        .outputPinsMasks = buttonsOutputPins
    };
    MatrixKeyboard<2, 2> buttons(buttonsConfig);
    ```

1. Periodically call method to update buttons state.
    ```cpp
    tick(); // or update() for keyboards
    ```

1. Define the frequency of periodical update function.
    ```cpp
    #define BUTTON_TICKING_FREQUENCY
    ```

1. Process every button state in the way you want. As a rule in main loop to minimize delay in periodic interrrupt. For example:
    ```cpp
    inline void handle_button() {
        /* State can be changed in most unsuitable moment.
         This condition disable the chance of wiping state without processing */
        if(button.isMod(Button::Mod::ANY) || button.state) {
            if(button.isMod(Button::Mod::CLAMPED)) {
                // Clamping
            }
            
            switch(button.state) {
                case Button::State::CLICK_RELEASED:
                    // Process simple click and/or double and triple.
                    if(button.isMod(Button::Mod::TRIPLE_CLICKED)) {

                    } else if(button.isMod(Button::Mod::DOUBLE_CLICKED) {

                    }
                break;

                /* Other available states
                case Button::State::PRESSED ...
                                    LONG_PRESSED,
                                    PRESS_RELEASED,
                                    LONG_PRESS_RELEASED,
                break;
                */
                
                default:
                break;
            }
            
            /* Don`t forget to clear states!
            Otherwise buttons will be processing forever. */
            button.clearState();
        }
    }
    ```

### Other
- Time intervals can be redefined in your program (example: [`#define CLICK_DELAY 60`](https://github.com/Atominick/GeneralPushButton/blob/72abbfbeb159f3e31549ccf65802fc4856633518/button.h#L13) )
- Button polarity inversing is available as third param in `Button` constructor.
- If your button processing programm lead to changing the way of processing, you can use `Button::freeze();` to ignore any next state before releasing.

---
### Feel free to contact me with any questions.
