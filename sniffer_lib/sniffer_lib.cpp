#include "sniffer_lib.h"

namespace devtitans::sniffer_lib
{

    int Sniffer::connect()
    {

        struct stat dir_stat;

        if (stat(this->kernel_dir_path_char, &dir_stat) == 0)
            if (S_ISDIR(dir_stat.st_mode))
                return DEVICE_IS_CONNECTED;

        bool allow_simulated = android::base::GetBoolProperty("devtitans.sniffer.allow_simulated", true);

        if (!allow_simulated)
            return DEVICE_NOT_FOUND;
        else
            return DEVICE_IS_SIMULATED;
    }

    std::string Sniffer::readFileValue(std::string file)
    {

        int connected = this->connect();

        if (connected == DEVICE_IS_SIMULATED)
        {
            if (file == this->network_info_name)
                return this->get_network_info_simulated();
        }
        else if (connected == DEVICE_IS_CONNECTED)
        {

            std::string value;
            const std::string file_name = this->get_kernel_directory() + file;
            std::fstream network_info;

            network_info.open(file_name, std::ios::in);

            if (network_info.is_open())
            {
                std::string line;
                getline(network_info, line);
                network_info.close();
                return line;
            }
        }

        return NULL;
    }

    std::string Sniffer::get_network_info()
    {
        return this->readFileValue(this->get_network_info_name());
    }

    std::string Sniffer::get_kernel_directory()
    {
        return this->kernel_dir_path;
    }

    std::string Sniffer::get_network_info_name()
    {
        return this->network_info_name;
    }

    std::string Sniffer::get_network_info_simulated()
    {
        return this->network_info_simulated;
    }

    std::string Sniffer::get_network_info_random()
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);
        return "malbolge " + std::to_string((dist100(rng)) * (-1));
    }

}