#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>


#ifndef BUTTON_TICKING_FREQUENCY
#define BUTTON_TICKING_FREQUENCY 100    // in Hz
#endif

#define MS_TO_TICKS(s) int(s * (BUTTON_TICKING_FREQUENCY / 1000.))

// Delays in ms

#ifndef CLICK_DELAY
#define CLICK_DELAY MS_TO_TICKS(60)    // Must exceed debounce time
#endif

#ifndef PRESS_DELAY
#define PRESS_DELAY MS_TO_TICKS(500)
#endif

#ifndef CLAMPED_DELAY
#define CLAMPED_DELAY MS_TO_TICKS(2000)
#endif

#ifndef LONG_PRESS_DELAY
#define LONG_PRESS_DELAY MS_TO_TICKS(2010)
#endif

#ifndef MULTIPLE_CLICKING_DELAY
#define MULTIPLE_CLICKING_DELAY MS_TO_TICKS(150)
#endif

#ifndef TIMEOUT_DELAY
#define TIMEOUT_DELAY MS_TO_TICKS(10000)
#endif




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
        TRIPLE_CLICKED = 0x04
    };

public:
    Button(volatile uint32_t *registerToCheck, const uint32_t registerMask, const uint8_t stateWhenOn = 1);

    void reset();
    void freeze();
    uint8_t isMod(Mod mod_to_check);
    void clearState();

    void tick();

private:
    volatile uint32_t *register_to_check = nullptr;
    const uint32_t register_mask;
    const uint8_t state_when_on;

    uint8_t clicked_counter = 0;
    uint32_t released_time = 0;
    uint8_t freezed = 0;

    // For clamping proportional with time
    uint32_t clamped_counter = 0;
    volatile Mod mods = Mod::NO_MODS;

public:
    uint32_t pressed_time = 0;
    volatile State state = State::NO_STATE;
};

#endif // BUTTON_H_