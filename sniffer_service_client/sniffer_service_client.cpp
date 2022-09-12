#include <android/binder_manager.h>
#include <aidl/devtitans/sniffer/ISniffer.h>
#include <aidl/devtitans/sniffer/NetworkInfo.h>
#include <iostream>
#include <errno.h>

int main()
{
    std::shared_ptr<aidl::devtitans::sniffer::ISniffer> service;
    service = aidl::devtitans::sniffer::ISniffer::fromBinder(ndk::SpAIBinder(AServiceManager_getService("devtitans.sniffer.ISniffer/default")));

    if (!service)
    {
        std::cout << "Error on accessing service!" << std::endl;
        return EXIT_FAILURE;
    }

    aidl::devtitans::sniffer::NetworkInfo _aidl_return;
    ndk::ScopedAStatus status = service->get_network_info(&_aidl_return);
    aidl::devtitans::sniffer::NetworkInfo network_info = static_cast<aidl::devtitans::sniffer::NetworkInfo>(_aidl_return);
    std::cout << "network info: SSID -> " << network_info.ssid << "RSSI -> " << network_info.rssi << std::endl;

    return EXIT_SUCCESS;
}