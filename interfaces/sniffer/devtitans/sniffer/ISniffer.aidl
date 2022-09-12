package devtitans.sniffer;

import devtitans.sniffer.NetworkInfo;

@VintfStability
interface ISniffer {    
    int connect();
    NetworkInfo get_network_info();
}