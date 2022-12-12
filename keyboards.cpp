#include "keyboards.h"


namespace button_lib {

KeyboardEvent Keyboard::getEvent() {
    for(int i = 0; i < keys_count; i++) {
        Event new_e = key[i].getEvent();
        if(new_e) {
            return KeyboardEvent((i + 1), new_e);
        }
    }

    return KeyboardEvent(0, Event::NO_EVENT);
}

void Keyboard::update() {
    for(int i = 0; i < keys_count; i++) {
        key[i].tick();
    }
}

} // namespace button_lib
