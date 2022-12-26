#include "button.h"


namespace general_button
{

    /**
     * Executed only once when Status changed to PUSHED
     **/
    void ButtonBase::processPush() {
        if(released_time > MULTIPLE_CLICK_DELAY_IN_TICKS)
            clicked_counter = 0;
    }

    /**
     * Executed every tick while Status remain PUSHED
     **/
    void ButtonBase::scanPushEvents() {
        // Sorted by frequency of triggering
        if(pushed_time == CLICK_DELAY_IN_TICKS) {
            // Multiple clicking
            clicked_counter++;
            if(clicked_counter == 2)
                event_collection |= (uint16_t)Event::DOUBLE_CLICKED;
            else if(clicked_counter > 2)
                event_collection |= (uint16_t)Event::MULTIPLE_CLICKED;

            event_collection |= (uint16_t)Event::CLICKED;
        } else if(pushed_time == PRESS_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::PRESSED;
        } else if(pushed_time == LONG_PRESS_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::LONG_PRESSED;
        } else if(pushed_time == TIMEOUT_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::TIMEOUT;
        }

        if(pushed_time > CLAMPED_DELAY_IN_TICKS) {
            if(clamped_divider++ > CLAMPED_PERIOD_IN_TICKS) {
                event_collection |= (uint16_t)Event::CLAMPED;
                clamped_divider = 0;
            }
        }
    }


    /**
     * Executed every tick while Status remain RELEASED
     **/
    void ButtonBase::scanReleaseEvents() {}

    /**
     * Executed only once when Status changed to RELEASED
     **/
    void ButtonBase::processRelease() {
        clamped_divider = 0;

        if(pushed_time > TIMEOUT_DELAY_IN_TICKS) {
            // Don`t do anything :)
        } else if(pushed_time > LONG_PRESS_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::LONG_PRESS_RELEASED;
        } else if(pushed_time > PRESS_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::PRESS_RELEASED;
        } else if(pushed_time > CLICK_DELAY_IN_TICKS) {
            event_collection |= (uint16_t)Event::CLICK_RELEASED;
        }
    }

    void ButtonBase::setStatus(Status status) {
        if(this->status != status) {
            if(status == Status::PUSHED) {
                processPush();
                released_time = 0;
            } else {
                processRelease();
                pushed_time = 0;
            }

            this->status = status;
        }
    }

    void ButtonBase::tick() {
        if(status == Status::PUSHED) {
            scanPushEvents();
            pushed_time++;
        } else { // Released
            scanReleaseEvents();
            released_time++;
        }
    }

    /**
     * Should be called relatively frequently cause return only one event per time
     **/
    Event ButtonBase::getEvent() {
        for(int i = 0; i < 16; i++) {
            uint16_t new_e = event_collection & (1 << i);

            if(new_e) {
                event_collection &= ~new_e;
                return (Event)new_e;
            }
        }

        return Event::NO_EVENT;
    }

    void ButtonBase::reset() {
        released_time = 0;
        pushed_time = 0;
    }


    /****************************************************************************************/
    void Button::tick() {
        if(!(*register_ptr & register_mask) ^ clicked_state) {
            setStatus(PUSHED);
        } else {
            setStatus(RELEASED);
        }

        ButtonBase::tick();
    }

} // namespace general_button
