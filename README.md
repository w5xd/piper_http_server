# piper_http_server
An http server that generates audio using piper1-gpl

This http server extends the product published at https://github.com/OHF-Voice/piper1-gpl. 
The web server published there, https://github.com/OHF-Voice/piper1-gpl/blob/main/docs/API_HTTP.md
has a very usable programming interface, http over tcp, for amateur radio contesting. But
it does not have the performance desired. In particular, it does not support the
http "Connection:keep-alive" header.

This http server publishes an http interface inspired by the one at piper1-gpl, but this
one is not the same. The primary difference is that piper1-gpl supports "Content-type: audio/wav",
while this one supports only "Content-type: audio/pcm".

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
