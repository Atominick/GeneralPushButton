
#ifndef KEYBOARDS_H
#define KEYBOARDS_H

#include "button.h"

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


// TODO
/***************************************************************************************
template <int Discreteness>
class AnalogKeyboard
{
public:
    struct Config {
        volatile uint32_t *analogValuePtr;
        uint16_t *keyValues;
    };

public:
    AnalogKeyboard(const Config &config)
        : config(config) 
        {}

    void update() {

    }

public:
    const uint8_t keys_number = Discreteness;
    ButtonBase key[Discreteness];

protected:
    const Config &config;
    const uint16_t hysteresys;
};
*/

#endif // KEYBOARDS_H

