#include "Eth_config.h"

using namespace std;


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


    return eth_values;

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

