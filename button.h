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


class ButtonBase
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
    };

    enum Mod
    {
        NO_MODS = 0x00,
        CLAMPED = 0x01,
        DOUBLE_CLICKED = 0x02,
        TRIPLE_CLICKED = 0x04,
        TIMEOUT = 0x08
    };

    enum Status {
        RELEASED,
        PUSHED
    };

public:    
    void setStatus(Status status) {
        this->status = status;
    }

    Status getStatus() {
        return status;
    }

    void freeze() {
        freezed = 1;
    }

    void reset();
    void clearState();
    uint8_t isMod(Mod mod_to_check);

    void tick();

private:
    void setMod(Mod mod_to_set);
    void processRelease();
    void processPush();

protected:
    Status status = RELEASED;

    uint8_t freezed = 0;
    uint32_t pushed_time = 0;
    uint32_t released_time = 0;
    volatile Mod mods = Mod::NO_MODS;


public:
    volatile State state = State::NO_STATE;

    uint32_t clamped_counter = 0;
    uint8_t clicked_counter = 0;
};


/****************************************************************************************/
class Button : public ButtonBase
{   
public:
    Button(volatile uint32_t* const registerToCheck, const uint32_t registerMask, const uint8_t stateWhenOn=1);

    void tick();

private:
    volatile uint32_t* const register_to_check;
    const uint32_t register_mask;
    const uint8_t state_when_on;
};

#endif // BUTTON_H_