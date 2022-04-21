
#ifndef KEYBOARDS_H
#define KEYBOARDS_H

#include "button.h"


struct KeyboardEvent {
    KeyboardEvent(int key_index, Button::Event e): index(key_index), event(e) {};

    int index;
    Button::Event event;
};


/****************************************************************************/
template <int Size>
class Keyboard
{
public:
    // Keyboard();

private:
    ButtonBase key[Size];
};


/****************************************************************************/
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

    KeyboardEvent getEvent() {
        for(int i = 0; i < XSize; i++) {
            for(int j = 0; j < YSize; j++) {
                Button::Event new_e = key[i][j].getEvent();
                if(new_e) {
                    return KeyboardEvent((i*j), new_e);
                }
            }
        }

        return KeyboardEvent(0, Button::Event::NO_EVENT);
    }

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

#endif // KEYBOARDS_H

