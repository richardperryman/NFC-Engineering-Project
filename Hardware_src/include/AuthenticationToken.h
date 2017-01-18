#ifndef AUTHENTICATIONTOKEN_H
#define AUTHENTICATIONTOKEN_H

#include <Types.h>

#define MAX_TOKEN_SIZE 2056

class AuthenticationToken
{
private:
    uint16_t tokenSize;
    uint8_t tokenData[MAX_TOKEN_SIZE];

public:
    AuthenticationToken(uint16_t tokenSize, uint8_t* tokenData);
    const char* getData();
    uint16_t getSize();
    uint8_t getDataByteAt(uint16_t index);
    std::string* toString();
};

#endif // AUTHENTICATIONTOKEN_H
