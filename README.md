# piper_http_server
An http server that generates text-to-speech using piper1-gpl

This http server extends the product published at https://github.com/OHF-Voice/piper1-gpl. 
The web server published there, https://github.com/OHF-Voice/piper1-gpl/blob/main/docs/API_HTTP.md
has a usable programming interface, http over tcp, for amateur radio contesting. But
it does not have the performance desired. In particular, it does not support the
http "Connection:keep-alive" header.

The http server here publishes an http interface inspired by that one at piper1-gpl, but is
not the same. One big difference is that piper1-gpl supports "Content-type: audio/wav",
while this one supports only "Content-type: audio/pcm".

This server's intended purpose is localhost only, and with a single
performance-demanding client. It loads only one piper onnx voice model
and only allows one thread to access piper at a time.

<h4>HTTP POST</h4>
The only HTTP command this web server responds to is HTTP Post
which must be accompanied by json data. The json parameters are
<ul>
<li>text<br/>
Required. The value of "text" is the phrase to be spoken.
<li>length_scale
<li>noise_scale
<li>noise_w_scale<br/>
These three parameters are optional, and passed to the piper TTS engine.
</ul>
