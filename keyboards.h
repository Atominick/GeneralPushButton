
#pragma once

#include "button.h"


namespace button_lib {

struct KeyboardEvent {
    KeyboardEvent(int key_index, Event e) : index(key_index), event(e){};

    int index;
    Event event;
};


/****************************************************************************/
class Keyboard
{
public:
    Keyboard(Button *keys, const uint16_t keys_count) 
        : keys_count(keys_count), key(keys) {}


    KeyboardEvent getEvent();
    void update();

private:
    const uint16_t keys_count;

public:
    Button *key;
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
                Event new_e = key[i][j].getEvent();
                if(new_e) {
                    return KeyboardEvent(((i + 1) * (j + 1)), new_e);
                }
            }
        }

        return KeyboardEvent(0, Event::NO_EVENT);
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

public:
    const uint8_t inputs_number = XSize;
    const uint8_t outputs_number = YSize;
    ButtonBase key[XSize][YSize];

private:
    const Config &config;
};

} // namespace button_lib
