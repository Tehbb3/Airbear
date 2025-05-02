#ifndef LOGGER_H
#define LOGGER_H

#include "globals.h"

enum LogLevel {
    LOG_LEVEL_OFF = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

void debugMsg(String, uint8_t);

#endif