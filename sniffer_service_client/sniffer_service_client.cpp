#include <android/binder_manager.h>
#include <aidl/devtitans/sniffer/ISniffer.h>
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

    std::string _aidl_return;
    ndk::ScopedAStatus status = service->get_network_info(&_aidl_return);
    std::string network_info = static_cast<std::string>(_aidl_return);
    std::cout << "network info: " << network_info << std::endl;

    return EXIT_SUCCESS;
}