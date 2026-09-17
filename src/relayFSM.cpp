#include "../include/relayFSM.h"

uint8_t relayTick(const uint8_t state){
    relayAction();
    return state;
}

void relayAction() {
    if (manager.isEmpty()) return; // No action state if channel queue is empty

    // Iterate through the queue and manage channels
    for (uint_fast8_t i = 0; i < manager.getCount(); ++i) {
        TriggerEvent element{};
        manager.getAtIdx(i, element); // Get active event at index i
        if (!element.set) {
            if (toggleRelay(element.ch)) {
                // Update the event to TRIGGERED
                manager.setAsTriggered(true, millis(), i);
                return;
            }
        }

        if ((millis() - element.startTime) >= DURATION) {
            manager.dequeue();
            toggleRelay(element.ch);
        }
    }
}

bool toggleRelay(const ChannelMap ch){
    switch (ch) {
        case (CHANNEL1):
            digitalWrite(CH1, !digitalRead(CH1));
            return true;

        case (CHANNEL2):
            digitalWrite(CH2, !digitalRead(CH2));
            return true;

        case (CHANNEL3):
            digitalWrite(CH3, !digitalRead(CH3));
            return true;

        case (CHANNEL4):
            digitalWrite(CH4, !digitalRead(CH4));
            return true;
    }
    return false;
}