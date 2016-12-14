
#include <AuthenticationToken.h>

AuthenticationToken::AuthenticationToken(uint16_t tokenSize, uint8_t* tokenData)
{
    this->tokenSize = tokenSize;
    this->tokenData = tokenData;
}

uint16_t AuthenticationToken::getSize()
{
    return this->tokenSize;
}

std::string* AuthenticationToken::toString() {
    return new std::string((const char*)this->tokenData, this->tokenSize);
}
