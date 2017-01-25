#ifndef PACKET_H
#define PACKET_H

#define PACKET_FLAG 0x7E
#define MAX_ENCODED_BYTES 255
//#ifndef ARDUINO
#define MAX_DECODED_BYTES 253
//#else
//#define MAX_DECODED_BYTES 508
//#endif
#define MAX_PACKET_SIZE (MAX_ENCODED_BYTES + 6)

#include <Types.h>

class Packet
{
private:
    packet_opcode_t opcode;
    uint16_t blockNumber;
    
public:
    Packet(packet_opcode_t opcode, uint16_t blockNumber);
    virtual ~Packet() = default;
    packet_opcode_t getOpcode();
    uint16_t getBlockNumber();
    virtual uint16_t getSize() = 0;
    virtual uint16_t getBytes(uint8_t* destination) = 0;
};

#endif // PACKET_H
