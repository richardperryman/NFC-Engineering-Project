#ifndef DATAPACKET_H
#define DATAPACKET_H

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include <Types.h>

/**
 * @class DataPacket
 * @brief A class representing a data packet passed through the serial port.
 * @author jessicamorris
 *
 * Provides methods for creating a packet using generated data (i.e. user-
 * created byte arrays/strings), and also for decoding a packet received
 * through the serial port.
 */

#define MAX_DATA_BYTES 508
#define MAX_ENCODED_BYTES 512
#define PACKET_FLAG 0x7E

class DataPacket
{
private:
    bool isEncoded;
    packet_opcode_t opcode;
    uint16_t blockNum;
    uint8_t decodedData[MAX_DATA_BYTES];
    uint8_t encodedData[MAX_ENCODED_BYTES];
    uint16_t dataLen;
    
    void clearArrays(); // Sets everything in encodedData/decodedData to null
    uint16_t getEncodedSize();
    uint16_t getDecodedSize();
    uint16_t encodeData();
    uint16_t decodeData();
    
public:
    DataPacket(uint8_t packetData[], uint16_t dataLen); // For decoding
    DataPacket(packet_opcode_t opcode); // SETUP Packet constructor
    DataPacket(packet_opcode_t opcode, uint16_t blockNum); // ACK packet constructor
    #ifndef ARDUINO
    DataPacket(packet_opcode_t opcode, std::string data); // For encoding (DATA packet constructor)
    #endif
    DataPacket(packet_opcode_t opcode, uint16_t blockNum, uint8_t data[], uint16_t dataLen); // For encoding (DATA packet constructor)
    packet_opcode_t getOpcode();
    uint16_t getDataSize();
    uint16_t getData(uint8_t* destination);
    uint16_t getPacketSize();
    uint16_t toByteArray(uint8_t* destination);
};

#endif // DATAPACKET_H
