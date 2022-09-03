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
        ndk::ScopedAStatus get_network_info(std::string *_aidl_return) override;

    private:
        Sniffer sniffer;
    };
}