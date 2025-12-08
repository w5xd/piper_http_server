#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <piper.h>
#include "piper_synth.h"
#include "piper_synth_impl.h"

piper_synth_impl::~piper_synth_impl()
{
}

std::string piper_synth_impl::getProperty(const piper_synth::propertyTree_t& names, const std::string& ifMissing)
{
    try {
        auto tr = modelTree;
        for (unsigned i = 0; i < names.size() - 1; i++)
            tr = modelTree.get_child(names[i]);
        return tr.get<std::string>(names.back());
    }
    catch (std::exception&)
    {
        return ifMissing;
    }
}

void piper_synth_impl::onnxjson(const std::string& onnx_json_fname)
{
    pt::read_json(onnx_json_fname, modelTree);
}

#if !defined(SIMULATE_PIPER)

void piper_synth_impl::init(const std::string& onnx, const std::string& onnx_json, const std::string& espeak)
{
    onnxjson(onnx_json);
    m_synth.reset(piper_create(onnx.c_str(),
        onnx_json.c_str(),
        espeak.c_str()), [](piper_synthesizer* p) {piper_free(p); });
}


void piper_synth_impl::synth(const std::string& phrase, const boost::property_tree::ptree&tr, const piper_synth::synth_cb_t& cb)
{
    auto synth = m_synth.get();
    if (synth == 0)
        throw std::runtime_error("piper_synth_impl::synth NULL");
    auto options = piper_default_synthesize_options(synth);
    // Change options here:
    // options.length_scale = 2;
    // options.speaker_id = 5;

    auto v1 = tr.get_optional<float>("length_scale");
    if (v1)
        options.length_scale = v1.get();

    auto v2 = tr.get_optional<float>("noise_scale");
    if (v2)
        options.noise_scale = v1.get();

    auto v3 = tr.get_optional<float>("noise_w_scale");
    if (v3)
        options.noise_w_scale = v1.get();

    piper_synthesize_start(synth, phrase.c_str(),
        &options /* NULL for defaults */);

    piper_audio_chunk chunk;

    while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
        cb(reinterpret_cast<const float*>(chunk.samples),
            chunk.num_samples);
    }
}

#else
struct piper_synthesizer {
    int noop;
};
void piper_synth_impl::init(const std::string& onnx, const std::string& onnx_json, const std::string& espeak)
{
    onnxjson(onnx_json);
    m_synth.reset(new piper_synthesizer(), [](piper_synthesizer* p) {
        delete p;
        });
}
void piper_synth_impl::synth(const std::string& phrase, const boost::property_tree::ptree&tr, const piper_synth::synth_cb_t& cb)
{
    auto synth = m_synth.get();
    if (synth == 0)
        throw std::runtime_error("piper_synth_impl::synth NULL");
    auto v1 = tr.get_optional<float>("length_scale");

    auto v2 = tr.get_optional<float>("noise_scale");

    auto v3 = tr.get_optional<float>("noise_w_scale");

    float f = 0;
    cb(&f, 1);
}
#endif

piper_synth::piper_synth()
{   // m_impl EMPTY. this instance won't work
}

piper_synth::piper_synth(std::shared_ptr<piper_synth_impl> p) : m_impl(p)
{
}

piper_synth::piper_synth(const std::string& onnx, const std::string& onnx_json, const std::string& espeak)
    : m_impl(new piper_synth_impl)
{
    m_impl->init(onnx, onnx_json, espeak);
}

piper_synth::piper_synth(const piper_synth& other)
{
    m_impl = other.m_impl;
}

piper_synth::~piper_synth()
{
}

void piper_synth::synth(const std::string& phrase, const boost::property_tree::ptree&tr, const synth_cb_t& cb)
{
    return m_impl->synth(phrase, tr, cb);
}

piper_synth& piper_synth::operator=(const piper_synth& o)
{
    m_impl = o.m_impl;
    return *this;
}

piper_synth& piper_synth::operator=(std::shared_ptr<piper_synth_impl> p)
{
    m_impl = p;
    return *this;
}

std::string piper_synth::getProperty(const propertyTree_t& names, const std::string& ifMissing)
{
    return m_impl->getProperty(names, ifMissing);
}

