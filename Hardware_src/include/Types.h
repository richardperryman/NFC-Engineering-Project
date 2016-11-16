#ifndef TYPES_H
#define TYPES_H

#ifndef ARDUINO // Makes this header Arduino-safe
#include <string>
#else
#include <Arduino.h>
#endif
#include <stdint.h>

/**
 * @file Types.h
 * 
 * This header handles all type imports and type definitions.
 */

enum debug_level_t {
    CRITICAL = 50,
    ERROR = 40,
    WARNING = 30,
    INFO = 20
};

enum packet_opcode_t {
    OPCODE_REQUEST = 0x0001, // Request authentication sequence start
    OPCODE_ACK = 0x0002, // Acknowledge request/data
    OPCODE_DATA = 0x0003, // Send data
    OPCODE_SETUP = 0x1111, // Setup opcode
    OPCODE_ERR = 0xFFFF // Error
};

enum error_code_t {
    ERROR_BADOPCODE = 0x0001,
    ERROR_DECODING = 0x0002
};

#endif // TYPES_H
