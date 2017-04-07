
#include <Types.h>
#include <DecodedPacket.h>
#include <EncodedPacket.h>

/**
 * Unit test class for the EncodedPacket and DecodedPacket classes
 * Test cases:
 *    1 - Test data buffer larger than 0, smaller than max size, no flag in data
 *    2 - Test data buffer larger than 0, smaller than max size, with one flag in data
 *    3 - Test full data buffer, with at least one flag in data
 *    4 - Test empty data buffer
 */

static
bool test(uint16_t testNum, uint8_t* data, uint16_t dataSize) {
    EncodedPacket e(OPCODE_DATA, 0x0001, data, dataSize);
    
    printf("===========================\n");
    printf("          TEST %d\n", testNum);
    printf("===========================\n");
    
    printf("Input bytes:\n");
    for (uint16_t i = 0; i < dataSize; i++) {
        printf("0x%02X ", data[i]);
        if ((i > 0) && i%0xF == 0) printf("\n");
    }
    printf("\n");
    
    uint16_t size = e.getSize();
    uint8_t encodedBytes[size];
    e.getBytes(encodedBytes);
    
    printf("Encoded bytes:\n");
    for (uint16_t i = 0; i < size; i++) {
        printf("0x%02X ", encodedBytes[i]);
        if (i > 0 && i%0xF == 0) printf("\n");
    }
    printf("\n");
    
    bool matching = true;
    
    DecodedPacket d(encodedBytes, size);
    printf("Decoded bytes:\n");
    for (uint16_t i = 0; i < d.getSize(); i++) {
        printf("0x%02X ", d.getByteAt(i));
        if ((i > 0) && i%0xF == 0) printf("\n");
        matching &= (d.getByteAt(i) == data[i]);
    }
    printf("\n");
    printf("--Output matches input: %s\n\n", (matching ? "true" : "false"));
    
    return matching;
}

int main(void) {
    bool allResult = true;
    
    uint8_t test1[] = { 0, 1, 2, 3, 4, 5};
    allResult &= test(1, test1, 6);
    
    uint8_t test2[] = { 0, 1, 2, 3, PACKET_FLAG, 4};
    allResult &= test(2, test2, 6);
    
    uint8_t test3[254];
    for (uint8_t i = 0; i < 254; i++) { test2[i] = i; }
    // Inject two more occurences of flag
    test3[23] = PACKET_FLAG;
    test3[230] = PACKET_FLAG;
    allResult &= test(3, test3, 254);
    
    uint8_t test4[0];
    allResult &= test(4, test4, 0);
    
    printf("===========================\n");
    printf(" Final result: %s\n", (allResult ? "pass" : "fail"));
    printf("===========================\n");
    
    return 0;
}

