#include "../include/serialReadFSM.h"

uint8_t readTick(const uint8_t state) {
    readAction();
    return state;
}

void readAction() {
    char buffer[32];
    EnqueueResult success;

    // If there is no data in the buffer do nothing
    if (Serial.available() == 0) return;

    // Read next byte and check for new line or returns.
    char channel = static_cast<char>(Serial.read());

    // Check for \n or \r
    if (channel == '\r' || channel == '\n') return;

    channel -= '0';
    switch(channel){
        case CHANNEL1:
            success = manager.enqueue({CHANNEL1, 0, false});
            break;

        case CHANNEL2:
            success = manager.enqueue({CHANNEL2, 0, false});
            break;

        case CHANNEL3:
            success = manager.enqueue({CHANNEL3, 0,false});
            break;

        case CHANNEL4:
            success = manager.enqueue({CHANNEL4, 0, false});
            break;

        default:
            success = ENQ_BAD_CMD;
    }

    if (success == ENQ_OK) {
        snprintf(buffer, sizeof(buffer), "<OK,CH:%u>\r\n", channel);
        Serial.print(buffer);
    }

    if(success == ENQ_FULL)
        Serial.print("<ERR,FULL>\r\n");

    if(success == ENQ_BAD_CMD)
        Serial.print("<ERR,BAD_CMD>\r\n");

    if(success == ENQ_PRESENT) {
        Serial.print("<ERR,CH_ACTIVE>\r\n");
        Serial.print("<STS, CH_EXT>\r\n");
    }
}
