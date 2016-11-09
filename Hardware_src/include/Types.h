#ifndef TYPES_H
#define TYPES_H

#ifndef ARDUINO // Makes this header Arduino-safe
#include <string>
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

// Missing opcodes for startup sequence stuff
// Not sure if I'll leave this as 16-bit hex numbers or convert to byte array
enum packet_opcode_t {
    OPCODE_REQ = 0x0001, // Request authentication sequence start
    OPCODE_ACK = 0x0002, // Acknowledge request/data
    OPCODE_DATA = 0x0003, // Send data
    OPCODE_SETUP = 0x1111, // Setup opcode
    OPCODE_ERR = 0xFFFF // Error
};

#endif // TYPES_H
