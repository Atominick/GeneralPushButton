
#include "button.h"


// uint8_t ButtonBase::isMod(Mod mod_to_check) {
//     return ((int)mods & (int)mod_to_check);
// }

// void ButtonBase::setMod(Mod mod_to_set) {
//     mods = (Mod) ((int)mod_to_set | (int)mods);
// }

// void ButtonBase::clearState() {
//     state = State::NO_STATE;
//     mods = Mod::NO_MODS;
// }


void ButtonBase::scanPushEvents() {
    if(pushed_time == PRESS_DELAY_IN_TICKS) {
        event_collection |= (uint16_t) Event::PRESSED;
    } else if(pushed_time == CLICK_DELAY_IN_TICKS) {
        event_collection |= (uint16_t) Event::CLICKED;
    } else if(pushed_time == TIMEOUT_DELAY_IN_TICKS){
        event_collection |= (uint16_t) Event::TIMEOUT;
    }

    if(pushed_time > CLAMPED_DELAY_IN_TICKS) {
        if(clamped_divider++ > CLAMPED_PERIOD_IN_TICKS) {
            event_collection |= (uint16_t) Event::CLAMPED;
            clamped_divider = 0;
        };
    }

/*
    if(pushed_time == CLICK_DELAY_IN_TICKS) {
        state = CLICKED;
        if(released_time > MULTIPLE_CLICK_DELAY_IN_TICKS)
            clicked_counter = 0;
    } else if(pushed_time == PRESS_DELAY_IN_TICKS) {
        state = PRESSED;
    } else if(pushed_time == LONG_PRESS_DELAY_IN_TICKS) {
        state = LONG_PRESSED;
    } else if(pushed_time > TIMEOUT_DELAY_IN_TICKS) {
        state = NO_STATE; // TimeOut
        setMod(TIMEOUT);
    }

    if(pushed_time > CLAMPED_DELAY_IN_TICKS) {
        clamped_counter++;
        setMod(CLAMPED);
    }
*/
}   

void ButtonBase::processRelease() {
    clamped_divider = 0;

    if(pushed_time > PRESS_DELAY_IN_TICKS) {
        event_collection |= (uint16_t) Event::PRESS_RELEASED;
    } else if(pushed_time > CLICK_DELAY_IN_TICKS) {
        event_collection |= (uint16_t) Event::CLICK_RELEASED;
    } else if(pushed_time > TIMEOUT_DELAY_IN_TICKS) {
        // Don`t do anything)
        // event_collection |= (uint16_t) Event::TIMEOUT;
    }

/*
    if(pushed_time > LONG_PRESS_DELAY_IN_TICKS) {
        state = LONG_PRESS_RELEASED;
    } else if(pushed_time > PRESS_DELAY_IN_TICKS) {
        state = PRESS_RELEASED;
    } else if(pushed_time > CLICK_DELAY_IN_TICKS) {
        state = CLICK_RELEASED;

        // Multiple clicking
        switch(++clicked_counter) {
        case 2:
            setMod(DOUBLE_CLICKED);
            break;
        case 3:
            setMod(TRIPLE_CLICKED);
            break;
        }
    } else {
        state = NO_STATE; // For values smaller then minimum
        setMod(TIMEOUT);
    }
    */
}

void ButtonBase::tick() {
    if(status == Status::PUSHED) {
        scanPushEvents();
        pushed_time++;

        if(released_time) {
            released_time = 0;
        }
    } else { // Released
        released_time++;

        if(pushed_time) {
            processRelease();
            pushed_time = 0;
        }
    }

/*
    if(status == Status::PUSHED) {
        if(!freezed) {
            pushed_time++;
            processPush();
        }
        // Ignore last value
        if(freezed) {
            reset();
        } else {
            released_time++;
            if(pushed_time) {
                processRelease();
                reset();
            }
        }
    }
*/
}

void ButtonBase::reset() {
    // clamped_counter = 0;
    released_time = 0;
    pushed_time = 0;
    freezed = 0;

    // clearState();
}


/****************************************************************************************/
Button::Button(volatile uint32_t *registerToCheck, const uint32_t registerMask, const uint8_t stateWhenOn)
    : register_to_check(registerToCheck), register_mask(registerMask), state_when_on(stateWhenOn) {}

void Button::tick() {
    if(!(*register_to_check & register_mask) ^ state_when_on) {
        setStatus(PUSHED);
    } else {
        setStatus(RELEASED);
    }

    ButtonBase::tick();
}
