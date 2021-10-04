# GeneralPushButton
This lib will help to process push buttons. It supports almost every type of pressing and several types of keyboards.

## Features
- simple integration to any platform
- time interval settings in miliseconds
- 3 different types of pressing
- flexible keyboards setting
- optimized resources using 
- no dependecies

## Where to use?
Every system where push buttons or their functional analogs are used and different press types should be supported. The library was developed for embedded systems.

## How to use?
### General instructions:
1. Create objects of needed class.
        
        // Simple button example
        Button button(&(GPIOA->IDR), GPIO_IDR_ID0);
        
        // Keyboard example
        uint16_t buttonsInputPins[] = {PIN_3, PIN_4, PIN_5};
        uint16_t buttonsOutputPins[] = {PIN_0, PIN_1, PIN_2, PIN_3};
        static const MatrixKeyboard<3, 4>::Config buttonsConfig = {
            .inputIORegister = &(GPIOB->IDR),
            .inputPinsMasks = buttonsInputPins, 
            .outputIORegister = &(GPIOA->ODR),
            .outputPinsMasks = buttonsOutputPins
        };
        MatrixKeyboard<3, 4> buttons(buttonsConfig);


1. Periodically call method to update buttons state.

        tick(); // or update() for keyboards

1. Define the frequency of periodical update function.

        #define BUTTON_TICKING_FREQUENCY

1. Process every button state in the way you want. As a rule in main loop to minimize delay in periodic interrrupt. For example:

        inline void handle_button() {
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

### Other
- Time intervals can be redefined in your program
- Button polarity inversing is available as third param in constructor.
- If your button processing programm lead to changing the way of processing, you can use `Button::freeze();` to ignore any next state before releasing.

---
### Feel free to contact me with any questions)
