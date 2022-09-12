#pragma once

#include <android-base/logging.h>
#include <android/binder_process.h>
#include <android/binder_manager.h>

#include <aidl/devtitans/sniffer/BnSniffer.h>

#include "sniffer_lib.h"

using namespace devtitans::sniffer_lib;

namespace aidl::devtitans::sniffer
{

    class SnifferService : public aidl::devtitans::sniffer::BnSniffer
    {

    public:
        ndk::ScopedAStatus connect(int32_t *_aidl_return) override;
        ndk::ScopedAStatus get_network_info(aidl::devtitans::sniffer::NetworkInfo *_aidl_return) override;

    private:

        int default_rssi = 0;
        std::string default_ssid = "UNKNOWN";

        Sniffer sniffer;

        aidl::devtitans::sniffer::NetworkInfo parseString(std::string info);

        int get_default_rssi();
        std::string get_default_ssid();
    };
}