
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
    
    // Get the number of bytes to copy
    int datasize = sizeof(data)/sizeof(*data);
    this->dataLen = min(MAX_DATA_BYTES, datasize);
    
    // Copy the bytes
    memcpy(this->data, data, this->dataLen);
}

uint8_t DataPacket::getEncodedSize()
{
	return this->dataLen + (this->dataLen / 254) + 1;
}

uint8_t* DataPacket::encodeData()
{
	uint8_t encodedData[getEncodedSize()];
	size_t readIndex = 0;
	size_t writeIndex = 1;
	size_t codeIndex = 0;
	
	while (readIndex < this->dataLen)
	{
		// Replace null char with pointer to next null
		if (this->data[readIndex] == 0)
		{
			encodedData[codeIndex] = code;
			code = 1;
			codeIndex = writeIndex++;
			readIndex++;
		}
		else
		{
			encodedData[writeIndex++] = this->data[readIndex++];
			code++;
			
			// Reset after hitting 254 bytes read
			if (code == 0xFF)
			{
				encodedData[codeIndex] = code;
				code = 1;
				codeIndex = writeIndex++;
			}
		}
	}
	
    return encodedData;
}

uint8_t* DataPacket::decodeData()
{
    return 0;
}

