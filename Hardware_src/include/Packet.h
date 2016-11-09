#ifndef PACKET_H
#define PACKET_H

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

class Packet
{
private:
    packet_opcode_t opcode;
public:
    Packet(packet_opcode_t opcode);
    packet_opcode_t getOpcode();
};

/*class ACKPacket : public Packet
{
private:
    uint16_t blockNum;
public:
    uint16_t getBlockNum();
}

class DATAPacket : public Packet
{
private:
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
    
}

class ERRORPacket : public Packet
{
private:
    uint16_t errorCode;
    uint8_t errorMsg;
}

class REQPacket : public Packet
{
private:
    uint16_t numBlocks;
}*/

class SETUPPacket : public Packet
{
public:
    SETUPPacket();
};

#endif // PACKET_H
