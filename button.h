#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>


#ifndef BUTTON_TICKING_FREQUENCY
#define BUTTON_TICKING_FREQUENCY 100    // in Hz
#endif

// Delays in ms
#ifndef CLICK_DELAY
#define CLICK_DELAY 60    // Must exceed debounce time
#endif

#ifndef PRESS_DELAY
#define PRESS_DELAY 450
#endif

#ifndef CLAMPED_DELAY
#define CLAMPED_DELAY 2000
#endif

#ifndef LONG_PRESS_DELAY
#define LONG_PRESS_DELAY 2010
#endif

#ifndef MULTIPLE_CLICK_DELAY
#define MULTIPLE_CLICK_DELAY 150
#endif

#ifndef TIMEOUT_DELAY
#define TIMEOUT_DELAY 5000
#endif


/***************************************************************************/
#define MS_TO_TICKS(s) (int((float) s / (1000. / BUTTON_TICKING_FREQUENCY)))

#define CLICK_DELAY_IN_TICKS             MS_TO_TICKS(CLICK_DELAY)
#define PRESS_DELAY_IN_TICKS             MS_TO_TICKS(PRESS_DELAY)
#define CLAMPED_DELAY_IN_TICKS           MS_TO_TICKS(CLAMPED_DELAY)
#define LONG_PRESS_DELAY_IN_TICKS        MS_TO_TICKS(LONG_PRESS_DELAY)
#define MULTIPLE_CLICK_DELAY_IN_TICKS    MS_TO_TICKS(MULTIPLE_CLICK_DELAY)
#define TIMEOUT_DELAY_IN_TICKS           MS_TO_TICKS(TIMEOUT_DELAY)


class Button
{
public:
    enum State
    {
        NO_STATE = 0x00,
        CLICKED,
        PRESSED,
        LONG_PRESSED,

        CLICK_RELEASED,
        PRESS_RELEASED,
        LONG_PRESS_RELEASED,

        UNCERTAIN,
    };

    enum Mod
    {
        NO_MODS = 0x00,
        CLAMPED = 0x01,
        DOUBLE_CLICKED = 0x02,
        TRIPLE_CLICKED = 0x04,
        TIMING_ERROR = 0x08
    };

public:
    Button(volatile uint32_t *registerToCheck, const uint32_t registerMask, const uint8_t stateWhenOn = 1);

    void reset();
    void freeze();
    void wipe_status();
    uint8_t is_mod(Mod mod_to_check);

    void tick();

private:
    volatile uint32_t *register_to_check = nullptr;
    const uint32_t register_mask;
    const uint8_t state_when_on;

    // For clamping proportional with time
    uint32_t clamped_counter = 0;
    uint8_t clicked_counter = 0;

    uint32_t released_time = 0;
    uint32_t pressed_time = 0;
    uint8_t freezed = 0;

    volatile Mod mods = Mod::NO_MODS;

public:
    volatile State state = State::NO_STATE;
};

#endif // BUTTON_H_