#include <ServerConnection.h>

#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <iomanip>
#include <json-c/json.h>
#include <Debug.h>
#include <AuthenticationModule.h>

ServerConnection::ServerConnection(std::string url)
{
    this->url = url;
}

const char* ServerConnection::getURL()
{
    return this->url.c_str();
}

int8_t ServerConnection::verifyConnection()
{
    int8_t result;
    CURL* curl;
    CURLcode res;
    
    curl = curl_easy_init();

    // Do a simple HTTP GET request to verify the server is online
    curl_easy_setopt(curl, CURLOPT_URL, this->getURL());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1); // Disable printing result
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L); // Give the server 5 seconds to respond
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        DEBUG_LOG(WARNING, __FUNCTION__, "Server %s does not appear to be online. Message: %s\n", this->getURL(), curl_easy_strerror(res));
        result = -1;
    } else {
        DEBUG_LOG(INFO, __FUNCTION__, "Reached %s successfully.", this->getURL());
        result = 0;
    }
    curl_easy_cleanup(curl);
    
    return result;
}

void ServerConnection::openConnection()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void ServerConnection::closeConnection()
{
    curl_global_cleanup();
}

// Generate a random string of bytes
static
void generateSecret(uint8_t* buffer, uint16_t buffLen)
{
    int fd = open("/dev/random", O_RDONLY);
    read(fd, buffer, buffLen);
    if (close(fd) == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Failed to close /dev/random");
    }
}

static
uint16_t byteArrayToHexString(uint8_t* in, uint16_t inLen, std::string* out)
{
    uint16_t i;
    for (i = 0; i < inLen; i++)
    {
        char temp[3];
        sprintf(temp, "%02X", in[i]);
        out->append(temp);
    }
    return i+1;
}

// Process the result of an access request
static
int8_t processResult(CURL* session)
{
    long http_code = 0;
    curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_code);
    
    if (http_code == 401)
    {
        return 1;
    } else if (http_code == 200) {
        return 0;
    } else {
        return -1;
    }
}

// 
static
std::string* removeSpaces(const char* json)
{
    std::string* str = new std::string(json);
    str->erase(remove(str->begin(), str->end(), ' '), str->end() );
    return str;
}

int8_t ServerConnection::requestAccess(uint32_t lock_id, std::vector<AuthenticationModule*>* modules)
{
    // cURL variables
    CURL* curly;
    CURLcode res;
    struct curl_slist *headers = NULL;
    int8_t accessResult;
    
    // Query variables
    std::string url = this->url + access_table_name;
    std::stringstream queryString;
    uint8_t secret[256];
    std::string lock_key;
    
    // JSON variable
    json_object* json;

    // Initialization
    curly = curl_easy_init();
    json = json_object_new_object();
    
    generateSecret(secret, sizeof(secret));
    lock_key.reserve(sizeof(secret)*2);
    byteArrayToHexString(secret, sizeof(secret), &lock_key);
    
    FILE *devnull = fopen("/dev/null", "w+");

    // Generate HTTP header
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");

    // Generate query string
    queryString << "?lock_id=";
    queryString << std::hex << lock_id;
    queryString << "&lock_key=";
    queryString << lock_key;
    url = url + queryString.str();

    // Generate JSON data for module : token info
    json = json_object_new_array();
    for (uint8_t i = 0; i < modules->size(); i++) {
        
        AuthenticationModule* module = modules->at(i);
        if (module->hasToken())
        {
            // The nfc module entry is formatted as
            // type: "nfc"
            // value: "{\"type\":\"Buffer\",\"data\":[...]}" 
            if (strcmp(module->getID(), "nfc") == 0) {
                json_object* token;
                json_object* buffer;
                json_object* data;

                token = json_object_new_object();
                buffer = json_object_new_object();
                data = json_object_new_array();
                json_object_object_add(token, "type", json_object_new_string(module->getID()));
                json_object_object_add(buffer, "type", json_object_new_string("Buffer"));
                for (uint16_t i = 0; i < module->getTokenSize(); i++)
                {
                    json_object_array_add(data, json_object_new_int(module->getTokenByteAt(i)));
                }
                json_object_object_add(buffer, "data", data);
                std::string* value = removeSpaces(json_object_to_json_string(buffer));
                json_object_object_add(token, "value", json_object_new_string(value->c_str()));
                json_object_array_add(json, token);
            }
            
            // Other modules follow the format
            // type: "modulename"
            // value: "value"
            else {
                json_object* token;

                token = json_object_new_object();
                json_object_object_add(token, "type", json_object_new_string(module->getID()));
                json_object_object_add(token, "value", json_object_new_string(module->getTokenString()));
                json_object_array_add(json, token);                
            }
        }        
    }
    
    // Set up the request headers, body, and send it
    curl_easy_setopt(curly, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curly, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curly, CURLOPT_POSTFIELDS, json_object_to_json_string(json));
    curl_easy_setopt(curly, CURLOPT_TIMEOUT, 5L); // Give the server 5 seconds to respond
    curl_easy_setopt(curly, CURLOPT_WRITEDATA, devnull); // Don't print the response 
    curl_easy_setopt(curly, CURLOPT_SSL_VERIFYPEER, 1L); // Verify the CA certificate
    curl_easy_setopt(curly, CURLOPT_SSL_VERIFYHOST, 1L); // Verify that the host reached matches the certificate
    
    DEBUG_LOG(DEBUGGING, __FUNCTION__, "Sending access request with body: %s\n", json_object_to_json_string(json));
    
    DEBUG_LOG(INFO, __FUNCTION__, "Sending access request...");
    res = curl_easy_perform(curly);
    
    if (res != CURLE_OK) {
        DEBUG_LOG(WARNING, __FUNCTION__, "Sending access request failed. Message: %s\n", curl_easy_strerror(res));
        accessResult = -1;
    } else {
        accessResult = processResult(curly);
    }
    
    // Cleanup
    fclose(devnull);
    json_object_put(json);
    curl_easy_cleanup(curly);
    curl_slist_free_all(headers);
    return accessResult;
}
