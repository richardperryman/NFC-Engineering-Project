
#include <Packet.h>

Packet::Packet(packet_opcode_t opcode, uint16_t blockNumber)
{
    this->opcode = opcode;
    this->blockNumber = blockNumber;
}

packet_opcode_t Packet::getOpcode()
{
    return this->opcode;
}

uint16_t Packet::getBlockNumber()
{
    return this->blockNumber;
}
