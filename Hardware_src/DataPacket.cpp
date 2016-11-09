
#ifndef ARDUINO
#include <string.h> // For memcpy and strncpy
#endif

#include <DataPacket.h>

DataPacket::DataPacket(uint8_t packetData[], uint16_t dataLen)
{
    isEncoded = true;
    
    // First byte is the opening FLAG byte so ignore it
    this->opcode = (packet_opcode_t)((packetData[1] << 8) + (packetData[2] & 0xFF));
    
    if (dataLen > 4)
    {
        this->blockNum = (uint16_t)((packetData[3] << 8) + (packetData[4] & 0xFF));
    
        clearArrays();
          
        // Read encoded data in until the null character
        uint16_t index = 5;
        do
        {
            encodedData[index-5] = packetData[index];
            index++;
        } while (index < dataLen-1);

        this->dataLen = index-3;
    }
    else
    {
        this->dataLen = dataLen - 2;
    }
}

DataPacket::DataPacket(packet_opcode_t opcode)
{
    isEncoded = false;
    this->opcode = opcode;
    this->blockNum = 0xFFFF;
    
    clearArrays();
    dataLen = 0;
}

DataPacket::DataPacket(packet_opcode_t opcode, uint16_t blockNum)
{
    isEncoded = false;
    this->opcode = opcode;
    this->blockNum = blockNum;
    
    clearArrays();
    dataLen = 0;
}

#ifndef ARDUINO
DataPacket::DataPacket(packet_opcode_t opcode, std::string data)
{
    isEncoded = false;
    this->opcode = opcode;
    
    clearArrays();
    
    // Convert the string to a byte array with c_str()
    strncpy((char*)this->decodedData, data.c_str(), MAX_DATA_BYTES);
}
#endif

DataPacket::DataPacket(packet_opcode_t opcode, uint16_t blockNum, uint8_t data[], uint16_t dataLen)
{
    isEncoded = false;
    this->opcode = opcode;
    this->blockNum = blockNum;
    
    // Get the number of bytes to copy
    #ifndef ARDUINO
    this->dataLen = std::min(MAX_DATA_BYTES, (int)dataLen);
    #else
    this->dataLen = min(MAX_DATA_BYTES, (int)dataLen);
    #endif
    
    clearArrays();
    
    // Copy the bytes
    memcpy(decodedData, data, this->dataLen);
}

void DataPacket::clearArrays()
{
    for (int i = 0; i < MAX_ENCODED_BYTES; i++)
    {
        if (i < MAX_DATA_BYTES)
        {
            decodedData[i] = '\0';
        }
        encodedData[i] = '\0';
    }
}

packet_opcode_t DataPacket::getOpcode()
{
    return opcode;
}

uint16_t DataPacket::getEncodedSize()
{
    if (dataLen == 0)
    {
        return 0;
    }
    else if (this->isEncoded)
    {
        return dataLen;
    } else {
        return (dataLen + (dataLen / 254 + (dataLen % 254 != 0)) + 1);
    }
}

uint16_t DataPacket::getDecodedSize()
{
    if (dataLen == 0)
    {
        return 0;
    }
    else if (isEncoded) {
        return (254 * (dataLen - 1)) / 255;
    }
    else {
        return dataLen;
    }
}

uint16_t DataPacket::encodeData()
{   
    if (dataLen == 0)
    {
        return 0;
    }
    else if (isEncoded)
    {
        return this->dataLen;
    }
    
	uint16_t readIndex = 0;
	uint16_t writeIndex = 1;
	uint16_t codeIndex = 0;
    uint8_t code = 0x01;
	
	while (readIndex < dataLen)
	{
		// Replace null char with pointer to next null
		if (decodedData[readIndex] == 0x00)
		{
			encodedData[codeIndex] = code;
			code = 0x01;
			codeIndex = writeIndex++;
			readIndex++;
		}
		else
		{
			encodedData[writeIndex++] = decodedData[readIndex++];
			code++;
			
			// Reset after hitting 254 bytes read
			if (code == 0xFF)
			{
				encodedData[codeIndex] = code;
				code = 0x01;
				codeIndex = writeIndex++;
			}
		}
	}
    
    this->encodedData[codeIndex] = code;
	
    return writeIndex + 1;
}

uint16_t DataPacket::decodeData()
{
    if (dataLen == 0)
    {
        return 0;
    }
    else if (!isEncoded)
    {
        return dataLen;
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
        
        for (i = 1; i < code; i++)
        {
            decodedData[writeIndex++] = encodedData[readIndex++];
        }
        
        if (code != 0xFF && readIndex < dataLen - 1)
        {
            decodedData[writeIndex++] = '\0';
        }
    }
    
    return writeIndex;
}

uint16_t DataPacket::getDataSize()
{
    return getDecodedSize();
}

uint16_t DataPacket::getData(uint8_t* destination)
{
    uint16_t i;
    uint16_t maxIndex;
    
    if (!isEncoded)
    {
        maxIndex = dataLen;
    } else {
        maxIndex = getDecodedSize();
        decodeData();
    }
    
    for (i = 0; i < maxIndex; i++)
    {
        destination[i] = decodedData[i];
    }
       
    return maxIndex;
}

uint16_t DataPacket::getPacketSize()
{
    // Final packet size is encoded size + 2 flag bytes + 2 opcode bytes
    // Optional 2 bytes for block number
    if (blockNum == 0xFFFF)
    {
        return (uint16_t)(getEncodedSize() + 4);
    }
    else
    {
        return (uint16_t)(getEncodedSize() + 6);
    }
}

uint16_t DataPacket::toByteArray(uint8_t* destination)
{
    if (!isEncoded)
    {
        encodeData();
    }
    
    uint16_t i = 0;
    
    destination[i++] = PACKET_FLAG;
    destination[i++] = (opcode >> 8) & 0xFF;
    destination[i++] = opcode & 0xFF;
    
    if (blockNum != 0xFFFF)
    {
        destination[i++] = (blockNum >> 8) & 0xFF;
        destination[i++] = opcode & 0xFF;
    }
    
    uint16_t offset = i;
    for (; i < this->getEncodedSize() + offset; i++)
    {
        destination[i] = encodedData[i - offset];
    }
    
    destination[i++] = PACKET_FLAG;
    
    return i;
}
