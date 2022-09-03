#include "sniffer_service.h"

using namespace ndk;

int main()
{

    LOG(INFO) << "Launching Sniffer AIDL Service ...";

    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<aidl::devtitans::sniffer::SnifferService> sniffer_service = SharedRefBase::make<aidl::devtitans::sniffer::SnifferService>();
    const std::string instance = std::string() + aidl::devtitans::sniffer::ISniffer::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(sniffer_service->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);

    LOG(INFO) << "Sniffer AIDL Service launching with name: " << instance;
    ABinderProcess_joinThreadPool();

    return EXIT_FAILURE;
}