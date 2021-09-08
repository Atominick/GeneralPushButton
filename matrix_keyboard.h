
#ifndef MATRIX_KEYBOARD_H
#define MATRIX_KEYBOARD_H

#include <stdint.h>

class MatrixKeyboard
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
    MatrixKeyboard(const Config &config)
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

#endif // MATRIX_KEYBOARD_H

