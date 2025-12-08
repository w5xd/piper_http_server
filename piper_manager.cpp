#include "piper_manager.h"
#include "piper_synth_impl.h"
#include <mutex>
/*
** Inside the webserver, use a single global instance of piper
*/
static auto gGlobalMutext = std::make_shared<std::mutex>();

class SingleThreadAccess : public piper_synth_impl
{
public:
    SingleThreadAccess()
    {
    }
    void synth(const std::string& phrase, const boost::property_tree::ptree&t, const piper_synth::synth_cb_t& cb)
    {
        std::unique_lock<std::mutex> l(*gGlobalMutext);
        piper_synth_impl::synth(phrase, t, cb);
    }
    ~SingleThreadAccess()
    {
    }
};
static std::shared_ptr<SingleThreadAccess> gGlobalInstance;

void piper_manager::initialize(const std::string& modelfile, const std::string& modeljson, const std::string& espeak)
{
    uninitialize();
    auto p = std::make_shared<SingleThreadAccess>();
    p->init(modelfile, modeljson, espeak);
    gGlobalInstance = p;

}
void piper_manager::uninitialize()
{
    gGlobalInstance.reset();
}

piper_synth piper_manager::getGlobalInstance()
{
    return(piper_synth(gGlobalInstance));
}
