#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <iomanip>
//#include "Eth_config.h"

using namespace std;

#define Preample_SFD_no_bytes 8
#define Src_Add_no_bytes 6
#define Dest_Add_no_bytes 6
#define FEC_no_bytes 4    
#define Eth_type_no_bytes 2

#define Preample_SFD_value 0xFB555555555555D5
#define Eth_type_value 0xAEFE
#define IFG_value 0x07


typedef struct {
    unsigned int Eth_Data_Rate_Gbps;
    unsigned int Eth_Capture_size_ms;
    unsigned int Eth_min_no_IFGs_per_packet;
    long long int Eth_Dest_Address;
    long long int Eth_Src_Address;
    unsigned int Eth_max_pkt_size;
    unsigned int Eth_Burst_size;
    unsigned int Eth_Burst_periodicity_us;
} Eth_config_parms;

typedef struct {
    unsigned int Eth_total_packet_size_with_IFGs;
    unsigned int Eth_payload_no_bytes;
    unsigned int Eth_total_no_bursts;
    unsigned int Eth_redundant_no_IFGs_after_transmission;
    unsigned int Eth_no_IFGs_per_packet_after_alignment;
    unsigned int Eth_redundant_no_IFGs_per_burst;
} Eth_config_values;


// Function declarations
string trim(const string& str);
bool isHex(const string& str);
long long hexToDec(const string& hexStr);
long long stringToDec(const string& str);
Eth_config_parms parseConfigFile(const string& filename);
Eth_config_values eth_value_calculation(Eth_config_parms eth_parms);
void Eth_Burst_packet_size_calc(Eth_config_parms,Eth_config_values);
uint32_t calculateCRC32(const vector<uint8_t>& data);
vector<uint32_t> generateCRCTable(void);
vector<uint8_t> packet_formation(Eth_config_parms eth_parms, Eth_config_values eth_values);
void write_txt_file(vector<uint8_t> packet_unit,Eth_config_parms eth_parms,Eth_config_values eth_values);

int main() {
    string filename = "first_milestone.txt";

    // Call the parseConfigFile function to get the struct with the parsed values
    Eth_config_parms ethConfig = parseConfigFile(filename);
    Eth_config_values eth_values=eth_value_calculation(ethConfig);

    vector<uint8_t> packet_unit=packet_formation(ethConfig,eth_values);
    uint32_t crc = calculateCRC32(packet_unit);

    vector<uint8_t> crc_vec={(uint8_t) (crc>>3*8),(uint8_t) (crc>>2*8),(uint8_t) (crc>>1*8),(uint8_t) crc};

    vector<uint8_t> packet_unit_with_crc;
    packet_unit_with_crc.reserve(packet_unit.size() + crc_vec.size()); // Reserve space
    packet_unit_with_crc.insert(packet_unit_with_crc.end(), packet_unit.begin(), packet_unit.end());
    packet_unit_with_crc.insert(packet_unit_with_crc.end(), crc_vec.begin(), crc_vec.end());
    
    return 0;
}


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

Eth_config_values eth_value_calculation(Eth_config_parms eth_parms){
    Eth_config_values eth_values;

    unsigned int no_bits_per_burst;
    double time_pkts_per_burst;


    no_bits_per_burst=eth_parms.Eth_max_pkt_size*8*eth_parms.Eth_Burst_size;
    time_pkts_per_burst=no_bits_per_burst/(eth_parms.Eth_Data_Rate_Gbps*(1000000000.0));
    
    double no_of_bursts=(eth_parms.Eth_Capture_size_ms*0.001)/time_pkts_per_burst;
    eth_values.Eth_total_no_bursts=floor(no_of_bursts);
    
    double IFG_time_after_transmission=eth_parms.Eth_Burst_periodicity_us*(0.000001)*(no_of_bursts-eth_values.Eth_total_no_bursts);
    eth_values.Eth_redundant_no_IFGs_after_transmission=floor(IFG_time_after_transmission*eth_parms.Eth_Data_Rate_Gbps*(1000000000.0)/8);

    eth_values.Eth_payload_no_bytes=eth_parms.Eth_max_pkt_size-(Preample_SFD_no_bytes+Src_Add_no_bytes+Dest_Add_no_bytes+FEC_no_bytes+Eth_type_no_bytes);
    

    int rem_IFGs_bytes=(eth_parms.Eth_max_pkt_size+eth_parms.Eth_min_no_IFGs_per_packet)%4;
      
      if(rem_IFGs_bytes==0){
        eth_values.Eth_total_packet_size_with_IFGs = (eth_parms.Eth_max_pkt_size+eth_parms.Eth_min_no_IFGs_per_packet);
        eth_values.Eth_no_IFGs_per_packet_after_alignment=eth_parms.Eth_min_no_IFGs_per_packet;
      }
      else{
        eth_values.Eth_total_packet_size_with_IFGs = (eth_parms.Eth_max_pkt_size+eth_parms.Eth_min_no_IFGs_per_packet)+ (4- rem_IFGs_bytes);
        eth_values.Eth_no_IFGs_per_packet_after_alignment=eth_parms.Eth_min_no_IFGs_per_packet+rem_IFGs_bytes;
      } 

      unsigned int pkts_per_burst=eth_values.Eth_total_packet_size_with_IFGs*eth_parms.Eth_Burst_size;
      eth_values.Eth_redundant_no_IFGs_per_burst=floor(eth_parms.Eth_Burst_periodicity_us*(0.000001)*eth_parms.Eth_Data_Rate_Gbps*(1000000000.0)/8)-pkts_per_burst;
      
      cout<<eth_values.Eth_payload_no_bytes<<endl;
      

    return eth_values;

}

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

vector<uint8_t> packet_formation(Eth_config_parms eth_parms, Eth_config_values eth_values){
    vector<uint8_t> packet(eth_parms.Eth_max_pkt_size-FEC_no_bytes);
    
    unsigned int i;
    
    for(i=0;i<Preample_SFD_no_bytes;i++){

        packet[i]=(Preample_SFD_value>>((Preample_SFD_no_bytes-1-i)*8));

    }

    for(i=0;i<Dest_Add_no_bytes;i++){

        packet[i+Preample_SFD_no_bytes]=(eth_parms.Eth_Dest_Address>>((Dest_Add_no_bytes-1-i)*8));

    }

    for(i=0;i<Src_Add_no_bytes;i++){

        packet[i+Dest_Add_no_bytes+Preample_SFD_no_bytes]=(eth_parms.Eth_Src_Address>>((Src_Add_no_bytes-1-i)*8));

    }


    for(i=0;i<Eth_type_no_bytes;i++){

        packet[i+Dest_Add_no_bytes+Preample_SFD_no_bytes+Src_Add_no_bytes]=(Eth_type_value>>((Eth_type_no_bytes-1-i)*8));

    }

    for(i=eth_parms.Eth_max_pkt_size-eth_values.Eth_payload_no_bytes-1;i<eth_values.Eth_payload_no_bytes;i++){
        
        packet[i]=0;
    
    }   
    
    return packet;

}
