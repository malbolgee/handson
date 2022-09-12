#include "sniffer_service.h"

namespace aidl::devtitans::sniffer
{

    ndk::ScopedAStatus aidl::devtitans::sniffer::SnifferService::connect(int32_t *_aidl_return)
    {

        *_aidl_return = this->sniffer.connect();
        LOG(INFO) << "connect(): " << *_aidl_return;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus aidl::devtitans::sniffer::SnifferService::get_network_info(aidl::devtitans::sniffer::NetworkInfo *_aidl_return)
    {
        *_aidl_return = this->parseString(this->sniffer.get_network_info());
        return ndk::ScopedAStatus::ok();
    }

    aidl::devtitans::sniffer::NetworkInfo aidl::devtitans::sniffer::SnifferService::parseString(std::string info)
    {

        LOG(INFO) << "parseString(): " << info;

        aidl::devtitans::sniffer::NetworkInfo network_info;

        if (info == "NULL")
        {
            network_info.ssid = this->get_default_ssid();
            network_info.rssi = this->get_default_rssi();
            return network_info;
        }


        // TODO: fow now, let's leave this parsing as it is, but we might
        // want to make it more robust to accept more than two arguments in the
        // response string.
        std::size_t found = info.find_last_of(" ");

        network_info.ssid = info.substr(0, found);
        network_info.rssi = stoi(info.substr(found + 1));

        return network_info;
    }

    int aidl::devtitans::sniffer::SnifferService::get_default_rssi() {
        return this->default_rssi;
    }

    std::string aidl::devtitans::sniffer::SnifferService::get_default_ssid() {
        return this->default_ssid;
    }

}