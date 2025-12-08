//
// request_handler.cpp
//
// Cribbed from HTTP Server 3 at: https://www.boost.org/doc/libs/latest/doc/html/boost_asio/examples/cpp11_examples.html
//
#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "piper_manager.h"

extern bool diagnosticHttpOut;

namespace http {
namespace server3 {
    namespace pt = boost::property_tree;

    request_handler::request_handler(const std::string& doc_root)
        : doc_root_(doc_root)
    { }

    bool request_handler::handle_request(const request& req, reply& rep, const std::string& content)
    {
        // Decode url to path.
        std::string request_path;
        if (!url_decode(req.uri, request_path))
        {
            rep = reply::stock_reply(reply::bad_request);
            return false;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/'
            || request_path.find("..") != std::string::npos)
        {
            rep = reply::stock_reply(reply::bad_request);
            return false;
        }

        if (request_path != "/")
        {
            rep = reply::stock_reply(reply::not_found);
            return false;
        }

        bool ret = false;
        unsigned contentlength = 0;
        unsigned rateRequested = 0;
        for (auto const h : req.headers)
        {
            if (boost::iequals(h.name, "connection"))
            {
                if (boost::iequals(h.value, "keep-alive"))
                    ret = true;
            }
            else if (boost::iequals(h.name, "Content-Length"))
                contentlength = std::stoi(h.value);
            else if (boost::iequals(h.name, "Accept"))
            {
                std::string rateString = h.value;
                if (rateString.find("audio/pcm") == rateString.npos)
                {
                    rep = reply::stock_reply(reply::no_content);
                    return false;
                }
                auto ratePos = rateString.find("rate=");
                if (ratePos != rateString.npos)
                    rateRequested = atoi(rateString.substr(ratePos + 5).c_str());
            }
        }

        std::string toSpeak;
        pt::ptree reqTree;
        if (!content.empty())
        {
            std::stringstream ss(content.substr(0, contentlength));
            pt::read_json(ss, reqTree);
            toSpeak = reqTree.get<std::string>("text", "");
        }

        unsigned rate = 0;
        if (!toSpeak.empty())
        {
            if (diagnosticHttpOut)
                std::cout << "HTTP POST requests phrase \"" << toSpeak << "\"" << std::endl;
            rep.content.clear();
            auto piper = piper_manager::getGlobalInstance();
            piper_synth::propertyTree_t ratearg({ "audio", "sample_rate" });
            rate = std::stoi(piper.getProperty(ratearg, "16000"));

            if (rateRequested != 0 && rate != rateRequested)
            {
                rep = reply::stock_reply(reply::no_content);
                return false;
            }

            auto& spoken(rep.content);
            piper.synth(toSpeak, reqTree, [&spoken](const float* p, size_t count)
                {
                    // convert float to audio PCM 16 bit signed in this callback
                    typedef short PcmSample_t;
                    static_assert(sizeof(PcmSample_t) == 2, "16 bit sample specified");
                    static const PcmSample_t MAX_AMPLITUDE = 0x7FF8;
                    while (count-- > 0)
                    {
                        auto r = *p++;
                        if (r > 1.0f)
                            r = 1.0f;
                        else if (r < -1.0f)
                            r = -1.0f;
                        PcmSample_t v = static_cast<PcmSample_t>(r * MAX_AMPLITUDE);
                        char* x = reinterpret_cast<char*>(&v);
                        spoken.push_back(*x++);
                        spoken.push_back(*x++);
                    }
                });
        }

        // Fill out the reply to be sent to the client.
        rep.status = reply::ok;
        auto len = rep.content.size();
        rep.headers.resize(len > 0 ? 3 : 2);
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = std::to_string(len);
        rep.headers[1].name = "Connection";
        rep.headers[1].value = ret ? "keep-alive" : "close";
        if (len > 0)
        {
            rep.headers[2].name = "Content-Type";
            std::ostringstream contenttype;
            contenttype << "audio/pcm;rate=" << rate << ";encoding=signed-int;bits=16;channels=1";
            rep.headers[2].value = contenttype.str();
        }
        return ret;
    }

    bool request_handler::url_decode(const std::string& in, std::string& out)
    {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i)
        {
            if (in[i] == '%')
            {
                if (i + 3 <= in.size())
                {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value)
                    {
                        out += static_cast<char>(value);
                        i += 2;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else if (in[i] == '+')
            {
                out += ' ';
            }
            else
            {
                out += in[i];
            }
        }
        return true;
    }

} // namespace server3
} // namespace http
