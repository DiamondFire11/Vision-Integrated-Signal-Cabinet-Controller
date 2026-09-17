#include "../include/ledFSM.h"

uint8_t ledTick(uint8_t state) {
    state = ledTransition(state);
    ledAction(state);
    return state;
}

uint8_t ledTransition(uint8_t state) {
    switch (state) {
        case LED_INIT:
        case ON:
            state = OFF;
            break;

        case OFF:
            state = ON;
            break;

        default:
            Serial.println("Error: Encountered unknown state, resetting SM.");
            state = LED_INIT;
    }
    return state;
}

void ledAction(const uint8_t state) {
    switch (state) {
        case LED_INIT:
        case OFF:
            digitalWrite(LED_BUILTIN, LOW);
            break;

        case ON:
            digitalWrite(LED_BUILTIN, HIGH);
            break;

        default:
            digitalWrite(LED_BUILTIN, LOW);
    }
}