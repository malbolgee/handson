#pragma once

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "sniffer_lib.h"

namespace devtitans::sniffer_client
{
    class SnifferClient
    {

    public:
        void start(int argc, char **argv);
    };

}