#pragma once
#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;
struct piper_synthesizer;
class piper_synth_impl {
public:

    virtual ~piper_synth_impl(); // can subclass
    std::string getProperty(const piper_synth::propertyTree_t& names, const std::string& ifMissing);

    void init(const std::string& onnx, const std::string& onnx_json, const std::string& espeak);

    virtual void synth(const std::string& phrase, const boost::property_tree::ptree&, const piper_synth::synth_cb_t& cb);
protected:
    void onnxjson(const std::string& onnx_json_fname);
    std::shared_ptr<piper_synthesizer> m_synth;
    pt::ptree modelTree;

};
