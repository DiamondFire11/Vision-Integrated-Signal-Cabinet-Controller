#include "../include/ChannelManager.h"

ChannelManager::ChannelManager() : head(0), tail(0), count(0), activeMask(0), triggerBuffer{} {
}

DequeueResult ChannelManager::dequeue() {
    // Nothing to pop
    if (this->count == 0) return DQ_FAIL;

    const ChannelMap ch = this->triggerBuffer[this->head].ch;
    this->activeMask &= ~(1 << (ch - 1)); // Clear active bit

    // Advance head to dequeue expired event
    this->head = (this->head + 1) & (BUFFER_SIZE - 1);
    --this->count;

    return DQ_OK;
}

EnqueueResult ChannelManager::enqueue(const TriggerEvent &in) {
    // Buffer is full (should never happen)
    if (this->count == BUFFER_SIZE) return ENQ_FULL;

    // Input is not a valid HW channel
    if (CHANNEL1 > in.ch || CHANNEL4 < in.ch) return ENQ_BAD_CMD;

    // Input is valid but already active
    if (this->contains(in.ch)) {
        this->extendChannel(in.ch, millis());
        return ENQ_PRESENT;
    }

    this->activeMask |= (1 << (in.ch - 1)); // Set channel bit to active
    triggerBuffer[this->tail] = in;

    this->tail = (this->tail + 1) & (BUFFER_SIZE - 1);
    ++this->count;

    return ENQ_OK;
}

bool ChannelManager::peek(TriggerEvent &out) const {
    if (this->isEmpty()) return false;

    out = this->triggerBuffer[this->head];
    return true;
}

bool ChannelManager::getAtIdx(const uint8_t idx, TriggerEvent & out) const {
    if (this->isEmpty()) return false;

    out = this->triggerBuffer[(this->head + idx) & (BUFFER_SIZE - 1)];
    return true;
}

bool ChannelManager::findCh(const ChannelMap ch, uint8_t & idx) const {
    if (this->isEmpty()) return false;

    // Iterate through active channels and if that channel is found return it
    for (uint_fast8_t i = 0; i < this->count; ++i) {
        if (this->triggerBuffer[(this->head + i) & (BUFFER_SIZE - 1)].ch == ch) {
            idx = this->head + i;
            return true;
        }
    }

    return false;
}

bool ChannelManager::setAsTriggered(const bool val, const uint32_t time, const uint8_t idx) {
    if (this->isEmpty()) return false;
    this->triggerBuffer[(this->head + idx) & (BUFFER_SIZE - 1)].set = val;
    this->triggerBuffer[(this->head + idx) & (BUFFER_SIZE - 1)].startTime = time;
    return true;
}

bool ChannelManager::extendChannel(const ChannelMap ch, const uint32_t time) {
    if (this->isEmpty()) return false;

    // Search for the requested channel
    uint8_t eventIdx;
    if (!this->findCh(ch, eventIdx)) return false;

    // Update the start time for that channel to extend
    this->triggerBuffer[eventIdx].startTime = time;
    return true;
}

// Return true if channel bit is set high
bool ChannelManager::contains(const ChannelMap channel) const {
    return this->activeMask & (1 << (channel - 1)); // Check for active bit
}

// Clear the ring buffer and cancel all active trigger events
void ChannelManager::clearAll() {
    this->count = 0;
    this->tail = this->head;
}
