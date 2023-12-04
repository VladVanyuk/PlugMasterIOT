#ifndef Relay_h
#define Relay_h

#include <Arduino.h>
#include "pins.h"


#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#define RELAY_ON digitalWrite(RELAY_PIN, FALSE)
#define RELAY_OFF digitalWrite(RELAY_PIN, TRUE)
#define RELAY_STATE(x) digitalWrite(RELAY_PIN, x)

typedef enum 
{
    ON,
    OFF
} RelayState_t;

bool relay_state;
bool relay_state_changed = false;

bool relay_change_flag();
bool toggle_relay_state();
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

RelayState_t get_relay_state()
{
    return (RelayState_t)relay_state;
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