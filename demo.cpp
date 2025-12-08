#include <fstream>
#include <string>
#include <piper.h>

int main(int argc, char **argv) {
    std::string onnx = argv[1];
    std::string onnx_json = onnx + ".json";
    std::string espeak = argv[2];

    piper_synthesizer *synth = piper_create(onnx.c_str(),
                                            onnx_json.c_str(),
                                            espeak.c_str());

    // aplay -r 22050 -c 1 -f FLOAT_LE -t raw output.raw
    std::ofstream audio_stream("output.raw", std::ios::binary);

    piper_synthesize_options options = piper_default_synthesize_options(synth);
    // Change options here:
    // options.length_scale = 2;
    // options.speaker_id = 5;

    piper_synthesize_start(synth, "Welcome to the world of speech synthesis!",
                           &options /* NULL for defaults */);

    piper_audio_chunk chunk;
    while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
        audio_stream.write(reinterpret_cast<const char *>(chunk.samples),
                           chunk.num_samples * sizeof(float));
    }

    piper_free(synth);

    return 0;
}
