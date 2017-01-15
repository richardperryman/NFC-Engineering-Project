#ifndef AUTHENTICATIONTOKEN_H
#define AUTHENTICATIONTOKEN_H

#include <Types.h>

class AuthenticationToken
{
private:
    uint16_t tokenSize;
    uint8_t* tokenData;

public:
    AuthenticationToken(uint16_t tokenSize, uint8_t* tokenData);
    const char* getData();
    uint16_t getSize();
    std::string* toString();
};

#endif // AUTHENTICATIONTOKEN_H
