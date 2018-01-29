#ifndef PKT_H
#define PKT_H
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>

#define PROBE_RESPONSE 0x50
#define PROBE_REQUEST 0x40
#define BEACON_FRAME 0x80
#define DATA_FRAME 0x08


////////////////////////////////////////////////////////////////////
typedef struct info_stored{
    uint8_t             bssid[6];
    uint8_t             channel_power;
    uint8_t             channel;
    char              essid[32 + 1];
}info;

typedef struct ieee80211_radiotap_header {

    uint8_t it_version;
    uint8_t it_pad;

    uint16_t it_len; // this is in use

    uint32_t it_present;
    uint32_t it_present_second;

    uint8_t flags;
    uint8_t data_rate;

    uint16_t channel_frequency; // this is in use
    uint16_t channel_flags;

    uint8_t ssi_signal; // this is in use
}radioTap;

typedef struct ieee80211_default_header {

    uint8_t 	type;
    uint8_t     frame_control_field;
    uint16_t    duration;
    uint8_t     addr1[6];
    uint8_t     addr2[6];
    uint8_t     addr3[6];
    uint16_t 	fs_number;

}ie_de_head;

typedef struct ieee80211_tkip { /* data frame */
    uint8_t	 tkip[8];
}ie_frame_data_tkip;

typedef struct fixed_paprmeters {		/*probe response, beacon */

    uint8_t 	timestamp[8];
    uint16_t	beacon_interval;
    uint16_t	capability_info;

}ie_frame_fixedPara;

typedef struct ieee80211_802_management_frame {

    uint8_t tag_number; // if is it 0 then essid
    uint8_t SSID_length;

}ie_frame_mgmt;

//////////////////////////////////////////////////////////////////////////

class my_packet{


public:
    my_packet(const u_char *packet);

    radioTap *RT;
    ie_de_head *ie_header;
    ie_frame_data_tkip *tkip;
    ie_frame_fixedPara *fixedPara;
    ie_frame_mgmt *ie_frame;

    int flag = 0;
};

#endif // PKT_H
