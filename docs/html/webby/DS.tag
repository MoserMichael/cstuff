<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>webby.c</name>
    <path>/home/mmoser/archive/cstuff_bak/webby/</path>
    <filename>a00016</filename>
    <includes id="a00017" name="webby.h" local="yes" imported="no">webby.h</includes>
    <includes id="a00018" name="webbyimpl.h" local="yes" imported="no">webbyimpl.h</includes>
    <class kind="struct">tagVIRTUAL_HOST_DEFINITION</class>
    <class kind="struct">tagDATA_SINK_FILTER</class>
    <class kind="struct">tagDATA_SINK_FILTER_CONNECTION_CONTEXT</class>
    <class kind="struct">tagSERVLET_RUNNER_FILTER</class>
    <class kind="struct">SERVLET_FILTER_CONNECTION_CONTEXT</class>
    <member kind="define">
      <type>#define</type>
      <name>RESERVED_FOR_CHUNK_HEADER_SIZE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a44427928ad20e54e7cef524635ff2bc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LAST_CHUNK_NOT_FIRST</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a7f05649123949a8e965870c97010f80e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LAST_CHUNK_NOT_FIRST_SIZE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aab62a761d0efd5b816b635391ca69f8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LAST_CHUNK_FIRST</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a62c1114c9b862ff891296691c7564ac7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LAST_CHUNK_FIRST_SIZE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>acb30928d1b99967ceedfadeebc124151</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_100_CONTINUE_RESPONSE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a2015bcbf6c5384890d22567bba40056d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_100_CONTINUE_RESPONSE_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a5847d13704437f4dddb4ef4671dabb7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_400_BAD_REQUEST</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a080daa68e2273c3375d0adb8a5a5c0ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_400_BAD_REQUEST_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3c18a0d90bc1c9f3b6d7a2d469c10570</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_404_NOT_FOUND</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aa329368df498e3841e168fd0f2487ca9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_404_NOT_FOUND_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ad1a3e36a0d6be4f97e74c772e17db0f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_500_SERVER_ERROR</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a7c23b56c42f83405f572a89b1c562236</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_500_SERVER_ERROR_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aeac2904b4dc3e9bc8665967226a7ad81</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagVIRTUAL_HOST_DEFINITION</type>
      <name>VIRTUAL_HOST_DEFINITION</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a1e17d3c1c46e17e456fab4b91dbbf0fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagDATA_SINK_FILTER</type>
      <name>DATA_SINK_FILTER</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a1ceda15a8ef21d62a0457293b06bc420</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagDATA_SINK_FILTER_CONNECTION_CONTEXT</type>
      <name>DATA_SINK_FILTER_CONNECTION_CONTEXT</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a6d897d88574b6ae75f3f0fb87acd1623</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSERVLET_RUNNER_FILTER</type>
      <name>SERVLET_RUNNER_FILTER</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a143da2e07bf95015273943862a69602d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct SERVLET_FILTER_CONNECTION_CONTEXT</type>
      <name>SERVLET_FILTER_CONNECTION_CONTEXT</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3f314cfd2eb6f901764100ea0e403564</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>DATA_SINK_FILTER_CONNECTION_CONTEXT *</type>
      <name>DATA_SINK_FILTER_CONNECTION_CONTEXT_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aa55c83fd7a299c138c538b20d50e9cb2</anchor>
      <arglist>(void *impl_connection_ctx, BF *out_buf)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DATA_SINK_FILTER_CONNECTION_CONTEXT_free</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3213ef672b34bbb957fb0650ff5b2c85</anchor>
      <arglist>(DATA_SINK_FILTER_CONNECTION_CONTEXT *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_req_header_parsed</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a601da19b20c63ad3bb7ba3a16916b7f0</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_req_data</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>adda20398a8edb8125382ba5001294e58</anchor>
      <arglist>(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_req_completed</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a490c00eaaa1279aa373aa08b63ed572e</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_on_response_header</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a4e52cf5f68c18c32dcad138f7f444a09</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_on_response_data</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a2579884d18879e342c318b215e40e8dc</anchor>
      <arglist>(HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_on_response_complete</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aa71d1b94a4004acfa1d58c9535bea773</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sink_connection_close</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>af77c4d76ab2670314f249617de68fd5d</anchor>
      <arglist>(FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>DATA_SINK_FILTER *</type>
      <name>DATA_SINK_FILTER_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ae2c4f415dacbb1559c04a802a1b1ea85</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>DATA_SINK_FILTER_add_vhost</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a06f7aefbd60aee0524ea808d92fe7ae2</anchor>
      <arglist>(DATA_SINK_FILTER *vhost_filter, const char *host, int port_num, size_t *vhost_idx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>DATA_SINK_FILTER_add_filter_to_vhost</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a8ee2b58fd7b17a9b1c7a79f14f27f9cb</anchor>
      <arglist>(DATA_SINK_FILTER *vhost_filter, size_t vhost_idx, HTTP_FILTER *new_filter)</arglist>
    </member>
    <member kind="function">
      <type>SERVLET_FILTER_CONNECTION_CONTEXT *</type>
      <name>SERVLET_FILTER_CONNECTION_CONTEXT_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a4f9df4e8f2785ac8500896ec74d4de09</anchor>
      <arglist>(SERVLET_RUNNER_FILTER *rfilter)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>servlets_req_header_parsed</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a7d1a1a1862eeb63a0a4f4b7bcc0152c7</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>servlets_req_on_message_data</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a223c0d62cecfa47dcbf0e083b720ffd7</anchor>
      <arglist>(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>servlets_req_finished</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ad84e5be2ed54a5e98bc4c82fd8768926</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>servlets_connection_close</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0dfe75c88868c24edd8f7fb1d1dea130</anchor>
      <arglist>(FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>SERVLET_RUNNER_FILTER *</type>
      <name>SERVLET_RUNNER_FILTER_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ae2e8a500699c277f9d7461f75e15c517</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SERVLET_RUNNER_FILTER_free</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a21cf1ef43e78be908c81fca4e40b539b</anchor>
      <arglist>(SERVLET_RUNNER_FILTER *servlets)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SERVLET_RUNNER_FILTER_add_servlet</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a86c88fca79c44bdc017513e1c03dc45d</anchor>
      <arglist>(SERVLET_RUNNER_FILTER *servlets, HTTP_SERVLET *servlet)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_CONFIG_load</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga107523da95cf41a0ff405131f32277cc</anchor>
      <arglist>(WEBBY_CONFIG *cfg, const char *file)</arglist>
    </member>
    <member kind="function">
      <type>WEBBY *</type>
      <name>WEBBY_init</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga24e0938463669691db76aa3a2aba418e</anchor>
      <arglist>(WEBBY_CONFIG *cfg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_vhost</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga294920c6df652655a63a9cd5c368a09f</anchor>
      <arglist>(WEBBY *server, const char *host, int port_num, size_t *vhost_idx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_filter</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga63d8c8d948713ccf37e92513e926d8b5</anchor>
      <arglist>(WEBBY *server, size_t vhost_idx, HTTP_FILTER *filter)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_servlet</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6bcdf0577f33a202169d2df9fb2f782d</anchor>
      <arglist>(WEBBY *server, HTTP_SERVLET *servlet)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_run</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gad219e92389c7104756b0955e39f395a8</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_shutdown</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga2fff1941b52b1eba9ded25079ecc3c6a</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>http_header_parsed</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a5d270f92a2a1d91a4497f7619892c421</anchor>
      <arglist>(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>http_on_message_data</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a14b4ca97d7155383d1a5c97a27bb4944</anchor>
      <arglist>(HTTP_REQUEST *request, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>http_req_finished</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ada6754658cfa8e6ac72c2f81497ea618</anchor>
      <arglist>(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>WEBBY_CONNECTION *</type>
      <name>WEBBY_new_connection</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga468f7c2a048c111edea76276865b0f11</anchor>
      <arglist>(WEBBY *server, void *implconndata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_connection_data_received</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga80d94011f2e61e29fd041437c6dbdf17</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>WEBBY_connection_close</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga000431a1b99da08f5711e788533ce0e2</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_start</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ga520d40a9fe92a4764f7800a957e74f35</anchor>
      <arglist>(HTTP_servlet_response *resp, int status, const char *mime_type, HTTP_servlet_response_type rtype, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_send</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaed8f8e393fd1515d2f544f590c6b1867</anchor>
      <arglist>(HTTP_servlet_response *resp, void *data, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>BF *</type>
      <name>HTTP_response_get_chunk_buffer</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gac35744637829527c41116ff45b67db37</anchor>
      <arglist>(HTTP_servlet_response *resp, size_t chunk_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_write_chunk</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaba723292382fdd7307c5bd25a0370fe5</anchor>
      <arglist>(HTTP_servlet_response *resp, BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_finish</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gafcfed03fea048f6eab98c8b410bcc52c</anchor>
      <arglist>(HTTP_servlet_response *resp)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>webby.h</name>
    <path>/home/mmoser/archive/cstuff_bak/webby/</path>
    <filename>a00017</filename>
    <class kind="struct">tagFILTER_CONTEXT</class>
    <class kind="union">RDATA</class>
    <class kind="struct">tagHTTP_FILTER</class>
    <class kind="struct">tagHTTP_servlet_request</class>
    <class kind="struct">tagHTTP_servlet_response</class>
    <class kind="struct">tagSERVLET_CONTEXT</class>
    <class kind="struct">tagHTTP_SERVLET</class>
    <class kind="struct">tagWEBBY_CONFIG</class>
    <class kind="struct">tagWEBBY</class>
    <class kind="struct">tagWEBBY_CONNECTION</class>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_PARSER_BUFFER_SIZE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6216ff348213b42b05175fdda1b9398f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagFILTER_CONTEXT</type>
      <name>FILTER_CONTEXT</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga84c94eac8d6cfb5184937e3047eed77f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_INIT</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga074df44cd12ded4ad916672d381edaca</anchor>
      <arglist>)(struct tagHTTP_FILTER *filter)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_FREE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaaed8d741f1f3f1f7d16153f48df23e7d</anchor>
      <arglist>)(struct tagHTTP_FILTER *filter)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_HEADER_PARSED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga27407d3eb9a137407a8432603325b5b0</anchor>
      <arglist>)(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_DATA</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga07fb25acde5fb71c0bbcad748331e462</anchor>
      <arglist>)(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_COMPLETED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaf6f5ee2b34c30cd71ddeccf8a8cc98c8</anchor>
      <arglist>)(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_HEADER</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gace404adfffdcfe00660631c1b74d141e</anchor>
      <arglist>)(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_DATA</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3fc7467c414925853cc55e085bef5ddc</anchor>
      <arglist>)(HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_COMPLETED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3424cf82a3e72ff1a900de9383c52e61</anchor>
      <arglist>)(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_CONNECTION_CLOSE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga1f024d51ed2b1422159315f06fb051d8</anchor>
      <arglist>)(FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_FILTER</type>
      <name>HTTP_FILTER</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga11cd4e823ff1b0e53cd2b74efc0713b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_servlet_request</type>
      <name>HTTP_servlet_request</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga50e6ef5a1a187d05468238619735c67d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_servlet_response</type>
      <name>HTTP_servlet_response</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gab6262910a582a4aefa03806e8a9996ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSERVLET_CONTEXT</type>
      <name>SERVLET_CONTEXT</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gafa5034048e2302d8ac319265bbd7351c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_SERVLET_INIT</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga975f4529ae9b93d034cd2cf641497485</anchor>
      <arglist>)(struct tagHTTP_SERVLET *servlet_ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>void *(*</type>
      <name>HTTP_SERVLET_FREE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gac7c9be16ed3aaf3f9bca4df8940014ec</anchor>
      <arglist>)(struct tagHTTP_SERVLET *servlet_ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>SERVLET_STATUS(*</type>
      <name>HTTP_SERVLET_ACTION</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga8102140c2330dc5e68dde842ffc9382b</anchor>
      <arglist>)(HTTP_servlet_request *request, HTTP_servlet_response *response, SERVLET_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>HTTP_SERVLET_FREE_CONNECTION</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gabc39eec6dfe7967a1adb13ad5f3073c1</anchor>
      <arglist>)(SERVLET_CONTEXT *contextx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_SERVLET</type>
      <name>HTTP_SERVLET</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga27483d4166ac0bb9f9ca255b1e130ee4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagWEBBY_CONFIG</type>
      <name>WEBBY_CONFIG</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga2d5924aae79394b4e89cde3debdc44bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagWEBBY</type>
      <name>WEBBY</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6893c7f44d85f08f5773cadab1d883e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagWEBBY_CONNECTION</type>
      <name>WEBBY_CONNECTION</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga4db9caaa37abab15b82e31e56f5a363a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_servlet_response_type</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gacac342618d827f10f920dd5d548967ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CONNECTION_CLOSE</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efa9661c1d7c74d6eb76b75849ae30b33c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CONTENT_LENGTH</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efa673a78de2e61f5d56936fc551684d6ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CHUNKED</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efacc2acc83b0f66ab290f5f1fe6b940ef7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>SERVLET_STATUS</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga62da94570530db808ca7a449c02142a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_ERROR</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1a0212a1ce85c57b14397e080fe6249e78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_HANDLED</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1afe95521ecb510198d8d464d9e0c5d30e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_IGNORED</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1a8cc8096ba16a0a241e0a97b15b7c548e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_header_parsed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa72af13e42b9712dcd6651373796ebdc</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_data</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga039bb80f347f613afb853cfd74478630</anchor>
      <arglist>(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_completed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga0640b7d009a7e522d8ca1482f521a453</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_header</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga32fc8cf13fe1f891d2dcbb7e50f3b3a7</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_data</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga9f0f204b55f36f27c0fb680faf7e1305</anchor>
      <arglist>(HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_completed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga4418cb00f29e45e927ca4b428358f288</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE Http_version_type</type>
      <name>HTTP_servlet_protocol</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga868212bcf4f0161db9f4394f42b77dec</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE Http_method_type</type>
      <name>HTTP_servlet_method</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gaff7feee0d3c940abd53967613b77a056</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE URI *</type>
      <name>HTTP_servlet_uri</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gacbade50f8391f298f07e964fefff3f60</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>HTTP_servlet_find_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab592944c860cbfb2f349d5d84b044d5d</anchor>
      <arglist>(HTTP_servlet_request *req, const char *header_name)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE STRINGPAIR *</type>
      <name>HTTP_servlet_first_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab855f4f9cfbd19c2b3e1548fe0960634</anchor>
      <arglist>(HTTP_servlet_request *req, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE STRINGPAIR *</type>
      <name>HTTP_servlet_next_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab73da37758e0f60bcdc997f59029a199</anchor>
      <arglist>(HTTP_servlet_request *req, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DBUF *</type>
      <name>HTTP_servlet_data</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga4280d7a9dc1589c751d6fc44b80a9317</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_start</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ga520d40a9fe92a4764f7800a957e74f35</anchor>
      <arglist>(HTTP_servlet_response *resp, int status, const char *mime_type, HTTP_servlet_response_type rtype, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_send</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaed8f8e393fd1515d2f544f590c6b1867</anchor>
      <arglist>(HTTP_servlet_response *resp, void *data, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_finish</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gafcfed03fea048f6eab98c8b410bcc52c</anchor>
      <arglist>(HTTP_servlet_response *resp)</arglist>
    </member>
    <member kind="function">
      <type>BF *</type>
      <name>HTTP_response_get_chunk_buffer</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gac35744637829527c41116ff45b67db37</anchor>
      <arglist>(HTTP_servlet_response *resp, size_t chunk_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_write_chunk</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaba723292382fdd7307c5bd25a0370fe5</anchor>
      <arglist>(HTTP_servlet_response *resp, BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_SERVLET_init</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gad976ddbc939f4eeedd9202e83743a110</anchor>
      <arglist>(HTTP_SERVLET *servlet, HTTP_SERVLET_INIT init_servlet, HTTP_SERVLET_FREE free_servlet, HTTP_SERVLET_ACTION servlet_action, HTTP_SERVLET_FREE_CONNECTION free_connection)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_CONFIG_load</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga107523da95cf41a0ff405131f32277cc</anchor>
      <arglist>(WEBBY_CONFIG *cfg, const char *file)</arglist>
    </member>
    <member kind="function">
      <type>WEBBY *</type>
      <name>WEBBY_init</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga24e0938463669691db76aa3a2aba418e</anchor>
      <arglist>(WEBBY_CONFIG *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_vhost</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga294920c6df652655a63a9cd5c368a09f</anchor>
      <arglist>(WEBBY *server, const char *host, int port_num, size_t *vhost_idx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_filter</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga63d8c8d948713ccf37e92513e926d8b5</anchor>
      <arglist>(WEBBY *server, size_t vhost_idx, HTTP_FILTER *filter)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_servlet</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6bcdf0577f33a202169d2df9fb2f782d</anchor>
      <arglist>(WEBBY *server, HTTP_SERVLET *servlet)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_run</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gad219e92389c7104756b0955e39f395a8</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_shutdown</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga2fff1941b52b1eba9ded25079ecc3c6a</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>WEBBY_CONNECTION *</type>
      <name>WEBBY_new_connection</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga468f7c2a048c111edea76276865b0f11</anchor>
      <arglist>(WEBBY *server, void *implconndata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_connection_data_received</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga80d94011f2e61e29fd041437c6dbdf17</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>WEBBY_connection_close</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga000431a1b99da08f5711e788533ce0e2</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>webbyimpl.h</name>
    <path>/home/mmoser/archive/cstuff_bak/webby/</path>
    <filename>a00018</filename>
    <includes id="a00017" name="webby.h" local="no" imported="no">webby/webby.h</includes>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_new</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gad65c49dee7d03e91bb0986c6cb9a2a63</anchor>
      <arglist>(WEBBY *srv, WEBBY_CONFIG *cfg, void **impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_shutdown</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gaafe98ebbe19ad95eeff61d5e3e843b26</anchor>
      <arglist>(void *impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_run</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga303ba84a7bd2d3a7546babc2e4ee4082</anchor>
      <arglist>(void *impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_send_data</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga0cfa9e0e763976b6d9905076eb9f2fc1</anchor>
      <arglist>(void *conndata, void *data, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_response_completed</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga9590a8c114565d5e3a27c33a4c0b1fbf</anchor>
      <arglist>(void *conndata, int connection_close)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_FILTER</name>
    <title>HTTP_FILTER</title>
    <filename>a00019.html</filename>
    <class kind="struct">tagFILTER_CONTEXT</class>
    <class kind="union">RDATA</class>
    <class kind="struct">tagHTTP_FILTER</class>
    <member kind="typedef">
      <type>struct tagFILTER_CONTEXT</type>
      <name>FILTER_CONTEXT</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga84c94eac8d6cfb5184937e3047eed77f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_INIT</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga074df44cd12ded4ad916672d381edaca</anchor>
      <arglist>)(struct tagHTTP_FILTER *filter)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_FREE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaaed8d741f1f3f1f7d16153f48df23e7d</anchor>
      <arglist>)(struct tagHTTP_FILTER *filter)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_HEADER_PARSED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga27407d3eb9a137407a8432603325b5b0</anchor>
      <arglist>)(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_DATA</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga07fb25acde5fb71c0bbcad748331e462</anchor>
      <arglist>)(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_REQUEST_COMPLETED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaf6f5ee2b34c30cd71ddeccf8a8cc98c8</anchor>
      <arglist>)(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_HEADER</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gace404adfffdcfe00660631c1b74d141e</anchor>
      <arglist>)(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_DATA</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3fc7467c414925853cc55e085bef5ddc</anchor>
      <arglist>)(HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_RESPONSE_COMPLETED</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3424cf82a3e72ff1a900de9383c52e61</anchor>
      <arglist>)(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_FILTER_CONNECTION_CLOSE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga1f024d51ed2b1422159315f06fb051d8</anchor>
      <arglist>)(FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_FILTER</type>
      <name>HTTP_FILTER</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga11cd4e823ff1b0e53cd2b74efc0713b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_header_parsed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa72af13e42b9712dcd6651373796ebdc</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_data</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga039bb80f347f613afb853cfd74478630</anchor>
      <arglist>(HTTP_REQUEST *request, void *data, size_t data_size, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_request_completed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga0640b7d009a7e522d8ca1482f521a453</anchor>
      <arglist>(HTTP_REQUEST *request, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_header</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga32fc8cf13fe1f891d2dcbb7e50f3b3a7</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_data</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga9f0f204b55f36f27c0fb680faf7e1305</anchor>
      <arglist>(HTTP_RESPONSE *response, int is_chunk, RDATA rdata, FILTER_CONTEXT *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>call_next_filter_response_completed</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga4418cb00f29e45e927ca4b428358f288</anchor>
      <arglist>(HTTP_RESPONSE *response, FILTER_CONTEXT *context)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_SERVLET</name>
    <title>HTTP_SERVLET</title>
    <filename>a00020.html</filename>
    <class kind="struct">tagSERVLET_CONTEXT</class>
    <class kind="struct">tagHTTP_SERVLET</class>
    <subgroup>HTTP_servlet_request</subgroup>
    <subgroup>HTTP_servlet_response</subgroup>
    <member kind="typedef">
      <type>struct tagSERVLET_CONTEXT</type>
      <name>SERVLET_CONTEXT</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gafa5034048e2302d8ac319265bbd7351c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_SERVLET_INIT</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga975f4529ae9b93d034cd2cf641497485</anchor>
      <arglist>)(struct tagHTTP_SERVLET *servlet_ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>void *(*</type>
      <name>HTTP_SERVLET_FREE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gac7c9be16ed3aaf3f9bca4df8940014ec</anchor>
      <arglist>)(struct tagHTTP_SERVLET *servlet_ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>SERVLET_STATUS(*</type>
      <name>HTTP_SERVLET_ACTION</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga8102140c2330dc5e68dde842ffc9382b</anchor>
      <arglist>)(HTTP_servlet_request *request, HTTP_servlet_response *response, SERVLET_CONTEXT *context)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>HTTP_SERVLET_FREE_CONNECTION</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gabc39eec6dfe7967a1adb13ad5f3073c1</anchor>
      <arglist>)(SERVLET_CONTEXT *contextx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_SERVLET</type>
      <name>HTTP_SERVLET</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga27483d4166ac0bb9f9ca255b1e130ee4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>SERVLET_STATUS</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga62da94570530db808ca7a449c02142a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_ERROR</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1a0212a1ce85c57b14397e080fe6249e78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_HANDLED</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1afe95521ecb510198d8d464d9e0c5d30e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SERVLET_REQUEST_IGNORED</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gga62da94570530db808ca7a449c02142a1a8cc8096ba16a0a241e0a97b15b7c548e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_SERVLET_init</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gad976ddbc939f4eeedd9202e83743a110</anchor>
      <arglist>(HTTP_SERVLET *servlet, HTTP_SERVLET_INIT init_servlet, HTTP_SERVLET_FREE free_servlet, HTTP_SERVLET_ACTION servlet_action, HTTP_SERVLET_FREE_CONNECTION free_connection)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_servlet_request</name>
    <title>HTTP_servlet_request</title>
    <filename>a00021.html</filename>
    <class kind="struct">tagHTTP_servlet_request</class>
    <member kind="typedef">
      <type>struct tagHTTP_servlet_request</type>
      <name>HTTP_servlet_request</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga50e6ef5a1a187d05468238619735c67d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE Http_version_type</type>
      <name>HTTP_servlet_protocol</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga868212bcf4f0161db9f4394f42b77dec</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE Http_method_type</type>
      <name>HTTP_servlet_method</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gaff7feee0d3c940abd53967613b77a056</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE URI *</type>
      <name>HTTP_servlet_uri</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gacbade50f8391f298f07e964fefff3f60</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>HTTP_servlet_find_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab592944c860cbfb2f349d5d84b044d5d</anchor>
      <arglist>(HTTP_servlet_request *req, const char *header_name)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE STRINGPAIR *</type>
      <name>HTTP_servlet_first_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab855f4f9cfbd19c2b3e1548fe0960634</anchor>
      <arglist>(HTTP_servlet_request *req, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE STRINGPAIR *</type>
      <name>HTTP_servlet_next_header</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>gab73da37758e0f60bcdc997f59029a199</anchor>
      <arglist>(HTTP_servlet_request *req, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DBUF *</type>
      <name>HTTP_servlet_data</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ga4280d7a9dc1589c751d6fc44b80a9317</anchor>
      <arglist>(HTTP_servlet_request *req)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_servlet_response</name>
    <title>HTTP_servlet_response</title>
    <filename>a00022.html</filename>
    <class kind="struct">tagHTTP_servlet_response</class>
    <member kind="typedef">
      <type>struct tagHTTP_servlet_response</type>
      <name>HTTP_servlet_response</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gab6262910a582a4aefa03806e8a9996ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_servlet_response_type</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gacac342618d827f10f920dd5d548967ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CONNECTION_CLOSE</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efa9661c1d7c74d6eb76b75849ae30b33c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CONTENT_LENGTH</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efa673a78de2e61f5d56936fc551684d6ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RESPONSE_CHUNKED</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ggacac342618d827f10f920dd5d548967efacc2acc83b0f66ab290f5f1fe6b940ef7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_start</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ga520d40a9fe92a4764f7800a957e74f35</anchor>
      <arglist>(HTTP_servlet_response *resp, int status, const char *mime_type, HTTP_servlet_response_type rtype, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_send</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaed8f8e393fd1515d2f544f590c6b1867</anchor>
      <arglist>(HTTP_servlet_response *resp, void *data, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_finish</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gafcfed03fea048f6eab98c8b410bcc52c</anchor>
      <arglist>(HTTP_servlet_response *resp)</arglist>
    </member>
    <member kind="function">
      <type>BF *</type>
      <name>HTTP_response_get_chunk_buffer</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gac35744637829527c41116ff45b67db37</anchor>
      <arglist>(HTTP_servlet_response *resp, size_t chunk_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_response_write_chunk</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>gaba723292382fdd7307c5bd25a0370fe5</anchor>
      <arglist>(HTTP_servlet_response *resp, BF *bf)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>WEBBYCFG</name>
    <title>WEBBYCFG</title>
    <filename>a00023.html</filename>
    <class kind="struct">tagWEBBY_CONFIG</class>
    <member kind="typedef">
      <type>struct tagWEBBY_CONFIG</type>
      <name>WEBBY_CONFIG</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga2d5924aae79394b4e89cde3debdc44bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_CONFIG_load</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga107523da95cf41a0ff405131f32277cc</anchor>
      <arglist>(WEBBY_CONFIG *cfg, const char *file)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>WEBBY</name>
    <title>WEBBY</title>
    <filename>a00024.html</filename>
    <class kind="struct">tagWEBBY</class>
    <class kind="struct">tagWEBBY_CONNECTION</class>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_PARSER_BUFFER_SIZE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6216ff348213b42b05175fdda1b9398f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagWEBBY</type>
      <name>WEBBY</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6893c7f44d85f08f5773cadab1d883e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagWEBBY_CONNECTION</type>
      <name>WEBBY_CONNECTION</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga4db9caaa37abab15b82e31e56f5a363a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>WEBBY *</type>
      <name>WEBBY_init</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga24e0938463669691db76aa3a2aba418e</anchor>
      <arglist>(WEBBY_CONFIG *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_vhost</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga294920c6df652655a63a9cd5c368a09f</anchor>
      <arglist>(WEBBY *server, const char *host, int port_num, size_t *vhost_idx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_filter</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga63d8c8d948713ccf37e92513e926d8b5</anchor>
      <arglist>(WEBBY *server, size_t vhost_idx, HTTP_FILTER *filter)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_add_servlet</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga6bcdf0577f33a202169d2df9fb2f782d</anchor>
      <arglist>(WEBBY *server, HTTP_SERVLET *servlet)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_run</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gad219e92389c7104756b0955e39f395a8</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_shutdown</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga2fff1941b52b1eba9ded25079ecc3c6a</anchor>
      <arglist>(WEBBY *server)</arglist>
    </member>
    <member kind="function">
      <type>WEBBY_CONNECTION *</type>
      <name>WEBBY_new_connection</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga468f7c2a048c111edea76276865b0f11</anchor>
      <arglist>(WEBBY *server, void *implconndata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_connection_data_received</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga80d94011f2e61e29fd041437c6dbdf17</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>WEBBY_connection_close</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga000431a1b99da08f5711e788533ce0e2</anchor>
      <arglist>(WEBBY_CONNECTION *connection)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>WEBBYIMPL</name>
    <title>WEBBYIMPL</title>
    <filename>a00025.html</filename>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_new</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gad65c49dee7d03e91bb0986c6cb9a2a63</anchor>
      <arglist>(WEBBY *srv, WEBBY_CONFIG *cfg, void **impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_shutdown</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gaafe98ebbe19ad95eeff61d5e3e843b26</anchor>
      <arglist>(void *impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_run</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga303ba84a7bd2d3a7546babc2e4ee4082</anchor>
      <arglist>(void *impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_send_data</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga0cfa9e0e763976b6d9905076eb9f2fc1</anchor>
      <arglist>(void *conndata, void *data, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>WEBBY_impl_response_completed</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga9590a8c114565d5e3a27c33a4c0b1fbf</anchor>
      <arglist>(void *conndata, int connection_close)</arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>RDATA</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>struct RDATA::@0</type>
      <name>no_chunk</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a47df03eb99f239c14e83ff9527b8a97b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>data</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ab6caef07b1b3c5a8707b0652d6130419</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>data_size</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ab44ea2cc022386dc8af3adb2952355f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct RDATA::@1</type>
      <name>chunk</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a493098fc4beb27d6c267fe2bd649c2de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BF *</type>
      <name>bf</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>af7d7eeafb6d1e97a600fb9521a5a4a1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>chunk_no</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>a55d7c0465c9e3edb77468f63ab23bcdd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SERVLET_FILTER_CONNECTION_CONTEXT</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>HTTP_servlet_request</type>
      <name>request</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ac73b259830b18eb77f26c79356c78d60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_servlet_response</type>
      <name>response</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a610bc014a866a5014d574b7b80f8bf2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBUF</type>
      <name>request_data</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a5943e99590e838a81c9743e51009eacf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBUF *</type>
      <name>buff</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>aa14b3f0925116dad2ea0579eee21bf76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SERVLET_CONTEXT *</type>
      <name>servlet_contexts</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a06620f6d30a015b2341b8b42ed33ddc2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagDATA_SINK_FILTER</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>HTTP_FILTER</type>
      <name>base</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>aa8c43a02047472cd0e6d2b22cb9cfcba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ARRAY</type>
      <name>vhosts</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a9c6f8e0600fbd38e03b0538b0407c3f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VIRTUAL_HOST_DEFINITION</type>
      <name>default_vhost</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a2c0b9e3c6b8d7ab4e32b39247d785c9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WEBBY *</type>
      <name>server</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a66f3bc5acdb299a761b3a4e163636ba6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagDATA_SINK_FILTER_CONNECTION_CONTEXT</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>BF *</type>
      <name>out_buf</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>af3a9d3d154e3d743999c45d4d3ea9714</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_RESPONSE_WRITER</type>
      <name>writer</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a76f71af457f0df30ad77e4709a8816a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>impl_connection_ctx</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a21ea3e61a86610196315cefee2f71897</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>next_request_filter</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a90a9c5702903bcb1f94632ecf6c8828e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagFILTER_CONTEXT</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>connection_ctx</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a67ef86adee752b865e229fcf29b9ed90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagHTTP_FILTER *</type>
      <name>filter</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>ae9d9ee39035e778d3716187778febf40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>next_request_filter_idx</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>aee0b58f4f4d1647af8f01e19a046cdf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>next_response_filter_idx</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a6ca4c6974a625c100a13adfdb710acc4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_FILTER</name>
    <filename>a00006.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>next_request_filter_idx</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a2d64a2f372cb2f22ecc8fdfa53c0e3d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>next_response_filter_idx</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a39dbd3ce3cfb65108dfcdc963edc8a85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_INIT</type>
      <name>filter_free</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ab987d9068f1114e469acaf4ea45e4b97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_FREE</type>
      <name>filter_init</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a07354d3aa5e87b6a432531f0d88bf1f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_REQUEST_HEADER_PARSED</type>
      <name>on_request_header_parsed</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a680109fd10e4d352a38426fb3832154d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_REQUEST_DATA</type>
      <name>on_request_data</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a6acb030789968d9a75fb32f3305c7a3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_REQUEST_COMPLETED</type>
      <name>on_request_completed</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ac171b2bcad117337da86aa482089ca8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_RESPONSE_HEADER</type>
      <name>on_response_header</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a9e06479f6878202fa0b7c47adeed5030</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_RESPONSE_DATA</type>
      <name>on_response_data</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a7505d786cec72de0641211b0da6b5b75</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_RESPONSE_COMPLETED</type>
      <name>on_response_completed</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a68eb80c5a6751584f37e7c57bea85798</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_FILTER_CONNECTION_CLOSE</type>
      <name>on_connection_close</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a4e7c8380d5073fb0596d2ac4950c309f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_SERVLET</name>
    <filename>a00007.html</filename>
    <member kind="variable">
      <type>HTTP_SERVLET_INIT</type>
      <name>init_servlet</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ae6a2bc9b9e2e7f7f00aab9b42ac237b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_SERVLET_FREE</type>
      <name>free_servlet</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aeafcac3e6df66e734b92cc8d3c6e349a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_SERVLET_ACTION</type>
      <name>servlet_action</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ac176bbe2e49c194f8cee4f5a7ad4b855</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_SERVLET_FREE_CONNECTION</type>
      <name>free_connection</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a567b7d99882c8714c6a30ab41de0644a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_servlet_request</name>
    <filename>a00008.html</filename>
    <member kind="variable">
      <type>HTTP_REQUEST *</type>
      <name>request</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a7341bff4e78abe9aa746d6c52e2eaa20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBUF *</type>
      <name>request_data</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a427d50a189aafc2db7b033acf64d69ef</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_servlet_response</name>
    <filename>a00009.html</filename>
    <member kind="variable">
      <type>HTTP_RESPONSE</type>
      <name>response</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>aa55ce344b1ed261af07279c567b08d7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQUEST *</type>
      <name>request</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>acfe932960b95d037da48c87fc27beb79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FILTER_CONTEXT *</type>
      <name>filter_context</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a6807441c52560f55d6016a13404cd641</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>data_sent</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a03492116d30e34715724cc33f5fe52e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>state</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a1e5ec22738cccb12b9b3bdcdd431f168</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BF</type>
      <name>bf</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>aec0c6f890c3a0f47f41c448d609a02aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>chunk_buf</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a6271be8a3acfe1eb86179de567a912f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>chunk_buf_size</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>abf0aa9810763fa4c8edd2d0fbfb1004a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>chunk_no</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a83454d920f7218661b367f6cd9e3f3f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSERVLET_CONTEXT</name>
    <filename>a00010.html</filename>
    <member kind="variable">
      <type>struct tagHTTP_SERVLET *</type>
      <name>servlet</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ac9b7bef690938dfbe412979fd0b499b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>connection_ctx</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ae35983e812eaaa475a89740ab5f59be0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSERVLET_RUNNER_FILTER</name>
    <filename>a00011.html</filename>
    <member kind="variable">
      <type>HTTP_FILTER</type>
      <name>base</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a5b653e293e834b6d2a167533a0ff7b1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ARRAY</type>
      <name>servlets</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a27adf1fb6122697009848353e59d6e08</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagVIRTUAL_HOST_DEFINITION</name>
    <filename>a00012.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>host</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a9a11ee99cbd8cbb85c822bab961e51fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>host_port</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a7e6172f0c4d87cfc3692d85b7a03ebaa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>next_filter_idx</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a765532c34c7267a755f02fc24f31461c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>last_filter_idx</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ad85ba3100f4e65d70573fea4d748441e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagWEBBY</name>
    <filename>a00013.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>impl</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ad0a59cc3f69711b8680cd0f230690a1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FILTER_CONTEXT *</type>
      <name>filter_ctx_layout</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a56aa9a4e875902f86bd47750212b8086</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>filter_ctx_layout_size</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>aa4cb9fe479dec9c565bcd00cea19efac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ARRAY</type>
      <name>filters</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a8f09ca4fc1921b78808787b500de0431</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagSERVLET_RUNNER_FILTER *</type>
      <name>servlet_runner_filter</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a9e287b121c1bc10c198ea22702046392</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagDATA_SINK_FILTER *</type>
      <name>sink_filter</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>af61d4b5e954a75d0d84bd45b10eb4940</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WEBBY_CONFIG *</type>
      <name>cfg</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a96c8c9fff19088327c9ab5a3b7938ccb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagWEBBY_CONFIG</name>
    <filename>a00014.html</filename>
    <member kind="variable">
      <type>SOCKADDR</type>
      <name>listening_address</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ac08ad021d1c64e9eac9c34043087efa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>listen_backlog</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a72e5080306e25c54fc7f881ba9eb2a77</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>socket_buffer_size</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a4119ff3548a22f691d93075d038ea394</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>max_connections</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ab5f7d664cacb890429d2a91bf3d3c981</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>io_timeout</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a3e3dd5f074987dbfaa3ba0d931308191</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>idle_timeout</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a921222dc1522dd04b5c46e157816c839</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>stack_pages_per_thread</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a1c36a6ce7610fcb73a1fdcd36a75dbb0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagWEBBY_CONNECTION</name>
    <filename>a00015.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>in_buf_data</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a9d34617f3451ac32315d1223cdd56a36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BF</type>
      <name>in_buf</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>abe2bc97a98ec04b90f63ace8b4ec22ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQUEST_PARSER</type>
      <name>request_parser</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>aa6ca489e0ea2b39fd9eda1f048da4f3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQUEST</type>
      <name>request</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a0fd47114cffc5512054c650d2e931fee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FILTER_CONTEXT *</type>
      <name>filter_data</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a981014b88d204a0872d9943cd6dccb9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>num_filters</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a432d09002a99af37052a7d40de7db432</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
