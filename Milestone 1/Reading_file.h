#ifndef READING_FILE_H
#define READING_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <iomanip>

#include "eth_structs.h"
using namespace std;

string trim(const string& str);
bool isHex(const string& str);
long long hexToDec(const string& hexStr);
long long stringToDec(const string& str);
Eth_config_parms parseConfigFile(const string& filename);

#endif
