#pragma once

#include <stdint.h>


/*
This is standart timing settings
They can be trimmed with your own defines

DO NOT FORGET to set your own period of tick() function calling
    with BUTTON_TICKING_FREQUENCY define
*/

#ifndef BUTTON_TICKING_FREQUENCY
#define BUTTON_TICKING_FREQUENCY 1000 // in Hz
#endif

// Delays in ms
#ifndef CLICK_DELAY
#define CLICK_DELAY 15 // Must exceed debounce time
#endif

#ifndef MULTIPLE_CLICK_DELAY
#define MULTIPLE_CLICK_DELAY 300
#endif

#ifndef PRESS_DELAY
#define PRESS_DELAY 450
#endif

#ifndef CLAMPED_DELAY
#define CLAMPED_DELAY 1500
#endif

#ifndef CLAMPED_PERIOD
#define CLAMPED_PERIOD 50
#endif

#ifndef LONG_PRESS_DELAY
#define LONG_PRESS_DELAY 1500
#endif

#ifndef TIMEOUT_DELAY
#define TIMEOUT_DELAY 4500
#endif

#define MS_TO_TICKS(ms) (int((float)ms / (1000. / BUTTON_TICKING_FREQUENCY)))

#define CLICK_DELAY_IN_TICKS MS_TO_TICKS(CLICK_DELAY)
#define MULTIPLE_CLICK_DELAY_IN_TICKS MS_TO_TICKS(MULTIPLE_CLICK_DELAY)
#define PRESS_DELAY_IN_TICKS MS_TO_TICKS(PRESS_DELAY)
#define CLAMPED_DELAY_IN_TICKS MS_TO_TICKS(CLAMPED_DELAY)
#define CLAMPED_PERIOD_IN_TICKS MS_TO_TICKS(CLAMPED_PERIOD)
#define LONG_PRESS_DELAY_IN_TICKS MS_TO_TICKS(LONG_PRESS_DELAY)
#define TIMEOUT_DELAY_IN_TICKS MS_TO_TICKS(TIMEOUT_DELAY)


namespace button_lib
{

    // Is received when user makes some end action(like releasing a button)
    enum Event {
        NO_EVENT = 0x0000,
        CLICKED = 0x0001,
        PRESSED = 0x0002,
        LONG_PRESSED = 0x0004,

        CLICK_RELEASED = 0x0008,
        PRESS_RELEASED = 0x0010,
        LONG_PRESS_RELEASED = 0x0020,

        // Async event! Occuring independently with CLICKED | PRESSED | LONG_PRESSED | TIMEOUT
        CLAMPED = 0x1000,

        // Async event! Occuring independently with CLICKED
        DOUBLE_CLICKED = 0x2000,

        // Async event! Occuring independently with CLICKED
        MULTIPLE_CLICKED = 0x4000,

        TIMEOUT = 0x8000
    };

    class ButtonBase
    {
        // Status - I/O

    public:
        enum Status {
            RELEASED = 0,
            PUSHED = 1
        };


    public:
        void setStatus(Status status);
        Event getEvent();
        void reset();
        void tick();

        Status getStatus() {
            return status;
        }

    private:
        void scanReleaseEvents();
        void scanPushEvents();
        void processRelease();
        void processPush();

    protected:
        Status status = RELEASED;
        uint32_t released_time = 0;
        uint32_t pushed_time = 0;

    public:
        volatile uint16_t event_collection = 0x0000;
        uint32_t clamped_divider = 0;
        uint8_t clicked_counter = 0;
    };


    /****************************************************************************************/
    class Button : public ButtonBase
    {
    public:
        Button(volatile uint32_t* states_variable_ptr, const uint32_t mask, const uint8_t clicked_state = 1)
            : register_ptr(states_variable_ptr), register_mask(mask), clicked_state(clicked_state) {}

        void tick();

    private:
        volatile uint32_t* const register_ptr;
        const uint32_t register_mask;
        const uint8_t clicked_state;
    };

} // namespace button_lib
