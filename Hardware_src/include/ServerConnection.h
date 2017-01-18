#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <vector>
#include <Types.h>
#include <AuthenticationToken.h>
#include <AuthenticationModule.h>
#include <curl/curl.h>

class ServerConnection
{
private:
    std::string url;
    
    const char* getURL();
    const char* lock_table_name = "/locks";
    const char* access_table_name = "/access";
    
public:
    ServerConnection(std::string url);
    void openConnection();
    void closeConnection();
    int8_t verifyConnection();
    int8_t requestAccess(uint32_t lock_id, std::vector<AuthenticationModule*>* modules);
};

#endif // SERVERCONNECTION_H
