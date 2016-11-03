
#include <string.h>
#include <DataPacket.h>
#include <Debug.h>
using namespace std;

DataPacket::DataPacket(packet_opcode_t opcode, string data)
{
    this->opcode = opcode;
    
    // Convert the string to a byte array with c_str()
    strncpy((char*)this->data, data.c_str(), MAX_DATA_BYTES);
}

DataPacket::DataPacket(packet_opcode_t opcode, uint8_t data[])
{
    this->opcode = opcode;
    memcpy(this->data, data, (sizeof(data)/sizeof(*data)));
}


uint8_t* DataPacket::encodeData()
{
    return 0;
}


uint8_t* DataPacket::decodeData()
{
    return 0;
}

