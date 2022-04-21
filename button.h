#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>


#ifndef BUTTON_TICKING_FREQUENCY
#define BUTTON_TICKING_FREQUENCY 1000    // in Hz
#endif

// Delays in ms
#ifndef CLICK_DELAY
#define CLICK_DELAY 20    // Must exceed debounce time
#endif

#ifndef PRESS_DELAY
#define PRESS_DELAY 450
#endif

#ifndef CLAMPED_DELAY
#define CLAMPED_DELAY 2000
#endif

#ifndef CLAMPED_PERIOD
#define CLAMPED_PERIOD 50
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


/**************************** No need to modify this *********************************/
#define MS_TO_TICKS(ms) (int((float) ms / (1000. / BUTTON_TICKING_FREQUENCY)))

#define CLICK_DELAY_IN_TICKS             MS_TO_TICKS(CLICK_DELAY)
#define PRESS_DELAY_IN_TICKS             MS_TO_TICKS(PRESS_DELAY)
#define CLAMPED_DELAY_IN_TICKS           MS_TO_TICKS(CLAMPED_DELAY)
#define CLAMPED_PERIOD_IN_TICKS          MS_TO_TICKS(CLAMPED_PERIOD)
#define LONG_PRESS_DELAY_IN_TICKS        MS_TO_TICKS(LONG_PRESS_DELAY)
#define MULTIPLE_CLICK_DELAY_IN_TICKS    MS_TO_TICKS(MULTIPLE_CLICK_DELAY)
#define TIMEOUT_DELAY_IN_TICKS           MS_TO_TICKS(TIMEOUT_DELAY)


class ButtonBase
{
    // Status - I/O
    // State - some status for some time means one of possible states
    // Event - event is received when user makes some end action(like releasing a button)

public:
    enum Status {
        RELEASED,
        PUSHED
    };

    enum Event
    {
        NO_EVENT            = 0x0000,
        CLICKED             = 0x0001,
        PRESSED             = 0x0002,
        LONG_PRESSED        = 0x0004,

        CLICK_RELEASED      = 0x0008,
        PRESS_RELEASED      = 0x0010,
        LONG_PRESS_RELEASED = 0x0020,

        CLAMPED             = 0x1000,
        DOUBLE_CLICKED      = 0x2000,
        TRIPLE_CLICKED      = 0x4000,

        TIMEOUT             = 0x8000
    };

    // enum State
    // {
    //     NO_STATE = 0x00,

    //     CLICKED,
    //     PRESSED,
    //     LONG_PRESSED,

    //     CLICK_RELEASED,
    //     PRESS_RELEASED,
    //     LONG_PRESS_RELEASED,
    // };

    // enum Mod
    // {
    //     NO_MODS = 0x00,
    //     CLAMPED = 0x01,
    //     DOUBLE_CLICKED = 0x02,
    //     TRIPLE_CLICKED = 0x04,
    //     TIMEOUT = 0x08,
    //     ANY = 0xFF
    // };

public:
    void setStatus(Status status) {
        this->status = status;
    }

    Status getStatus() {
        return status;
    }

    // Should be called relatively frequently cause return only one event per time
    Event getEvent() {
        for(int i = 0; i < 16; i++) {
            uint16_t new_e = event_collection & (1 << i);

            if(new_e) {
                event_collection &= ~new_e;
                return (Event) new_e;
            }
        }

        return Event::NO_EVENT;
    }

    void freeze() {
        freezed = 1;
    }

    void reset();
    void clearState();
    // uint8_t isMod(Mod mod_to_check);

    void tick();

private:
    // void setMod(Mod mod_to_set);
    void scanPushEvents();
    void processRelease();

protected:
    // volatile Mod mods = Mod::NO_MODS;
    // uint16_t status = RELEASED;
    Status status = RELEASED;

    uint32_t released_time = 0;
    uint32_t pushed_time = 0;
    uint8_t freezed = 0;

public:
    volatile uint16_t event_collection = 0x00;

    uint32_t clamped_divider = 0;
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