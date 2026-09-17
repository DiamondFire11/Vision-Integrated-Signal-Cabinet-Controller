#pragma once

#include <Arduino.h>

enum LEDStates{LED_INIT, OFF, ON};

uint8_t ledTick(uint8_t);
uint8_t ledTransition(uint8_t);
void ledAction(uint8_t);