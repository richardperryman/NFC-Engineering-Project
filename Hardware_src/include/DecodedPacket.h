#ifndef DECODEDPACKET_H
#define DECODEDPACKET_H

#include <Types.h>
#include <Packet.h>

class DecodedPacket
{
private:
    packet_opcode_t opcode;
    uint16_t blockNumber;
    uint8_t decodedData[MAX_DECODED_BYTES];
    uint16_t dataLen;
    uint16_t decodeData(uint8_t* encodedData, uint16_t dataLen);
    
public:
    DecodedPacket(uint8_t* data, uint16_t dataLen);
    packet_opcode_t getOpcode();
    uint16_t getBlockNumber();
    uint16_t getData(uint8_t* destination);
    uint16_t getDataSize();
    uint8_t getDataByteAt(uint16_t index);
};

#endif // DECODEDPACKET_H
