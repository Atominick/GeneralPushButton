#include "matrix_keyboard.h"

/************************************************************************************/
// void MatrixKeyboard::init() {
//     uint16_t output_pins_mask = 0;
//     uint16_t input_pins_mask = 0;

//     for(int i = 0; i < config.outputs; i++) {
//         output_pins_mask |= config.outputPins[i];
//     }
//     gpio::configureOutput(config.outputIORegister, output_pins_mask, gpio::SpeedHigh);
//     // gpio::configurePullUp(config.outputGPIO, output_pins_mask);

//     for(int i = 0; i < config.inputs; i++) {
//         input_pins_mask |= config.inputPins[i];
//     }
//     gpio::configurePullDown(config.inputGPIO, input_pins_mask);
// } 

void MatrixKeyboard::update() {
    uint8_t button_no = 0;
    states = 0;
    
    for(int i = 0; i < config.outputs_number; i++) {
        // gpio::set(config.outputIORegister, config.inputPinsMasks[i]);
        *config.outputIORegister |= config.inputPinsMasks[i];


        for(int j = 0; j < config.inputs_number; j++) {
            states &= ~((uint64_t)1 << button_no);
            // uint8_t button_bool = gpio::get(config.inputIORegister, config.inputPinsMasks[j]) ? 1 : 0;
            uint8_t button_bool = !!(*config.inputIORegister & config.inputPinsMasks[j]);
            states |= ((uint64_t)button_bool << button_no);
            button_no++;
        }
        // gpio::reset(config.outputIORegister, config.inputPinsMasks[i]);
        *config.outputIORegister &= ~config.inputPinsMasks[i];

    }
}  