#ifndef CRC_H
#define CRC_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Function declarations
uint32_t calculateCRC32(const vector<uint8_t>& data);
vector<uint32_t> generateCRCTable(void);

#endif
