#include "button.h"

#include <steam/debug.h>


uint8_t ButtonBase::isMod(Mod mod_to_check) {
    return ((int)mods & (int)mod_to_check);
}

void ButtonBase::setMod(Mod mod_to_set) {
    mods = (Mod) ((int)mod_to_set | (int)mods);
}

void ButtonBase::clearState() {
    state = State::NO_STATE;
    mods = Mod::NO_MODS;
}

void ButtonBase::reset() {
    clamped_counter = 0;
    released_time = 0;
    pushed_time = 0;
    freezed = 0;
    // clearState();
}

void ButtonBase::processPush() {
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
}

void ButtonBase::processRelease() {
    state = UNCERTAIN; // For values smaller then minimum

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
    }
    // } else {
    //     clicked_counter = 0;
    // }
}

void ButtonBase::tick() {
    if(status == Status::PUSHED) {
        // debug_printf("Pushed\n");

        if(!freezed) {
            pushed_time++;
            processPush();
        }
    } else { // Released
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
