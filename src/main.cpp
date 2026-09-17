#include "../include/ChannelManager.h"
#include "../include/ledFSM.h"
#include "../include/serialReadFSM.h"
#include "../include/relayFSM.h"

#define FW_VERSION "v1.0.1-ringfix " __DATE__ " " __TIME__

struct Tasks {
    uint8_t state;
    uint32_t period;
    uint32_t elapsedTime;
    uint8_t (*tickFcn) (uint8_t);
} typedef Tasks;

Tasks tasks[3]; // Array storing FSMs

ChannelManager manager;

void setup() {
    constexpr uint8_t pins[] = {LED_BUILTIN, CH1, CH2, CH3, CH4};
    for(const uint_fast8_t pin : pins){
        pinMode(pin, OUTPUT);
    }

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);

    // Init LED (CPU Alive) state machine
    tasks[0].state = LED_INIT;
    tasks[0].period = 500;
    tasks[0].elapsedTime = 0;
    tasks[0].tickFcn = &ledTick;

    tasks[1].state = RUN;
    tasks[1].period = 10;
    tasks[1].elapsedTime = 0;
    tasks[1].tickFcn = &readTick;

    tasks[2].state = LOOP;
    tasks[2].period = 10;
    tasks[2].elapsedTime = 0;
    tasks[2].tickFcn = &relayTick;
}

void loop() {
    for (auto & task : tasks) {
        if (millis() - task.elapsedTime >= task.period) {
            task.state = task.tickFcn(task.state);
            task.elapsedTime = millis(); // Last time this task was ran
        }
    }
}
