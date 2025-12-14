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
    std::cout << "Usage: piper_http_server [-dDIAG] <model-file> <espeak-dir> [-a <address>] [-p <port>] [-n <num threads>] " << std::endl;
}

typedef std::function<bool(const std::string &, const std::string&)> argCompare_t;
class ArgCompare 
{   public:
        ArgCompare(const std::string &cmp, std::string &toAssign) : toAssign(toAssign), toCompare(cmp){}
        bool operator ()(const std::string& a1, const std::string& a2)  const  {
            if (a1 == toCompare) { toAssign = a2;     return true; }
            return false;
        }
    protected:  const std::string toCompare;  std::string &toAssign;
};

int main(int argc, char** argv) {

std::cout << 
"This is the piper_http_server by Wayne Wright, W5XD.\n"
"See https://github.com/w5xd/piper_http_server\n"
;

#if defined(SIMULATE_PIPER)
std::cout << "This server serves HTTP requests but does not actually call piper. For debugging only.\n";
#endif

    std::string port = "5000";
    std::string address = "0.0.0.0";
    std::string numThreads = "2";
    std::vector<std::string> args;

    {
        std::vector<argCompare_t> argCompare;
        argCompare.push_back(ArgCompare("-p", port));
        argCompare.push_back(ArgCompare("-a", address));
        argCompare.push_back(ArgCompare("-n", numThreads));

        for (int i = 1; i < argc; i++)
        {
            const char *pArg = argv[i];
            if (*pArg != '-')
                args.push_back(argv[i]);
            else
            {
                std::string a;
                std::string b;
                for (;*pArg;pArg++)
                {
                    a += toupper(*pArg);
                    b += *pArg;
                }
                bool ok = true;
                if (a == "-DIAG")
                    diagnosticHttpOut = true;
                else if (i < argc-1)
                {
                    ok = false;
                    for (auto &cmp : argCompare)
                        if (cmp(argv[i], argv[i + 1]))
                        {
                            i += 1;
                            ok = true;
                            break;
                        }
                }
                if (!ok)
                {
                    std::cerr << "Unknown option " << b << std::endl;
                    usage();
                    return 1;
                }
            }
        }

        if ( args.size() != 2)
        {
            usage();
            return 1;
        }
    }
    std::string onnx = args[0];
    std::string onnx_json = onnx + ".json";
    std::string espeak = args[1];
    try {
    static unsigned MAX_THREADS = 10;
        piper_manager::initialize(onnx, onnx_json, espeak);
        auto p = piper_manager::getGlobalInstance();
        std::size_t num_threads = std::min(static_cast<unsigned>(std::stoi(numThreads)), MAX_THREADS);
        http::server3::server s(address, port, "/", num_threads);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
