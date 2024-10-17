#ifndef WRITING_FILE_H
#define WRITING_FILE_H

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

#include"eth_structs.h"
#include"config_parms.h"

void write_txt_file(vector<uint8_t> packet_unit,Eth_config_parms eth_parms,Eth_config_values eth_values);


#endif
