#pragma once

#include <Arduino.h>

constexpr uint8_t BUFFER_SIZE = 8;
constexpr uint32_t DURATION = 5000;

static_assert((BUFFER_SIZE & (BUFFER_SIZE - 1)) == 0, "Buffer size must be a power of 2!");

enum ChannelMap {
    CHANNEL1 = 1,
    CHANNEL2 = 2,
    CHANNEL3 = 3,
    CHANNEL4 = 4
};

enum EnqueueResult {
    ENQ_OK,
    ENQ_FULL,
    ENQ_BAD_CMD,
    ENQ_PRESENT
};

enum DequeueResult {
    DQ_OK,
    DQ_FAIL
};

struct TriggerEvent {
    ChannelMap ch;
    uint32_t startTime;
    bool set;
};

class ChannelManager {
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t activeMask;
    TriggerEvent triggerBuffer[BUFFER_SIZE];

public:
    ChannelManager();

    EnqueueResult enqueue(const TriggerEvent &);

    DequeueResult dequeue();

    bool peek(TriggerEvent &) const;
    bool getAtIdx(uint8_t, TriggerEvent &) const;
    bool findCh(ChannelMap, uint8_t &) const;

    bool setAsTriggered(bool, uint32_t, uint8_t);
    bool extendChannel(ChannelMap, uint32_t);

    void clearAll();
    uint8_t getCount() const { return count; }

    bool isFull() const { return count == BUFFER_SIZE; }
    bool isEmpty() const { return count == 0; }

    bool contains(ChannelMap) const;
};
