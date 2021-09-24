
#ifndef KEYBOARS_H
#define KEYBOARS_H

#include <stdint.h>
#include "button.h"


template<int XSize, int YSize>
class KeyboardBase
{
public:
    virtual void update();

public:
    ButtonBase key[XSize][YSize];
};


/****************************************************************************************/
template<int XSize, int YSize>
class MatrixKeyboard : public Keyboard<XSize, YSize>
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

    virtual void initPins();
    void update() override;

private:
    const Config &config;
    const uint8_t outputs_number = XSize;
    const uint8_t inputs_number = YSize;

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

#endif // KEYBOARS_H

