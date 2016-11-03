#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <Types.h>
using namespace std;

#define MAX_DATA_BYTES 508

class DataPacket
{
private:
    packet_opcode_t opcode;
    uint8_t data[MAX_DATA_BYTES];
    uint8_t dataLen;
    
    uint8_t getEncodedSize();
    uint8_t* encodeData();
    uint8_t* decodeData();
    
public:
    DataPacket(packet_opcode_t opcode, string data);
    DataPacket(packet_opcode_t opcode, uint8_t data[]);
};

#endif // DATAPACKET_H
