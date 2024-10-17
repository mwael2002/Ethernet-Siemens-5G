#ifndef ETH_STRUCT_H
#define ETH_STRUCT_H

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


#endif