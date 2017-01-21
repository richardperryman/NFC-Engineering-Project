#ifndef DECODEDPACKET_H
#define DECODEDPACKET_H

#include <Types.h>
#include <Packet.h>

class DecodedPacket : public Packet
{
private:
    uint8_t decodedData[MAX_DECODED_BYTES];
    uint16_t dataLen;
    uint16_t decodeData(uint8_t* encodedData, uint16_t dataLen);
    
public:
    DecodedPacket(uint8_t* data, uint16_t dataLen);
    uint16_t getSize();
    uint16_t getBytes(uint8_t* destination);
    uint8_t getByteAt(uint16_t index);
};

#endif // DECODEDPACKET_H
