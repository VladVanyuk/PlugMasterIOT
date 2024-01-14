#ifndef Relay_h
#define Relay_h

#include <Arduino.h>
#include "pins.h"

#ifndef TRUE
#define TRUE true
#define FALSE false
#endif



enum RelayState_t
{
    ON,
    OFF
};

bool relay_state;
bool relay_state_changed = false;

bool relay_change_flag();
bool toggle_relay_state();
bool get_relay_state();

bool set_relay_state(RelayState_t state);

bool relay_change_flag()
{
    relay_state_changed = true;
    return relay_state_changed;
}

bool toggle_relay_state()
{
    relay_state = !relay_state;
    digitalWrite(RELAY_PIN, relay_state);
    return relay_state;
}


bool set_relay_state(RelayState_t state)
{
    if (relay_state == state)
    {
        return false; // no change
    }
    else
    {
        relay_state = state ? TRUE : FALSE;
        digitalWrite(RELAY_PIN, relay_state);
        return true; // changed
    }
}

void relay_setup()
{
    relay_state = OFF;

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, relay_state);
}

void relay_handler()
{
    if (relay_state_changed)
    {
        toggle_relay_state();
        relay_state_changed = false;
    }
}

#endif //Relay_h