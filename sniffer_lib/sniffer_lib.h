#pragma once

#include <fstream>
#include <android-base/properties.h>
#include <sys/stat.h>
#include <random>

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
         * otherwise, verify the devtitans.sniffer.allow_sumulated property, to
         * see if the sumulated values can be used.
         *
         * @return returns an integer representing a state, that can be either DEVICE_NOT_FOUND,
         *           DEVICE_IS_CONNECTED or DEVICE_IS_SIMULATED.
         */
        int connect();

        /**
         * @brief returns the RSSI value.
         *
         * @return returns an int representing the RSSI value.
         */
        int get_rssi_value();
        
        /**
         * @brief returns a random value for the RSSI.
         * 
         * @return returns an integer representing a random RSSI value.
         */
        int get_rssi_random();

    private:
        std::string rssi_name = "rssi";

        /** This is the variable of the simulated value of the RSSI. */
        int rssi_simulated = -1;

        /** Defines the directory contaning the rssi file is located. */
        char kernel_dir_path[22] = "/sys/kernel/sniffer/";

        /**
         * @brief This method will read the values in the file
         *          in the /sys/kernel/rssi directory.
         *
         * @param file is the file path for the rssi
         * @return returns a integer
         */
        int readFileValue(std::string file);

        /** Returns the directory  */
        char *get_kernel_directory();

        /** Returns the simulated rssi value. */
        int get_rssi_simulated();

        std::string get_rssi_name();
    };

}