#include "Writing_file.h"

void write_txt_file(vector<uint8_t> packet_unit,Eth_config_parms eth_parms,Eth_config_values eth_values){


    ofstream outFile("output.txt");
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    int m=0;
    for (unsigned int i = 0; i < eth_values.Eth_total_no_bursts; i++) {
        // Add "0x" at the start of every new line
        for (unsigned int j = 0; j < eth_parms.Eth_Burst_size; j++)
        {

        for (unsigned int k = 0; k < eth_parms.Eth_max_pkt_size; k++)
        {

        if (m == 0) {
            outFile << "0x";
        }

        // Write each byte in two-digit hex format
        outFile << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(packet_unit[k]);
        m++;

        // Add newline after every 8 bytes
        if (m  == 4) {
            outFile << endl;
            m=0;
        }

       }

        for (unsigned int k = 0; k < eth_values.Eth_no_IFGs_per_packet_after_alignment; k++)
        {

        if (m == 0) {
            outFile << "0x";
        }

        // Write each byte in two-digit hex format
        outFile << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(IFG_value);
        m++;

        // Add newline after every 8 bytes
        if (m  == 4 ) {
            outFile << endl;
            m=0;
        }
       }


    }
    }

    for (unsigned int k = 0; k < eth_values.Eth_redundant_no_IFGs_after_transmission; k++)
        {

        if (m == 0) {
            outFile << "0x";
        }

        // Write each byte in two-digit hex format
        outFile << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(IFG_value);
        m++;

        // Add newline after every 8 bytes
        if (m  == 4 ) {
            outFile << endl;
            m=0;
        }
       }

    outFile.close();


}
