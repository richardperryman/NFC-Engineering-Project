
#include <DecodedPacket.h>

DecodedPacket::DecodedPacket(uint8_t* data, uint16_t dataLen) : Packet((packet_opcode_t)((data[1] << 8) + data[2]), (uint16_t)((data[3] << 8) + data[4]))
{
    if (dataLen > 6) // For DATA/ERROR packet
    {
        // Subtract 6 from total data length (2 flag bytes, 2 bytes for opcode, 2 bytes for block number)
        this->dataLen = decodeData(data+5, dataLen-6);
    }
    else // For SETUP/REQ/ACK packet
    {
        this->dataLen = 0;
    }
}

uint16_t DecodedPacket::decodeData(uint8_t* encodedData, uint16_t dataLen)
{
    if (dataLen == 0)
    {
        return 0;
    }
    
    uint16_t readIndex = 0;
    uint16_t writeIndex = 0;
    uint16_t i;
    uint8_t code;
    
    while (readIndex < dataLen-1)
    {
        code = encodedData[readIndex];
        if (code > PACKET_FLAG) code--;
        
        if (readIndex + code > dataLen && code != 0x01)
        {
            return 0;
        }
        
        readIndex++;
        
        if (encodedData[readIndex] != PACKET_FLAG)
        {
            for (i = 1; i < code; i++)
            {
                this->decodedData[writeIndex++] = encodedData[readIndex++];
            }
        
            if (code != 0xFF && readIndex < dataLen)
            {
                this->decodedData[writeIndex++] = PACKET_FLAG;
            }
        }
    }
    
    return writeIndex;
}

uint16_t DecodedPacket::getSize()
{
    return this->dataLen;
}

uint16_t DecodedPacket::getBytes(uint8_t* destination)
{
    uint16_t i = 0;
    for (; i < dataLen; i++)
    {
        destination[i] = this->decodedData[i];
    }
    
    return i;
}

uint8_t DecodedPacket::getByteAt(uint16_t index)
{
    return this->decodedData[index];
}
