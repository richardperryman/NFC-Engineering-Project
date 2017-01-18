
#include <AuthenticationToken.h>
#include <cstring>

AuthenticationToken::AuthenticationToken(uint16_t tokenSize, uint8_t* tokenData)
{    
    this->tokenSize = (tokenSize > MAX_TOKEN_SIZE) ? MAX_TOKEN_SIZE : tokenSize;
    memcpy(this->tokenData, tokenData, this->tokenSize);
}

uint16_t AuthenticationToken::getSize()
{
    return this->tokenSize;
}

const char* AuthenticationToken::getData()
{
    return (const char*)(this->tokenData);
}

uint8_t AuthenticationToken::getDataByteAt(uint16_t index)
{
    return tokenData[index];
}

std::string* AuthenticationToken::toString() {
    return new std::string((const char*)this->tokenData, this->tokenSize);
}
