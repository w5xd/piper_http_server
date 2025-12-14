// piper_cpp_http_server.cpp : Defines the entry point for the application.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "piper_manager.h"
#include "server.hpp"

bool diagnosticHttpOut = false;


static void usage()
{
    std::cout << "Usage: piper_http_server [-dDIAG] <model-file> <espeak-dir> [<port>] [<num threads>] " << std::endl;
}

int main(int argc, char** argv) {

std::cout << 
"This is the piper_http_server by Wayne Wright, W5XD.\n"
"See https://github.com/w5xd/piper_http_server\n"
;

#if defined(SIMULATE_PIPER)
std::cout << "This server serves HTTP requests but does not actually call piper. For debugging only.\n";
#endif

    std::vector<std::string> args;
    for (unsigned i = 1; i < static_cast<unsigned>(argc); i++)
    {
        const char *pArg = argv[i];
        if (*pArg != '-')
            args.push_back(argv[i]);
        else
        {
            std::string a;
            for (;*pArg;pArg++)
                a += toupper(*pArg);
            if (a == "-DIAG")
                diagnosticHttpOut = true;
            else
            {
                std::cerr << "Unknown option " << a << std::endl;
                usage();
                return 1;
            }
        }
    }

    if (args.size() < 2 || args.size() > 4)
    {
        usage();
        return 1;
    }
    std::string onnx = args[0];
    std::string onnx_json = onnx + ".json";
    std::string espeak = args[1];
    std::string port = "5000";
    std::string address = "0.0.0.0";
    if (args.size() > 2)
        port = args[2];
    std::string numThreads = "1";
    if (args.size() > 3)
        numThreads = args[3];
    try {
        piper_manager::initialize(onnx, onnx_json, espeak);
        auto p = piper_manager::getGlobalInstance();
        std::size_t num_threads = std::stoi(numThreads);
        http::server3::server s(address, port, "/", num_threads);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
    return 0;
}
