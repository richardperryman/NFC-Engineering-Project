#include <Packet.h>

Packet::Packet(packet_opcode_t opcode)
{
    this->opcode = opcode;
}

packet_opcode_t Packet::getOpcode()
{
    return opcode;
}

SETUPPacket::SETUPPacket() : Packet(OPCODE_SETUP)
{
    
}
