#pragma once

#include <fstream>
#include <sys/stat.h>
#include <random>
#include <android-base/properties.h>

namespace devtitans::sniffer_lib
{

    class Sniffer
    {

    public:
        const int DEVICE_NOT_FOUND = 0;    /** Indicates that a device could not be found. */
        const int DEVICE_IS_CONNECTED = 1; /** Indicates that a device is connected. */
        const int DEVICE_IS_SIMULATED = 2; /** Indicates that a device is being sumulated. */

        /**
         * @brief Verifies if the directory /sys/kernel/sniffer exists.
         *
         * If this directory exists, then the Sniffer device is fisically connected,
         * otherwise, check the devtitans.sniffer.allow_sumulated property, to
         * see if the sumulated values can be used.
         *
         * @return returns an integer representing a state, that can be either DEVICE_NOT_FOUND,
         *           DEVICE_IS_CONNECTED or DEVICE_IS_SIMULATED.
         */
        int connect();

        /**
         * @brief returns the network info.
         *
         * @return returns a std::string representing the network info (SSID and RSSI).
         */
        std::string get_network_info();

        /**
         * @brief returns a random value for the network info.
         *
         * @return returns a std::string representing the nertwork info (SSID and RSSI).
         */
        std::string get_network_info_random();

    private:
        /** This is the file name created by the kernel module. */
        std::string network_info_name = "network_info";

        /** This is the variable of the simulated value of the network info (SSDI and RSSI). */
        std::string network_info_simulated = "malbolge -1";

        /** Defines the directory contaning the network_info file is located. */
        std::string kernel_dir_path = "/sys/kernel/sniffer/";

        /**  Keeps a char * version of the path for backward compatibility reasons */
        char kernel_dir_path_char[22] = "/sys/kernel/sniffer/";

        /**
         * @brief This method will read the values in the file
         *          located at the /sys/kernel/ directory.
         *
         * @param file is the file path for the network_info
         * @return returns a std::string representing the network info read from the file
         */
        std::string readFileValue(std::string file);

        /** Returns the directory where the kernel module creates the file interface.  */
        std::string get_kernel_directory();

        /** Returns a simulated network info. */
        std::string get_network_info_simulated();

        /** returns the network info file name. */
        std::string get_network_info_name();
    };

}