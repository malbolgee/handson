#include "sniffer_client.h"

namespace devtitans::sniffer_client
{

    void SnifferClient::start(int argc, char **argv)
    {

        std::cout << "Client Sniffer!" << std::endl;

        if (argc < 2)
        {

            std::cout << "Syntax: " << argv[0] << " " << std::endl;
            std::cout << "    Commands: get-rssi" << std::endl;
            exit(1);
        }

        sniffer_lib::Sniffer sniffer;

        if (!strcmp(argv[1], "get_rssi"))
            std::cout << sniffer.get_rssi_value() << std::endl;
        else if (!strcmp(argv[1], "get_rssi_random"))
            std::cout << sniffer.get_rssi_random() << std::endl;
        else
            std::cout << "Invalid command." << std::endl;
    }

}

using namespace devtitans::sniffer_client;

int main(int argc, char **argv)
{

    SnifferClient client;
    client.start(argc, argv);
    return 0;
}