
#include <DecodedPacket.h>

DecodedPacket::DecodedPacket(uint8_t* data, uint16_t dataLen)
{
    // First byte is the opening FLAG byte, so ignore it
    uint16_t index = 1;
    this->opcode = (packet_opcode_t)((data[index] << 8) + data[index+1]);
    
    index = 3;
    this->blockNumber = (uint16_t)((data[index] << 8) + data[index+1]);
    
    if (dataLen > 6) // For DATA/ERROR packet
    {
        // The last byte is the closing FLAG byte, so ignore it
        index = 5;
        this->dataLen = decodeData(data+index, dataLen-index-1);
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
        
        if (readIndex + code > dataLen && code != 0x01)
        {
            return 0;
        }
        
        readIndex++;
        
        if (encodedData[readIndex] != 0x00)
        {
            for (i = 1; i < code; i++)
            {
                decodedData[writeIndex++] = encodedData[readIndex++];
            }
        
            if (code != 0xFF && readIndex < dataLen)
            {
                decodedData[writeIndex++] = 0x00;
            }
        }
    }
    
    return writeIndex;
}

packet_opcode_t DecodedPacket::getOpcode()
{
    return opcode;
}

uint16_t DecodedPacket::getBlockNumber()
{
    return blockNumber;
}

uint16_t DecodedPacket::getData(uint8_t* destination)
{
    uint16_t i = 0;
    for (; i < dataLen; i++)
    {
        destination[i] = decodedData[i];
    }
    
    return i;
}

uint16_t DecodedPacket::getDataSize()
{
    return dataLen;
}

uint8_t DecodedPacket::getDataByteAt(uint16_t index)
{
    return decodedData[index];
}
