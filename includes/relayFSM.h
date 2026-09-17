#pragma once

#include "ChannelManager.h"

extern ChannelManager manager;

enum relayStates{LOOP};

uint8_t relayTick(uint8_t);
__attribute__((unused)) void relayTransition();
void relayAction();
bool toggleRelay(ChannelMap);