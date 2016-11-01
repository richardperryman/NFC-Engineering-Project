#ifndef SIGNALS_H
#define SIGNALS_H

using namespace std;

/**
 * @file Signals.h
 * @brief A collection of constants used for signalling between the Raspberry Pi and authentication modules.
 * @author jessicamorris
 *
 * Follows the TFTP style (https://tools.ietf.org/html/rfc1350) of a "client" (a module) sending a request
 * to the "server" (the SBACS process) to send an authentication token. Once the server acknowledges the 
 * token, the module will send the token in a single frame.
 * 
 * todo: read up more on hardware communication, not sure yet if any of this is a "good practice"
 * I might just change this to functions that take byte arrays and create byte array messages
 * No major design decisions after 10PM allowed
 */

// Request authentication sequence start - sent by module
// 0x00 0x01 |
//  opcode   |
const uint8_t REQ_AUTH = {};

// Acknowledge authentication start - sent by Pi
// 0x00 0x02 |
//  opcode   |
const uint8_t ACK_AUTH = {};

// Data header
// 0x00 0x03 |      0xXX 0xXX      | token | 0x00 0x00
//  opcode   | # of bytes in token |       | two null chars to terminate
const uint8_t DATA_HEADER = {};

// Data acknowledge
// 0x00 0x04 |    0xXX 0xXX    |
//  opcode   | # of bytes read |
const uint8_t DATA_ACKNOWLEDGE = {};


// Error header
// 0xFF 0xFF |  0xXX 0xXX | string | 0x00 
//  opcode   | error code | error message - null terminated
const uint8_t ERROR_HEADER = {};

#endif
