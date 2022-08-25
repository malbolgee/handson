#include "sniffer_lib.h"

namespace devtitans::sniffer_lib
{

    int Sniffer::connect()
    {

        struct stat dir_stat;

        if (stat(this->get_kernel_directory(), &dir_stat) == 0)
            if (S_ISDIR(dir_stat.st_mode))
                return DEVICE_IS_CONNECTED;

        bool allow_simulated = android::base::GetBoolProperty("devtitans.sniffer.allow_simulated", true);

        if (!allow_simulated)
            return DEVICE_NOT_FOUND;
        else
            return DEVICE_IS_SIMULATED;
    }

    int Sniffer::readFileValue(std::string file)
    {

        int connected = this->connect();

        if (connected == DEVICE_IS_SIMULATED)
        {
            if (file == "rssi")
                return this->get_rssi_simulated();
        }
        else if (connected == DEVICE_IS_CONNECTED)
        {

            int value;
            const std::string file_name = std::string(this->get_kernel_directory()) + file;
            std::ifstream file(file_name);

            if (file.is_open())
            {
                file >> value;
                file.close();
                return value;
            }
        }

        return -1;
    }

    int Sniffer::get_rssi_value()
    {
        return this->readFileValue(this->get_rssi_name());
    }

    char *Sniffer::get_kernel_directory()
    {
        return this->kernel_dir_path;
    }

    std::string Sniffer::get_rssi_name()
    {
        return this->rssi_name;
    }

    int Sniffer::get_rssi_simulated()
    {
        return this->rssi_simulated;
    }

    int Sniffer::get_rssi_random()
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist100(0,100);
        return (int) dist100(rng) * -1;
    }

}