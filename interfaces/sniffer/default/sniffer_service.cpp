#include "sniffer_service.h"

namespace aidl::devtitans::sniffer
{

    ndk::ScopedAStatus aidl::devtitans::sniffer::SnifferService::connect(int32_t *_aidl_return)
    {

        *_aidl_return = this->sniffer.connect();
        LOG(INFO) << "connect(): " << *_aidl_return;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus aidl::devtitans::sniffer::SnifferService::get_network_info(std::string *_aidl_return)
    {

        *_aidl_return = this->sniffer.get_network_info();
        LOG(INFO) << "get_network_info(): " << *_aidl_return;
        return ndk::ScopedAStatus::ok();
    }

}