#ifndef AUTHENTICATIONMODULE_H
#define AUTHENTICATIONMODULE_H

#include <Serial.h>
#include <AuthenticationToken.h>

class AuthenticationModule
{
private:
    std::string id;
    Serial* connection;
    AuthenticationToken* token;
public:
    AuthenticationModule(std::string id, Serial* connection);
    const char* getID();
    bool getToken();
    bool hasToken();
    void clearToken();
    const char* getTokenString();
};

#endif // AUTHENTICATIONMODULE_H
