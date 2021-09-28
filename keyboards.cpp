#include "keyboards.h"



/***********************************************************************************
template <int XSize, int YSize>
void MatrixKeyboard<XSize, YSize>::update() {
    uint8_t button_no = 0;    
    for(int i = 0; i < XSize; i++) {
        *config.outputIORegister |= config.inputPinsMasks[i];
        for(int j = 0; j < YSize; j++) {
            if(*config.inputIORegister & config.inputPinsMasks[j])
                key[i][j].setStatus(Button::Status::PUSHED);
            else
                key[i][j].setStatus(Button::Status::RELEASED);

            button_no++;
        }
        *config.outputIORegister &= ~config.inputPinsMasks[i];
    }
}
*/

// template class MatrixKeyboard<3, 4>;

/*
void AnalogKeyboard::update() {
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
*/ 