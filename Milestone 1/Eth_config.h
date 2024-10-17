#ifndef ETH_CONFIG_H
#define ETH_CONFIG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

#include "eth_structs.h"
#include "config_parms.h"
using namespace std;


// Function declarations
Eth_config_values eth_value_calculation(Eth_config_parms eth_parms);
vector<uint8_t> packet_formation(Eth_config_parms eth_parms, Eth_config_values eth_values);
#endif
