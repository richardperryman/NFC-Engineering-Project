
#include <EncodedPacket.h>

EncodedPacket::EncodedPacket(packet_opcode_t opcode, uint16_t blockNumber) : Packet(opcode, blockNumber)
{
    this->dataLen = 0;
}

EncodedPacket::EncodedPacket(packet_opcode_t opcode, uint16_t blockNumber,  uint8_t* data, uint16_t dataLen) : Packet(opcode, blockNumber)
{
    #ifndef ARDUINO
    uint16_t dataLimit = std::min((int)dataLen, MAX_ENCODED_BYTES);
    #else
    uint16_t dataLimit = min(dataLen, MAX_ENCODED_BYTES);
    #endif
    this->dataLen = encodeData(data, dataLimit);
}

uint16_t EncodedPacket::encodeData(uint8_t* decodedData, uint16_t dataLen)
{
    if (dataLen == 0)
    {
        return 0;
    }

	uint16_t readIndex = 0;
	uint16_t writeIndex = 1;
	uint16_t codeIndex = 0;
    uint8_t code = 0x01;
	
	while (readIndex < dataLen)
	{
		// Replace null char with pointer to next null
		if (decodedData[readIndex] == PACKET_FLAG)
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
            
            // Skip the packet flag while counting
            if (code == PACKET_FLAG) code++;
			
			// Reset after hitting 254 bytes read
			else if (code == 0xFF)
			{
				encodedData[codeIndex] = code;
				code = 0x01;
				codeIndex = writeIndex++;
			}
		}
	}
    
    encodedData[codeIndex] = code;
    encodedData[writeIndex++] = PACKET_FLAG;
	
    return writeIndex;
}

uint16_t EncodedPacket::getSize()
{
    // Total packet size = data length + 2 opcode bytes + 2 block number bytes + 2 flag bytes
    return dataLen + 6;
}

uint16_t EncodedPacket::getBytes(uint8_t* destination)
{
    uint16_t i = 0;
    
    destination[i++] = PACKET_FLAG;
    destination[i++] = this->getOpcode() >> 8;
    destination[i++] = this->getOpcode() & 0x00FF;  
    destination[i++] = this->getBlockNumber() >> 8;
    destination[i++] = this->getBlockNumber() & 0x00FF;
    
    uint16_t offset = i;
    for (; i < dataLen + offset; i++)
    {
        destination[i] = encodedData[i - offset];
    }
    
    destination[i++] = PACKET_FLAG;
    
    return i;
}
