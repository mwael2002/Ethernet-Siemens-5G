#include "Reading_file.h"



string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to check if a string is hexadecimal
bool isHex(const string& str) {
    return str.length() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
}

// Function to convert hexadecimal string to decimal integer
long long hexToDec(const string& hexStr) {
    long long decimalValue;
    stringstream ss;
    ss << hex << hexStr;
    ss >> decimalValue;
    return decimalValue;
}

// Function to convert string to decimal integer
long long stringToDec(const string& str) {
    long long decimalValue;
    stringstream ss(str);
    ss >> decimalValue;
    return decimalValue;
}

// Function to parse the configuration file and store values in the struct
Eth_config_parms parseConfigFile(const string& filename) {
    ifstream file(filename);
    string line;
    Eth_config_parms config; // Struct to store the parsed values

    if (file.is_open()) {
        int index = 0;
        while (getline(file, line)) {
            size_t pos = line.find("=");
            if (pos != string::npos) {
                string value = trim(line.substr(pos + 1)); // Extract the value part after '='

                // Assign the value to the correct field in the struct based on the order
                switch (index) {
                    case 0:
                        config.Eth_Data_Rate_Gbps = static_cast<unsigned int>(stringToDec(value));
                        break;
                    case 1:
                        config.Eth_Capture_size_ms = static_cast<unsigned int>(stringToDec(value));
                        break;
                    case 2:
                        config.Eth_min_no_IFGs_per_packet = static_cast<unsigned int>(stringToDec(value));
                        break;
                    case 3:
                        config.Eth_Dest_Address = isHex(value) ? hexToDec(value) : stringToDec(value);
                        break;
                    case 4:
                        config.Eth_Src_Address = isHex(value) ? hexToDec(value) : stringToDec(value);
                        break;
                    case 5:
                        config.Eth_max_pkt_size = static_cast<unsigned int>(stringToDec(value));
                        break;
                    case 6:
                        config.Eth_Burst_size = static_cast<unsigned int>(stringToDec(value));
                        break;
                    case 7:
                        config.Eth_Burst_periodicity_us = static_cast<unsigned int>(stringToDec(value));
                        break;
                    default:
                        cerr << "Unexpected line in file." << endl;
                        break;
                }
                index++; // Move to the next field
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file." << endl;
    }

    return config;
}