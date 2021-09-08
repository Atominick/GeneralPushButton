#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>


#ifndef BUTTON_TICKING_FREQUENCY
#define BUTTON_TICKING_FREQUENCY 100    // in Hz
#endif

#define TO_TICKS(s) int(s * (1000 / BUTTON_TICKING_FREQUENCY))

// Delays in ms
#define CLICK_DELAY TO_TICKS(60)    // Must exceed debounce time
#define PRESS_DELAY TO_TICKS(500)
#define CLAMPED_DELAY TO_TICKS(2000)
#define LONG_PRESS_DELAY TO_TICKS(2010)
#define MULTIPLE_CLICKING_DELAY TO_TICKS(150)
#define TIMEOUT_DELAY TO_TICKS(10000)



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
    uint32_t pressed_time = 0;
    uint8_t freezed = 0;

    // For clamping proportional with time
    uint32_t clamped_counter = 0;
    volatile Mod mods = Mod::NO_MODS;

public:
    volatile State state = State::NO_STATE;
};

#endif // BUTTON_H_