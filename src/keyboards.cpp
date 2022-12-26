#include "keyboards.h"


namespace general_button
{

    KeyboardEvent Keyboard::getEvent() {
        for(int i = 0; i < keys_count; i++) {
            Event new_e = key[i].getEvent();
            if(new_e) {
                return KeyboardEvent(i, new_e);
            }
        }

        return KeyboardEvent(0, Event::NO_EVENT);
    }

    void Keyboard::update() {
        for(int i = 0; i < keys_count; i++) {
            key[i].tick();
        }
    }

} // namespace general_button
