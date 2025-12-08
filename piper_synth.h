#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <boost/property_tree/ptree.hpp>
class piper_synth_impl;
class piper_synth
{
    public:
        piper_synth(); // empty constructor. this one won't work
        piper_synth(const std::string& modelfile, const std::string& modeljson, const std::string& espeak);
        piper_synth(const piper_synth&);
        piper_synth(std::shared_ptr<piper_synth_impl>);
        piper_synth & operator=(const piper_synth&);
        piper_synth& operator=(std::shared_ptr<piper_synth_impl>);
        ~piper_synth();
        typedef std::function<void(const float *, size_t)> synth_cb_t;
        void synth(const std::string &phrase, const boost::property_tree::ptree &, const synth_cb_t &cb);
        typedef std::vector<std::string> propertyTree_t;
        std::string getProperty(const propertyTree_t &names, const std::string &ifMissing="");
    protected:
        std::shared_ptr<piper_synth_impl> m_impl;
};


