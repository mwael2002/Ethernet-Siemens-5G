#include "CRC.h"

using namespace std;

vector<uint32_t> generateCRCTable(void) {
    vector<uint32_t> crcTable(256);
    uint32_t polynomial = 0xEDB88320;

    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
        crcTable[i] = crc;
    }
    return crcTable;
}

// Function to calculate CRC-32 from a vector of bytes
uint32_t calculateCRC32(const vector<uint8_t>& data) {
    static vector<uint32_t> crcTable = generateCRCTable(); // Create table once

    uint32_t crc = 0xFFFFFFFF;  // Initial value
    for (const auto& byte : data) {
        uint8_t index = static_cast<uint8_t>(byte ^ (crc & 0xFF));
        crc = (crc >> 8) ^ crcTable[index];
    }
    return crc ^ 0xFFFFFFFF;  // Final XOR value
}
