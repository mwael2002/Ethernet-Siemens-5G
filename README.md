# Ethernet-Siemens-5G
## Milestone 1
### Generate a stream of Ethernet packets and write them to a text file, with the packet format specified.
### Packets are generated for a defined streaming duration.
### Read packet configurations from a text file, and send packets in bursts.
### Bursts are sent periodically until the streaming duration ends.
### Packets must be 4-byte aligned, meaning the Ethernet packet's preamble must start at multiples of 4 bytes (use IFGs as padding to align the packets).
### If the period or total streaming duration ends while a packet is being generated, the packet is discarded, and IFGs are sent instead.
### The program accepts two arguments: 1. Configuration file path (relative or absolute) and 2. Output file name.
### Packets are dumped into the output file in a 4-byte aligned format.
## Milestone 2
### The Ethernet payload must include eCPRI.
### The eCPRI payload should contain the ORAN user-plane data.
### Refer to the ORAN standard for any required information.
### Include IQ samples in the ORAN user-plane packet.
### An input file will be provided with formatted IQ samples.
### If additional IQ samples are needed and the input file data is insufficient, loop through the file again.
