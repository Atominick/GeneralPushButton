#include "button.h"


Button::Button(volatile uint32_t *registerToCheck, const uint32_t registerMask, const uint8_t stateWhenOn)
    : register_to_check(registerToCheck), register_mask(registerMask), state_when_on(stateWhenOn) {}

void Button::tick() {
    if(!(*register_to_check & register_mask) ^ state_when_on) { // Pushed
        if(!freezed) {
            pressed_time++;

            if(pressed_time == CLICK_DELAY_IN_TICKS) {
                state = CLICKED;
                if(released_time > MULTIPLE_CLICK_DELAY_IN_TICKS) {
                    clicked_counter = 0;
                }
            } else if(pressed_time == PRESS_DELAY_IN_TICKS) {
                state = PRESSED;
            } else if(pressed_time == LONG_PRESS_DELAY_IN_TICKS) {
                state = LONG_PRESSED;
            } else if(pressed_time == TIMEOUT_DELAY_IN_TICKS) {
                mods = (Mod)((int)TIMING_ERROR | (int)mods);
            }

            if(pressed_time > CLAMPED_DELAY) {
                mods = (Mod)((int)CLAMPED | (int)mods);
                clamped_counter++;
            }
        }
    } else { // Released
        released_time++;

        if(freezed) {
            reset();
            freezed = 0;
        }

        if(pressed_time) {            
            if(pressed_time > LONG_PRESS_DELAY_IN_TICKS 
                    && pressed_time < TIMEOUT_DELAY_IN_TICKS) {
                state = LONG_PRESS_RELEASED;
            } else if(pressed_time > PRESS_DELAY_IN_TICKS) {
                state = PRESS_RELEASED;
            } else if(pressed_time > CLICK_DELAY_IN_TICKS) {
                state = CLICK_RELEASED;
            } else {
                // For values beyond borders
                mods = (Mod)((int)TIMING_ERROR | (int)mods);
            }

            // Multiple clicking
            if(state == CLICK_RELEASED) {
                // debug_printf("CC: %d\n", clicked_counter);
                switch(++clicked_counter) {
                case 2:
                    mods = (Mod)((int)DOUBLE_CLICKED | (int)mods);
                    break;
                case 3:
                    mods = (Mod)((int)TRIPLE_CLICKED | (int)mods);
                    break;
                }

            } else {
                clicked_counter = 0;
            }

            clamped_counter = 0;
            released_time = 0;
            pressed_time = 0;
        }
    }
}

uint8_t Button::is_mod(Mod mod_to_check) {
    if((int)mods & (int)mod_to_check)
        return 1;
    else
        return 0;
}

void Button::wipe_status() {
    state = State::NO_STATE;
    mods = Mod::NO_MODS;
}

void Button::reset() {
    pressed_time = 0;
    released_time = 0;
    clamped_counter = 0;
    wipe_status();
}

void Button::freeze() {
    freezed = 1;
}
