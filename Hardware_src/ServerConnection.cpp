
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

#include <iomanip>
#include <json-c/json.h>
#include <Debug.h>
#include <ServerConnection.h>

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
    CURLcode res;

    // Do a simple HTTP GET request to verify the server is online
    curl_easy_setopt(this->curl, CURLOPT_URL, this->getURL());
    curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->curl, CURLOPT_NOBODY, 1); // Disable printing
    curl_easy_setopt(this->curl, CURLOPT_TIMEOUT, 5L); // Give the server 5 seconds to respond
    res = curl_easy_perform(this->curl);

    if (res != CURLE_OK) {
        DEBUG_LOG(WARNING, __FUNCTION__, "Server %s does not appear to be online. Message: %s\n", this->getURL(), curl_easy_strerror(res));
        result = -1;
    } else {
        result = 0;
    }
    curl_easy_cleanup(this->curl);
    
    return result;
}

void ServerConnection::openConnection()
{
    curl_global_init(CURL_GLOBAL_ALL);
    
    this->curl = curl_easy_init();
    
    if (!this->curl)
    {
        DEBUG_LOG(ERROR, __FUNCTION__, "Error initializing cURL.");
    }
}

void ServerConnection::closeConnection()
{
    curl_global_cleanup();
}

static
void generateSecret(uint8_t* buffer, uint16_t buffLen)
{
    int fd = open("/dev/random", O_RDONLY);
    read(fd, buffer, buffLen);
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

static
int8_t processResult(CURL* session)
{
    long http_code = 0;
    curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_code);
    
    if (http_code == 401)
    {
        printf("XX DENID\n");
        return -1;
    } else if (http_code == 200) {
        return 1;
    } else {
        return -1;
    }
}

int8_t ServerConnection::requestAccess(uint32_t lock_id, std::vector<std::string> moduleIDs, std::vector<AuthenticationToken*> tokens)
{
    // cURL stuff
    CURLcode res;
    
    // Query stuff
    std::string url = this->url + access_table_name;
    std::stringstream queryString;
    uint8_t secret[256];
    std::string lock_key;
    
    // JSON stuff
    json_object* json;

    // Initialization
    curl = curl_easy_init();
    json = json_object_new_object();
    
    generateSecret(secret, sizeof(secret));
    lock_key.reserve(sizeof(secret)*2);
    byteArrayToHexString(secret, sizeof(secret), &lock_key);

    // Generate HTTP header
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Generate query string
    queryString << "?lock_id=";
    queryString << std::hex << lock_id;
    queryString << "&lock_key=";
    queryString << lock_key;
    url = url + queryString.str();

    // Generate JSON data for module : token info
    json = json_object_new_array();
    for (uint8_t i = 0; i < moduleIDs.size(); i++) {
        json_object* token;
        std::string* tokenStr = tokens.at(i)->toString();
        
        token = json_object_new_object();
        json_object_object_add(token, "type", json_object_new_string(moduleIDs.at(i).c_str()));
        json_object_object_add(token, "value", json_object_new_string(tokenStr->c_str()));
        json_object_array_add(json, token);
        delete(tokenStr);
    }
    printf("Post body in json: %s\n", json_object_to_json_string(json));
    
    FILE *devnull = fopen("/dev/null", "w+");
    
    curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, json_object_to_json_string(json));
    curl_easy_setopt(this->curl, CURLOPT_TIMEOUT, 5L); // Give the server 5 seconds to respond
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, devnull); // Don't print the response 
    res = curl_easy_perform(this->curl);
    res = CURLE_OK;
    
    if (res != CURLE_OK) {
        DEBUG_LOG(WARNING, __FUNCTION__, "Sending access request failed. Message: %s\n", url.c_str(), curl_easy_strerror(res));
    } else {
        processResult(this->curl);
    }
    
    fclose(devnull);
    json_object_put(json);
    curl_easy_cleanup(this->curl);
    curl_slist_free_all(headers);
    return 0;
}
