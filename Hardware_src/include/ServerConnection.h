#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <vector>
#include <Types.h>
#include <AuthenticationToken.h>
#include <AuthenticationModule.h>
#include <curl/curl.h>

/**
 * @class ServerConnection
 * @brief This class handles interactions with the SBACS server.
 * @author jessicamorris
 *
 * Uses the cURL library generate HTTPS requests for access.
 */

/**
 * @fn void openConnection()
 * @brief Initializes the cURL library.
 */
 
/**
 * @fn void closeConnection()
 * @brief Deinitializes the cURL library.
 */
 
/**
 * @fn int verifyConnection()
 * @brief Verifies the connection to the SBACS server by sending a empty GET request.
 * @return -1 on connection failure, 0 on success.
 */
 
/**
 * @fn int requestAccess()
 * @brief Given an array of authentication modules, converts their tokens into a HTTP request to the SBACS server.
 * @return -1 on access denied or connection failure, 0 on access granted.
 */

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
