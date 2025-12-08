#pragma once
#include "piper_synth.h"
/*piper_http_server has a global instance of piper_synth
** This class is specific to the behavior desired of this particualr http server.
** It is designed to handle only one concurrent client. 
** That client will do keep-alive on its HTTP POST calls.
** But a client can die unexpectedly. Therefore the server is multithreaded, even though
** it can handle only one client. The second thread can connect for the scenario that
** keep-alive has hung the first thread.
**
** This manager limits only one call through piper_synth::synth() because
** a hang is deemed better than a crash if it turns out piper is not re-entrant (this
** author doesn't know)
*/
class piper_manager
{
public:
    static void initialize(const std::string& modelfile, const std::string& modeljson, const std::string& espeak);
    static void uninitialize(); // 
    static piper_synth getGlobalInstance(); // returns functially instance only between initialize and uninitialize
};