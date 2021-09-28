
#ifndef KEYBOARDS_H
#define KEYBOARDS_H

#include <stdint.h>
#include "button.h"

#include <steam/debug.h>

/*
template <int XSize, int YSize>
class KeyboardBase
{
public:
    virtual void update();

public:
};
*/

template <int XSize, int YSize>
class MatrixKeyboard
{
public:
    struct Config {
        volatile uint32_t *inputIORegister;
        uint16_t *inputPinsMasks;
        volatile uint32_t *outputIORegister;
        uint16_t *outputPinsMasks;
    };

public:
    MatrixKeyboard(const Config &config)
        : config(config) {}

    // Can`t move to .cpp couse of C++ restrictions https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
    void update() {
        for(int i = 0; i < outputs_number; i++) {
            *config.outputIORegister |= config.outputPinsMasks[i];
            for(int j = 0; j < inputs_number; j++) {
                key[j][i].tick();

                if(*config.inputIORegister & config.inputPinsMasks[j]) {
                    key[j][i].setStatus(Button::Status::PUSHED);
                } else {
                    key[j][i].setStatus(Button::Status::RELEASED);
                }
            }
            *config.outputIORegister &= ~config.outputPinsMasks[i];
            // debug_printf(" ");
        }
    }

    void clearStates() {
        for(int i = 0; i < outputs_number; i++) {
            for(int j = 0; j < inputs_number; j++) {
                key[j][i].clearState();
            }
        }
    }

public:
    const uint8_t inputs_number  = XSize;
    const uint8_t outputs_number = YSize;
    ButtonBase key[XSize][YSize];

private:
    const Config &config;
};


/***************************************************************************************
class AnalogKeyboard
{
public:
    struct Config {
        uint8_t inputs_number;
        uint8_t outputs_number;

        volatile uint32_t *inputIORegister;
        uint16_t *inputPinsMasks;
        volatile uint32_t *outputIORegister;
        uint16_t *outputPinsMasks;
    };

public:
    AnalogKeyboard(const Config &config)
        : config(config) 
        {}

    virtual void initPins();
    void update();

public:
    // Every bit shows state of button in keyboard
    uint64_t states = 0;

protected:
    const Config &config;
};
*/

#endif // KEYBOARDS_H

