#ifndef ENCODEDPACKET_H
#define ENCODEDPACKET_H

#include <Types.h>
#include <Packet.h>

class EncodedPacket : public Packet
{
private:
    uint8_t encodedData[MAX_ENCODED_BYTES];
    uint16_t dataLen;
    uint16_t encodeData(uint8_t* decodedData, uint16_t dataLen);

public:
    EncodedPacket(packet_opcode_t opcode, uint16_t blockNumber);
    EncodedPacket(packet_opcode_t opcode, uint16_t blockNumber, uint8_t* data, uint16_t dataLen);
    uint16_t getSize();
    uint16_t getBytes(uint8_t* destination);
};

#endif // ENCODEDPACKET_H
