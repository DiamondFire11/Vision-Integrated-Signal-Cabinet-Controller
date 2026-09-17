#pragma once

#include "ChannelManager.h"

extern ChannelManager manager;

enum readStates{RUN};

uint8_t readTick(uint8_t);
__attribute__((unused)) void readTransition();
void readAction();
