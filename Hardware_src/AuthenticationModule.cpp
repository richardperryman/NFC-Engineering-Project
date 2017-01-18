
#include <AuthenticationModule.h>
#include <Debug.h>

AuthenticationModule::AuthenticationModule(std::string id, Serial* connection)
{
    this->id = id;
    this->connection = connection;
    this->token = nullptr;
}

const char* AuthenticationModule::getID()
{
    return this->id.c_str();
}

bool AuthenticationModule::getToken()
{
    if (this->hasToken())
    {
        return false;
    }
    else
    {
        bool success = false;
        uint16_t dataLen = 0;
        uint8_t destination[MAX_TOKEN_SIZE];
        
        // Receive REQUEST
        DecodedPacket* requestPacket = connection->receivePacket();
        
        if (requestPacket != nullptr)
        {
            if (requestPacket->getOpcode() != OPCODE_REQUEST)
            {
                DEBUG_LOG(ERROR, __FUNCTION__, "Received request packet with bad opcode.");
                delete(requestPacket);
            }
            else
            {
                uint16_t numDataPackets = requestPacket->getBlockNumber();
                uint16_t currentDataPacket = 0x0000;
                delete(requestPacket);
                
                DEBUG_LOG(INFO, __FUNCTION__, "Expecting %d data packets from module %s.", numDataPackets, this->id.c_str());
                
                bool error = false;
                for (; currentDataPacket < numDataPackets + 1 && !error; currentDataPacket++)
                {
                    // Send ACK
                    EncodedPacket *ack = new EncodedPacket(OPCODE_ACK, currentDataPacket);
                    connection->sendPacket(*ack);
                    
                    // Receive DATA
                    if (currentDataPacket < numDataPackets)
                    {
                        DecodedPacket *data = connection->receivePacket();
                            
                        if (data == nullptr)
                        {
                            DEBUG_LOG(ERROR, __FUNCTION__, "Data packet timed out.");
                            error = true;
                        }
                        else
                        {
                            if (data->getOpcode() != OPCODE_DATA)
                            {
                                DEBUG_LOG(ERROR, __FUNCTION__, "Received data packet with bad opcode.");
                                error = true;
                            }
                            else
                            {
                                DEBUG_LOG(INFO, __FUNCTION__, "Received data packet of length %d", data->getDataSize());
                                
                                if ((dataLen + data->getDataSize()) > MAX_TOKEN_SIZE)
                                {
                                    DEBUG_LOG(WARNING, __FUNCTION__, "Too much data sent from module %s, token will be truncated.", this->id.c_str());
                                    error = true;
                                }
                                
                                printf("Received packet bytes:\n");
                                int i = 0;
                                for (; (i + dataLen) < data->getDataSize(); i++) {
                                    destination[i + dataLen] = data->getDataByteAt(i);
                                    printf("0x%02X ", destination[i + dataLen]);
                                    if (i > 0 && (i+1)%16 == 0) printf("\n");
                                }
                                printf("\n");
                            
                                dataLen += i;
                            
                                delete(data);
                            }
                        }
                    }
                }
                
                if (!error)
                {
                    success = true;
                    this->token = new AuthenticationToken(dataLen, destination);
                    connection->closePort(); // block any new tokens from coming in
                }
            }
        }
        
        return success;
    }
}

bool AuthenticationModule::hasToken()
{
    return (this->token != nullptr);
}

void AuthenticationModule::clearToken()
{
    delete(this->token);
    this->token = nullptr;
    connection->openPort(SERIAL_BAUD_NFC);
}

const char* AuthenticationModule::getTokenString()
{
    return this->token->toString()->c_str();
}
