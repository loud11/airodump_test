#include "pkt.h"

my_packet::my_packet(const u_char *packet){

    RT = (radioTap *)(packet);

    ie_header = (ie_de_head *)(packet + RT->it_len);

    switch(ie_header->type){

    case BEACON_FRAME: flag = BEACON_FRAME;

    case PROBE_RESPONSE: if(flag == 0) flag = PROBE_RESPONSE;

        ie_frame = (ieee80211_802_management_frame *)(packet + RT->it_len + sizeof(ie_de_head) + sizeof(ie_frame_fixedPara));
        break;

    case DATA_FRAME: flag = DATA_FRAME;

        ie_frame = (ieee80211_802_management_frame *)(packet + RT->it_len + sizeof(ie_de_head) + sizeof(ie_frame_data_tkip));
        break;

    default :
        printf("this is else\n");
    }
}
