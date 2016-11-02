#ifndef SIGNALS_H
#define SIGNALS_H

// n.b. The functions I need to define for this also fit easily into Serial, so I'll probably remove this header later.
// Keeping it for now so that all my design decisions stay documented.

using namespace std;

/**
 * @file Signals.h
 * @brief A collection of constants used for signalling between the Raspberry Pi and authentication modules.
 * @author jessicamorris
 *
 * Follows the TFTP style (https://tools.ietf.org/html/rfc1350) of a "client" (a module) sending a request
 * to the "server" (the SBACS process) to let it send an authentication token. Once the server acknowledges
 * the request, the module will send the token over one or more 512-byte, encoded data blocks.
 * 
 * Data should be encoded using COBS (https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) so
 * that they can be safely NULL-delimited. Using COBS means that, in the worst case, only 4 bytes in the
 * 512-byte block will be used for encoding (obtained using the equation:
 * enc_size = unenc_size + (unenc_size/254) + 1)
 *
 * Frames will be delimited w/ a FLAG byte at the start and end (01111110b = 0x7E)
 */

#define FLAG 0x7E

// Request authentication sequence start - sent by module
// 0x00 0x01 |    0xXX 0xXX     |
//  opcode   | # of data blocks |
const uint8_t REQ_AUTH = {};

// Request is acknowledged with ACK0
// Acknowledge header
// 0x00 0x02 |  0xXX 0xXX   |
//  opcode   | block number |
const uint8_t ACK_HEADER = {};

// Data header
// 0x00 0x03 |  0xXX 0xXX   |   token   | 0x00
//  opcode   | block number | 512 bytes | null char to terminate as per COBS
const uint8_t DATA_HEADER = {};

// Error header
// 0xFF 0xFF |  0xXX 0xXX | string | 0x00 
//  opcode   | error code | error message - null terminated
const uint8_t ERROR_HEADER = {};

uint8_t encodeData(string data);

#endif
