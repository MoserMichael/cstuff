<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>charclass.h</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00014</filename>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_digit</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a0e785ba5f60fda38d1abd5af67581f04</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_upper_case</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a0c3332312d428ba39cd5728493062454</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_hex</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a5f8d72d8577cc7ff464bcc2a322890f4</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_hex_ext</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ae41c2dfdb3827d25ed2cd83a85c90343</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_alpha</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a3d3ebd9ca48c4ae7abae10d7b9f54763</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_alphanum</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a283b46097df7193a0f62701ef8b65ebb</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hparse.c</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00015</filename>
    <includes id="a00017" name="http.h" local="yes" imported="no">http.h</includes>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parser_content_length_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a94ed21024788a6468bef1254fbcc601a</anchor>
      <arglist>(struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parser_connection_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a40d967c801e7387bf035ca0180cf65c5</anchor>
      <arglist>(struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parser_transfer_encoding_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a60ca36cce17075b298907e33a65e8074</anchor>
      <arglist>(struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parser_expect_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>aacd9120df894136a7ee383efcb4698c3</anchor>
      <arglist>(struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parser_host_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a1afdaf777be1d5dfaab44388faf46531</anchor>
      <arglist>(struct tagHTTP_MESSAGE *message, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>init_parsers_general_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>aed9d9e4b3924953f553c81d8767b9670</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>init_parsers_request_header</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>a8839eee7a29fdaecb57faddfea66620d</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>http.c</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00016</filename>
    <includes id="a00017" name="http.h" local="yes" imported="no">http.h</includes>
    <includes id="a00020" name="sutils.h" local="no" imported="no">sutils.h</includes>
    <includes id="a00014" name="charclass.h" local="no" imported="no">charclass.h</includes>
    <class kind="struct">tagHEADER_HASH_ACTION</class>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_GET</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a5b1419b28ee900c497c9d12a3d55a086</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_GET_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a6039dc3a681b3e19368e2697573956be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_POST</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>af7305bcf03b5d811dc79b837253b1392</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_POST_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a8b08b8d70a27b1c157ac42b39b839522</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_PUT</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0b471f186758a2e5ae3d3ae5ca9e9172</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_PUT_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a9dcd450da87c90b4cfc3455721ad51d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_DELETE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0577b387a60434ff4178b0d5a8a75f73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_DELETE_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>adca36b9ed5af9ed93bd488c3c8df3728</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_HEAD</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a51339cf7ae257dd055af3f07507660ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_HEAD_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>abf39a335866cfd4ae3898f699bb53a2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_OPTIONS</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>adca7a73be3c2f076707f0dfee53d2ee3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_OPTIONS_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aca7b51eb47872d3b0742042951826365</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_TRACE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a01fd12e9de2f85def75e0370e212c3a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_TRACE_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a419bfe90e9a5a493e1c39d96d0ca4b38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_CONNECT</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a55bf7b1c612fef715d52dc12dc90b940</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_METHOD_CONNECT_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a51d6c342d92ab5bc91de12d16270ca0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_START_S</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a00b0ac1e809bca4fb7929dbd137c5a8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_START_S_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a788ad423247009c8ed8797848d27f30b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_EOF_LINE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aa14dc30ffe09bc9349ddf289170c3c15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_EOF_LINE_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a0bcec4a44eab88ca0b2e278aeeb94397</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>to_lower</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a407d07021aeaf63da6145057e7fd7d93</anchor>
      <arglist>(pos)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_HTTP_TOKEN_SIZE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ab1dd5ca9a62c5ae382e84d2993da6498</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_CONNECTION_CLOSE</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3c2b0d5e4dae1a3b6b38e47275dac583</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_CONNECTION_CLOSE_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a36802223fbb5d545dcbb6e4e684ba519</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_TRANSFER_CHUNKED</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aa6f01c37e720b04b0f7271e8077c104f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>S_TRANSFER_CHUNKED_LEN</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ae639266697246d81f6b3a89a700d6071</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RETURN_WRITER_MORE_DATA</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3ee4cd5eec968a38b7280244c5d72bc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHEADER_HASH_ACTION</type>
      <name>HEADER_HASH_ACTION</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aaf0e1bc8c0745e8435c3f478537603ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>init_parsers_request_header</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a8839eee7a29fdaecb57faddfea66620d</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>init_parsers_general_header</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>aed9d9e4b3924953f553c81d8767b9670</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>http_is_ctl</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a2ca5809163b8fa1bb3cb98e92087ba71</anchor>
      <arglist>(char ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>http_is_ctl_or_space</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a4c076db9ce25ed33a2ce16be3dc277aa</anchor>
      <arglist>(char ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>http_is_space</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a079ef1c165906ce8b5bced46a7746680</anchor>
      <arglist>(char ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>http_is_separator</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ac42c9702c6a1547de4493f7f3ef21bc1</anchor>
      <arglist>(char ch)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>free_stringpair</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ae15795ec6bb30bef27be04ac28000dbf</anchor>
      <arglist>(DLISTUNR *list, void *entry, void *context)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_init</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga14d478691c5f29cd966cb8abc4700a1f</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HTTP_MESSAGE_free</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gab01f22c91d577dfae9702308dbd0c518</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_add_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga1ca9f7fea4b5dfd0d084c546a489220e</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name, const char *value)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>HTTP_MESSAGE_find_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga33eb4b3bf28432797029c90115dd80d7</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_first_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga225a9e18b90020b71906c943ef3bcde3</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_next_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga405406f091c3d4236fc340910391cc87</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>hash_compare</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a3fcdd47ea95ee4ef2da80d5e1f4c250f</anchor>
      <arglist>(HASH_Entry *entry, void *key, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa5370897809bd47280b44b5fa679d2d3</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_free</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaf7d9eeee759da719084291a7cd155c94</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_add_header_parser</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gae0af51ac516c4e68c9d7ba843fbea541</anchor>
      <arglist>(HTTP_PARSER *parser, const char *header_name, HEADER_ACTION action_func)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_get_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac5e21a738d73d75ebec125c97038961a</anchor>
      <arglist>(BF *bf, char **start_line)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_get_header_token</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8d14d1a7cd91bbd477872efafe8e9962</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_parse_header_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa737a4c11979880764fb8031d5dfc488</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *request, BF *bf, int *eof_header)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_content_length_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8b4ee246e968497579a9d9d194a92571</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_content_length_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga34a73b6fe4c943ce1831c8b93c7fb426</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_chunked_data_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga0cfd781757cd6db4812087df78424fda</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_PARSER_chunked_data_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac93ea444be8644a86692ae98ec0b86c6</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PARSER_STATUS</type>
      <name>parse_request_line</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a6dbb89ef5afb7d9640fad8c33c6cda21</anchor>
      <arglist>(HTTP_REQUEST *request, BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_PARSER_init</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>gaf0a986ea0582bfc4b011d2ac61aac9ca</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQ_HEADER_PARSED header_parsed, HTTP_REQ_MESSAGE_BODY_DATA on_message_body_data, HTTP_REQ_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_REQUEST_PARSER_process</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>ga84afcea80bc8b534b3eee27594d2afbd</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQUEST *request, BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_is_persistent</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gaa3893b9709b438c414da743d2e9ba285</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PARSER_STATUS</type>
      <name>parse_response_line</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>af2d54b915c5c209b05dfeb84b4dd640d</anchor>
      <arglist>(HTTP_RESPONSE *response, BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_RESPONSE_PARSER_init</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ga00451ddae893b0ea2fb035d9cd1b319a</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESP_HEADER_PARSED header_parsed, HTTP_RESP_MESSAGE_BODY_DATA on_message_body_data, HTTP_RESP_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_PARSER_process</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>gac69a62184875abe24177d14f0a9a0313</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESPONSE *response, BF *bf)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const char *</type>
      <name>http_version_to_str</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a21309e0c6192fd4b8a34e3aabf7ccad7</anchor>
      <arglist>(Http_version_type ver)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const char *</type>
      <name>get_reason_phrase</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ae3f621842d3c96a248f1a1d6a7b21d9e</anchor>
      <arglist>(int code)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_WRITER_write</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga52fcf93eeb1c013427abde12a234adff</anchor>
      <arglist>(HTTP_RESPONSE_WRITER *writer, BF *bf)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>http.h</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00017</filename>
    <includes id="a00022" name="uri.h" local="no" imported="no">hutils/uri.h</includes>
    <class kind="struct">tagSTRINGPAIR</class>
    <class kind="struct">tagHTTP_MESSAGE</class>
    <class kind="struct">tagHTTP_REQUEST</class>
    <class kind="struct">tagHTTP_RESPONSE</class>
    <class kind="struct">tagHTTP_PARSER</class>
    <class kind="struct">tagHTTP_REQUEST_PARSER</class>
    <class kind="struct">tagHTTP_RESPONSE_PARSER</class>
    <class kind="struct">tagHTTP_RESPONSE_WRITER</class>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_MESSAGE_FLAG_CONNECTION_CLOSE</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>a0a66a9905aa743b7f731f1c5f1f9e981</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_MESSAGE_FLAG_TRANSFER_CHUNKED</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ac8a5bb01ca6fedc97a5a5377397f5a84</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_MESSAGE_FLAG_HAS_CONTENT_LENGTH</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>a091dd3c50642a1632d85fe2d552d4a69</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_MESSAGE_FLAG_KEEPALIVE</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ac75f76df3b41c35d459bbfc50cec3123</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTRINGPAIR</type>
      <name>STRINGPAIR</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga8118b527e2c0aaba786f406681e80b65</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_MESSAGE</type>
      <name>HTTP_MESSAGE</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gab956fd255b384bed00c5015cd1a6ee14</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HEADER_ACTION</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gaef3a97b256b09f6a4ef1527ead10d52f</anchor>
      <arglist>)(struct tagHTTP_MESSAGE *request, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="typedef">
      <type>enum tagHttp_method_type</type>
      <name>Http_method_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gac5182036656d3198819f12ef9ec9d08b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tagHttp_version_type</type>
      <name>Http_version_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga06dc534889d1806e0448e56dd9757dae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_REQUEST</type>
      <name>HTTP_REQUEST</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga7548b2c494ccf7acc059409a5d21ccaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE</type>
      <name>HTTP_RESPONSE</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gac0d52817d9795fea81e6cfa5afa369e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_HEADER_PARSED</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gab50a0c614e6c5e0b1364121270be9076</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_MESSAGE_BODY_DATA</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga3c216fb9f06ecedc1565d2479c20a51e</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_FINISHED</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga27680b8dcf2740e6afd384f0f0b26003</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>enum tagHTTP_STATE_PARSING</type>
      <name>HTTP_STATE_PARSING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga5112371ddec9917722b85465dc5b3062</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_PARSER</type>
      <name>HTTP_PARSER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga1a0ea2ba6e2fd94a692102126e5ac0f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_PROCESS_MSG_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaccc79fd2431c99ecd68a090469da5d40</anchor>
      <arglist>)(HTTP_MESSAGE *msg, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_HEADER_PARSED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga50bf360c3d97280b2666c1809a506e2a</anchor>
      <arglist>)(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_MESSAGE_BODY_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa24d67ab42bb7b2da8c617b1b21b54a9</anchor>
      <arglist>)(HTTP_REQUEST *request, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_FINISHED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaaad3a1f235bb76629fb76266f229a6bc</anchor>
      <arglist>)(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_REQUEST_PARSER</type>
      <name>HTTP_REQUEST_PARSER</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>ga21137341221e8a3b92b3a73253d2fb08</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE_PARSER</type>
      <name>HTTP_RESPONSE_PARSER</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ga9fd23307e5d20621c1ae6973172f3d6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE_WRITER</type>
      <name>HTTP_RESPONSE_WRITER</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga666a8001dbee4674469ba1124d4f0e2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tagHttp_method_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga254d7e697a3b26a246614dd867644816</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_GET</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a90754abc55dbb76862fa50abee5af659</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_POST</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a1944682922ac79b2e682312d8f3f71e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_PUT</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816af7557927b605c64f35f629b43823b1c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_HEAD</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a27e993d38ff6b43284014a10798b6223</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_OPTIONS</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816af6b02f8ff0467dbdaebb24f0a996cdd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_DELETE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a761371f7807255b7912afbac2f665ffe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_TRACE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a95b5b19305a14de1400aedfdec1c1eed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_CONNECT</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a3755246bea87120e8311dc914d573fbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tagHttp_version_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga92ff2f79ea14e92657972a36d58883a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_VERSION_1_0</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga92ff2f79ea14e92657972a36d58883a1a3932308db22120ce049dd60f3430b814</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_VERSION_1_1</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga92ff2f79ea14e92657972a36d58883a1af1a34119b438038bc609a273f9bc08c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tagHTTP_STATE_PARSING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8399cf0558ed67328d5221a21a155cd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_REQUEST_LINE</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a4b3e4ef1a704d0e1c16ad482d0bb38aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_HEADERS</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2ab6734bc6ccbc93928357db1f17dd9e97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CONTENT_LENGTH</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2ade8a7e55f2fe126b0bddf7780bff40a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_HEADER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a0a070153481a16d24111499b85b08989</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a477106ecea88e2d9e496913739e9f3aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_EOF_AFTER_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a4529c656e6bcf37dfb70ea237b08602a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_TRAILER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2aed4bf2d5e923949fa9b3697ae97020c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>PARSER_STATUS</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gace21a5a69d5e05c5c42d4861f37a49c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_ERROR</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4a4bd96b2bfcb709eb5caf0ef934cc921d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_COMPLETED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4a25ead330805092a1babdf67ca0f018c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_NEED_MORE_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4af41f996b5530777e9711a51a4050bb9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_TK_TYPE</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga7c53c9c3c95f9f3ac02e3c9428639f8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_QUOTED_STRING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aad6d7754244a911a6b8cd06a72bae0a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_TEXT</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aa5a1d5470fadc11e11133c6cde6152a8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_SEPARATOR</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aae5c3c691300dabcc36e7c1bb99149f15</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_EOF</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aad0b61dc2115021e1798bc23bd4fabf78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_RESPONSE_WR_STATE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga9e080e7ca1d16b175996b2cda2576672</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_STATUS_LINE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a5e5a98554127cc4161ac3e6f9a224fd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CONNECTION_CLOSE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a802496a2dfbfa20b8949c1f826ada553</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CHUNKED</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a2046b96a1daddcaea11fedf47c9aef20</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CONTENT_LENGTH</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a048c496846785f0052808907603e5a6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_HEADERS</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a9616a2518f4fdffcc45caf3458b2a7a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_EOF</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a42c49006e654df7b1449bbd96acb3b86</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_init</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga14d478691c5f29cd966cb8abc4700a1f</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HTTP_MESSAGE_free</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gab01f22c91d577dfae9702308dbd0c518</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_add_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga1ca9f7fea4b5dfd0d084c546a489220e</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name, const char *value)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_MESSAGE_set_content_length</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gaa43fad45c09c9c1063dd42decc1e4b21</anchor>
      <arglist>(HTTP_MESSAGE *message, int content_length)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>HTTP_MESSAGE_find_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga33eb4b3bf28432797029c90115dd80d7</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_first_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga225a9e18b90020b71906c943ef3bcde3</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_next_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga405406f091c3d4236fc340910391cc87</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_is_persistent</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gaa3893b9709b438c414da743d2e9ba285</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HTTP_REQUEST_init</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gab308a83fced871be3693cdb3fba3ec09</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_REQUEST_free</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga4ab326e8402a38ac166e2f90a50d91ec</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HTTP_RESPONSE_init</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gacc700a0e0c7ed34df458f567dad4c374</anchor>
      <arglist>(HTTP_RESPONSE *message, Http_version_type version, int status_code)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_RESPONSE_free</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga7ca06985dfe2540a91cc74927115cde0</anchor>
      <arglist>(HTTP_RESPONSE *message)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa5370897809bd47280b44b5fa679d2d3</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_free</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaf7d9eeee759da719084291a7cd155c94</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_add_header_parser</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gae0af51ac516c4e68c9d7ba843fbea541</anchor>
      <arglist>(HTTP_PARSER *parser, const char *header_name, HEADER_ACTION action)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_get_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac5e21a738d73d75ebec125c97038961a</anchor>
      <arglist>(BF *bf, char **start_line)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_get_header_token</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8d14d1a7cd91bbd477872efafe8e9962</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_parse_header_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa737a4c11979880764fb8031d5dfc488</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *request, BF *bf, int *eof_header)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_content_length_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8b4ee246e968497579a9d9d194a92571</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *msg)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_PARSER_content_length_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga34a73b6fe4c943ce1831c8b93c7fb426</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_chunked_data_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga0cfd781757cd6db4812087df78424fda</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_PARSER_chunked_data_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac93ea444be8644a86692ae98ec0b86c6</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_PARSER_init</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>gaf0a986ea0582bfc4b011d2ac61aac9ca</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQ_HEADER_PARSED header_parsed, HTTP_REQ_MESSAGE_BODY_DATA on_message_body_data, HTTP_REQ_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_REQUEST_PARSER_process</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>ga84afcea80bc8b534b3eee27594d2afbd</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQUEST *request, BF *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_RESPONSE_PARSER_init</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ga00451ddae893b0ea2fb035d9cd1b319a</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESP_HEADER_PARSED header_parsed, HTTP_RESP_MESSAGE_BODY_DATA on_message_body_data, HTTP_RESP_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_PARSER_process</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>gac69a62184875abe24177d14f0a9a0313</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESPONSE *response, BF *data)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_RESPONSE_WRITER_init</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gabf2bcdffe3cecb7ca6484489009d8004</anchor>
      <arglist>(HTTP_RESPONSE_WRITER *writer, HTTP_RESPONSE *response)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_WRITER_write</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga52fcf93eeb1c013427abde12a234adff</anchor>
      <arglist>(HTTP_RESPONSE_WRITER *writer, BF *data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>query.c</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00018</filename>
    <includes id="a00019" name="query.h" local="yes" imported="no">query.h</includes>
    <includes id="a00020" name="sutils.h" local="yes" imported="no">sutils.h</includes>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>QUERYSTRING_add</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>a6dea40cf4f21e1920e14f8313d6cb97c</anchor>
      <arglist>(QUERYSTRING *query, char *name, char *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>compare_entry</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ab807d56c9ee74184743bbf877337ea6d</anchor>
      <arglist>(HASH_Entry *entry, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>QUERYSTRING_init</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>a84f6d4b865208e705c978cf1073277a0</anchor>
      <arglist>(QUERYSTRING *query, URI *uri)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>QUERYSTRING_free</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>a46527f532970c29c2f54fb1a4a36ab41</anchor>
      <arglist>(QUERYSTRING *query)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>query.h</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00019</filename>
    <includes id="a00022" name="uri.h" local="no" imported="no">hutils/uri.h</includes>
    <class kind="struct">tagHASHNAMEVALUE</class>
    <class kind="struct">tagQUERYSTRING</class>
    <member kind="define">
      <type>#define</type>
      <name>_QUERY_STRING_H</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>afbd6fdbfd91ab4847e5978c985f52f6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHASHNAMEVALUE</type>
      <name>HASHNAMEVALUE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ac184fd3f2d398c24cac43794432f19d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagQUERYSTRING</type>
      <name>QUERYSTRING</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ab329cce974e495979d3537ec5ec7f146</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>QUERYSTRING_init</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>a84f6d4b865208e705c978cf1073277a0</anchor>
      <arglist>(QUERYSTRING *query, URI *uri)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>QUERYSTRING_free</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>a46527f532970c29c2f54fb1a4a36ab41</anchor>
      <arglist>(QUERYSTRING *query)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>QUERYSTRING_find</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>abcabd89b8e64535cc2322d25b58297d4</anchor>
      <arglist>(QUERYSTRING *query, const char *name)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sutils.h</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00020</filename>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_space</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>abed2077fd9ffa7b7c62e7b9d6e72d54a</anchor>
      <arglist>(char ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>skip_spaces</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>a391c19d476a48f47d1ba80070b228c52</anchor>
      <arglist>(char *line)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>skip_non_spaces</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>af773e1b35368eb6ebe774f2a1603fe90</anchor>
      <arglist>(char *line)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>get_token</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>add28c967c71026785db6e89be7d006b2</anchor>
      <arglist>(char *line, char **eof)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>strdup_range</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ac3b6bc5a3df3720ec7dd261c75ce20ac</anchor>
      <arglist>(char *from, char *to)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>uri.c</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00021</filename>
    <includes id="a00022" name="uri.h" local="yes" imported="no">uri.h</includes>
    <includes id="a00014" name="charclass.h" local="yes" imported="no">charclass.h</includes>
    <includes id="a00020" name="sutils.h" local="yes" imported="no">sutils.h</includes>
    <class kind="struct">tagURIPARSECTX</class>
    <member kind="typedef">
      <type>struct tagURIPARSECTX</type>
      <name>URIPARSECTX</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ae8eefa49928eead588c27e25064dc764</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_mark</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a7f600c7e017e6798df185314bf06c79e</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_unreserved</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>acce9ea7662290b5b285addb415153760</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>is_reserved</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a4be1d64a304771921bfb433f56da9b8e</anchor>
      <arglist>(int8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ctx_copy_string_raw</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>abde5e24e4ee019b871ebe2a5575e270d</anchor>
      <arglist>(URIPARSECTX *ctx, char *start, char *end)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ctx_add_escaped_char</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a4b4396f507fe125c822b3ca4042a41da</anchor>
      <arglist>(URIPARSECTX *ctx, char ch, int char_encoded)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ctx_finish_escaped_string</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a0ad56df073d2ba27ea027e41469b3f9c</anchor>
      <arglist>(URIPARSECTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ctx_undo_escaped_string</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a80e6ead0b8e5e18540ac5e23d60575b4</anchor>
      <arglist>(URIPARSECTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_escaped</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a0b56a1f8c34732e7d34f6a26071196ff</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_uric</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>aa28908824686b0d40be21534a60ed2b9</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_uric_sequence</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>adfb50d1059700bd12272e3a5fba61461</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_pchar</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a98de74d7af924a1394abb989b52dfe6f</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_pchar_sequence</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>aa42297e323b57f1d41ecc6ab66d15228</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_segment</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a09d4853f4d37fd26d138645da055bafc</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_path_segments</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a55e2a69ac69cfed87ca6c2dc94b41e07</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_ipv4_address</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a631c7bb7fc4481fd87c0ba388854ca04</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_userinfo</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>aab2f681a85eb14cf468100ce68ed20ab</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_domainlabel</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ae0824c26d405c6e026b81e5863a7ee3e</anchor>
      <arglist>(char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_hostname</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>af8279a3b2cdc184e6a3e320792259b19</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_ipv6_address</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a3454d9f952367c406bc9232cd3dd45a2</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_hostport</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a66ff71abf545fe5b8177bd642e16653c</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_server</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>af5c64e23533d55310336a6616922852b</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_authority</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a54749645383feb12ffbe62e168c7d5fa</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_scheme</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a39e605f0073f009c55127a0e9bd21707</anchor>
      <arglist>(URIPARSECTX *ctx, char *line, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_abs_path</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ad882bddea7e87fc45d69b807bb53d5ab</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_net_path</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>af005921411390c1a063713542cc5e662</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>parse_uric_no_slash</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ae6afcc5b19c364ba41ef076cde2f1467</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_opaque_part</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a9d3fa2060cba0d40131cac8691492bbf</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_hier_part</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a329ed8214b110b4c294f413f49cb9635</anchor>
      <arglist>(URIPARSECTX *ctx, char *ptr, char **next, int parse_opaque)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>URI_parse</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab9acb8adb11be6e657892d858f40e917</anchor>
      <arglist>(URI *url, char *line)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>uri.h</name>
    <path>/home/mmoser/archive/cstuff_bak/hutils/</path>
    <filename>a00022</filename>
    <class kind="struct">tagURI</class>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_PARSE_PARTS</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga681056ca2fe0804b02c215e651dcbb2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_IS_ABSOLUTE_URI</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gafa8525689f71c3fcc360d66eb75b79ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_IS_OPAQUE</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab18d910ab6fe32e5e2bd49e138f1f1e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_IPv4</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga88117f2de53ca60461fdbfc19195c5e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_IPv6</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gabdb6298e0841c217c1799c247fd27e60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_HOSTNAME</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gaa8b74e1e32f1bfcbdc1c35bc351d0c37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HAS_SCHEME</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae3a98cd8c957552df89bce7b42ad9692</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagURI</type>
      <name>URI</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga5297e3ae1364bb45d1ab31ecba29aeaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>URI_init</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga8ff72430ab547d0151fc8c58c8039825</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>URI_parse</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab9acb8adb11be6e657892d858f40e917</anchor>
      <arglist>(URI *url, char *line)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>URI_free</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab82397c63ee9a3456c6bdb8e109a007a</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_scheme</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gad25a20b072ea1273ceae9c415f60885a</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_userinfo_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga7e7b36dd5a61ed237af86b2f61f18afe</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_userinfo</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gac9ffe857583c93e3f670fe91386b5007</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_host</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga49cd1027b83d86b0fb2feaeb9a5512b6</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>URI_port</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga1e247e963b134702005b8594f1208229</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_path_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga45df5100f7aaf9ff9b3a48a7e1b8058f</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_path</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gafc9b62fcf20ed6453067e40b73afda3e</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_query_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gacb7e21082a7179b1c28b1790f5add72b</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_query</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae515aeb08040a6457cc2e0100bd3be99</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_fragment_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga622da4084e73a2c02abde7034f8ad6ea</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_fragment</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae6489119ac652510ee30ed253635f8b6</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_opaque_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gaff48eba33becc19347ac38fcf53d98b7</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_opaque</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga773c3e4af0c6238a6412da8f6f59898f</anchor>
      <arglist>(URI *url)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_MESSAGE</name>
    <title>HTTP_MESSAGE</title>
    <filename>a00023.html</filename>
    <class kind="struct">tagSTRINGPAIR</class>
    <class kind="struct">tagHTTP_MESSAGE</class>
    <member kind="typedef">
      <type>struct tagSTRINGPAIR</type>
      <name>STRINGPAIR</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga8118b527e2c0aaba786f406681e80b65</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_MESSAGE</type>
      <name>HTTP_MESSAGE</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gab956fd255b384bed00c5015cd1a6ee14</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_init</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga14d478691c5f29cd966cb8abc4700a1f</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HTTP_MESSAGE_free</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gab01f22c91d577dfae9702308dbd0c518</anchor>
      <arglist>(HTTP_MESSAGE *message)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_MESSAGE_add_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga1ca9f7fea4b5dfd0d084c546a489220e</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name, const char *value)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_MESSAGE_set_content_length</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>gaa43fad45c09c9c1063dd42decc1e4b21</anchor>
      <arglist>(HTTP_MESSAGE *message, int content_length)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>HTTP_MESSAGE_find_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga33eb4b3bf28432797029c90115dd80d7</anchor>
      <arglist>(HTTP_MESSAGE *message, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_first_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga225a9e18b90020b71906c943ef3bcde3</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
    <member kind="function">
      <type>STRINGPAIR *</type>
      <name>HTTP_MESSAGE_next_header</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>ga405406f091c3d4236fc340910391cc87</anchor>
      <arglist>(HTTP_MESSAGE *message, DLISTUNR_position *pos)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_REQUEST</name>
    <title>HTTP_REQUEST</title>
    <filename>a00024.html</filename>
    <class kind="struct">tagHTTP_REQUEST</class>
    <member kind="typedef">
      <type>int(*</type>
      <name>HEADER_ACTION</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gaef3a97b256b09f6a4ef1527ead10d52f</anchor>
      <arglist>)(struct tagHTTP_MESSAGE *request, struct tagHTTP_PARSER *parser)</arglist>
    </member>
    <member kind="typedef">
      <type>enum tagHttp_method_type</type>
      <name>Http_method_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gac5182036656d3198819f12ef9ec9d08b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tagHttp_version_type</type>
      <name>Http_version_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga06dc534889d1806e0448e56dd9757dae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_REQUEST</type>
      <name>HTTP_REQUEST</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga7548b2c494ccf7acc059409a5d21ccaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tagHttp_method_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga254d7e697a3b26a246614dd867644816</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_GET</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a90754abc55dbb76862fa50abee5af659</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_POST</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a1944682922ac79b2e682312d8f3f71e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_PUT</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816af7557927b605c64f35f629b43823b1c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_HEAD</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a27e993d38ff6b43284014a10798b6223</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_OPTIONS</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816af6b02f8ff0467dbdaebb24f0a996cdd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_DELETE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a761371f7807255b7912afbac2f665ffe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_TRACE</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a95b5b19305a14de1400aedfdec1c1eed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_METHOD_CONNECT</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga254d7e697a3b26a246614dd867644816a3755246bea87120e8311dc914d573fbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tagHttp_version_type</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga92ff2f79ea14e92657972a36d58883a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_VERSION_1_0</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga92ff2f79ea14e92657972a36d58883a1a3932308db22120ce049dd60f3430b814</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_VERSION_1_1</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gga92ff2f79ea14e92657972a36d58883a1af1a34119b438038bc609a273f9bc08c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_is_persistent</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gaa3893b9709b438c414da743d2e9ba285</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HTTP_REQUEST_init</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>gab308a83fced871be3693cdb3fba3ec09</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_REQUEST_free</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>ga4ab326e8402a38ac166e2f90a50d91ec</anchor>
      <arglist>(HTTP_REQUEST *message)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_RESPONSE</name>
    <title>HTTP_RESPONSE</title>
    <filename>a00025.html</filename>
    <class kind="struct">tagHTTP_RESPONSE</class>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE</type>
      <name>HTTP_RESPONSE</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gac0d52817d9795fea81e6cfa5afa369e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_HEADER_PARSED</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gab50a0c614e6c5e0b1364121270be9076</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_MESSAGE_BODY_DATA</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga3c216fb9f06ecedc1565d2479c20a51e</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_RESP_FINISHED</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga27680b8dcf2740e6afd384f0f0b26003</anchor>
      <arglist>)(HTTP_RESPONSE *request, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HTTP_RESPONSE_init</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>gacc700a0e0c7ed34df458f567dad4c374</anchor>
      <arglist>(HTTP_RESPONSE *message, Http_version_type version, int status_code)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_RESPONSE_free</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>ga7ca06985dfe2540a91cc74927115cde0</anchor>
      <arglist>(HTTP_RESPONSE *message)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_PARSER</name>
    <title>HTTP_PARSER</title>
    <filename>a00026.html</filename>
    <class kind="struct">tagHTTP_PARSER</class>
    <member kind="typedef">
      <type>enum tagHTTP_STATE_PARSING</type>
      <name>HTTP_STATE_PARSING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga5112371ddec9917722b85465dc5b3062</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagHTTP_PARSER</type>
      <name>HTTP_PARSER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga1a0ea2ba6e2fd94a692102126e5ac0f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_PROCESS_MSG_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaccc79fd2431c99ecd68a090469da5d40</anchor>
      <arglist>)(HTTP_MESSAGE *msg, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_HEADER_PARSED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga50bf360c3d97280b2666c1809a506e2a</anchor>
      <arglist>)(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_MESSAGE_BODY_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa24d67ab42bb7b2da8c617b1b21b54a9</anchor>
      <arglist>)(HTTP_REQUEST *request, void *data, size_t data_size, void *ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HTTP_REQ_FINISHED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaaad3a1f235bb76629fb76266f229a6bc</anchor>
      <arglist>)(HTTP_REQUEST *request, void *ctx)</arglist>
    </member>
    <member kind="enumeration">
      <name>tagHTTP_STATE_PARSING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8399cf0558ed67328d5221a21a155cd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_REQUEST_LINE</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a4b3e4ef1a704d0e1c16ad482d0bb38aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_HEADERS</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2ab6734bc6ccbc93928357db1f17dd9e97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CONTENT_LENGTH</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2ade8a7e55f2fe126b0bddf7780bff40a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_HEADER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a0a070153481a16d24111499b85b08989</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a477106ecea88e2d9e496913739e9f3aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_EOF_AFTER_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2a4529c656e6bcf37dfb70ea237b08602a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_STATE_PARSING_BODY_CHUNK_TRAILER</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga8399cf0558ed67328d5221a21a155cd2aed4bf2d5e923949fa9b3697ae97020c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>PARSER_STATUS</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gace21a5a69d5e05c5c42d4861f37a49c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_ERROR</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4a4bd96b2bfcb709eb5caf0ef934cc921d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_COMPLETED</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4a25ead330805092a1babdf67ca0f018c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PARSER_STATUS_NEED_MORE_DATA</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ggace21a5a69d5e05c5c42d4861f37a49c4af41f996b5530777e9711a51a4050bb9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_TK_TYPE</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga7c53c9c3c95f9f3ac02e3c9428639f8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_QUOTED_STRING</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aad6d7754244a911a6b8cd06a72bae0a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_TEXT</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aa5a1d5470fadc11e11133c6cde6152a8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_SEPARATOR</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aae5c3c691300dabcc36e7c1bb99149f15</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_TK_EOF</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gga7c53c9c3c95f9f3ac02e3c9428639f8aad0b61dc2115021e1798bc23bd4fabf78</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa5370897809bd47280b44b5fa679d2d3</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_free</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaf7d9eeee759da719084291a7cd155c94</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_add_header_parser</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gae0af51ac516c4e68c9d7ba843fbea541</anchor>
      <arglist>(HTTP_PARSER *parser, const char *header_name, HEADER_ACTION action)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_get_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac5e21a738d73d75ebec125c97038961a</anchor>
      <arglist>(BF *bf, char **start_line)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_get_header_token</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8d14d1a7cd91bbd477872efafe8e9962</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_parse_header_line</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gaa737a4c11979880764fb8031d5dfc488</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *request, BF *bf, int *eof_header)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_content_length_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga8b4ee246e968497579a9d9d194a92571</anchor>
      <arglist>(HTTP_PARSER *parser, HTTP_MESSAGE *msg)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_PARSER_content_length_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga34a73b6fe4c943ce1831c8b93c7fb426</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_PARSER_chunked_data_init</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>ga0cfd781757cd6db4812087df78424fda</anchor>
      <arglist>(HTTP_PARSER *parser)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_PARSER_chunked_data_process</name>
      <anchorfile>a00026.html</anchorfile>
      <anchor>gac93ea444be8644a86692ae98ec0b86c6</anchor>
      <arglist>(HTTP_PARSER *parser, BF *bf, HTTP_PROCESS_MSG_DATA cb, HTTP_MESSAGE *msg, void *ctx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_REQUEST_PARSER</name>
    <title>HTTP_REQUEST_PARSER</title>
    <filename>a00027.html</filename>
    <class kind="struct">tagHTTP_REQUEST_PARSER</class>
    <member kind="typedef">
      <type>struct tagHTTP_REQUEST_PARSER</type>
      <name>HTTP_REQUEST_PARSER</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>ga21137341221e8a3b92b3a73253d2fb08</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_REQUEST_PARSER_init</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>gaf0a986ea0582bfc4b011d2ac61aac9ca</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQ_HEADER_PARSED header_parsed, HTTP_REQ_MESSAGE_BODY_DATA on_message_body_data, HTTP_REQ_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_REQUEST_PARSER_process</name>
      <anchorfile>a00027.html</anchorfile>
      <anchor>ga84afcea80bc8b534b3eee27594d2afbd</anchor>
      <arglist>(HTTP_REQUEST_PARSER *parser, HTTP_REQUEST *request, BF *data)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_RESPONSE_PARSER</name>
    <title>HTTP_RESPONSE_PARSER</title>
    <filename>a00028.html</filename>
    <class kind="struct">tagHTTP_RESPONSE_PARSER</class>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE_PARSER</type>
      <name>HTTP_RESPONSE_PARSER</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ga9fd23307e5d20621c1ae6973172f3d6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HTTP_RESPONSE_PARSER_init</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>ga00451ddae893b0ea2fb035d9cd1b319a</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESP_HEADER_PARSED header_parsed, HTTP_RESP_MESSAGE_BODY_DATA on_message_body_data, HTTP_RESP_FINISHED on_request_finished, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_PARSER_process</name>
      <anchorfile>a00028.html</anchorfile>
      <anchor>gac69a62184875abe24177d14f0a9a0313</anchor>
      <arglist>(HTTP_RESPONSE_PARSER *parser, HTTP_RESPONSE *response, BF *data)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HTTP_RESPONSE_WRITER</name>
    <title>HTTP_RESPONSE_WRITER</title>
    <filename>a00029.html</filename>
    <class kind="struct">tagHTTP_RESPONSE_WRITER</class>
    <member kind="typedef">
      <type>struct tagHTTP_RESPONSE_WRITER</type>
      <name>HTTP_RESPONSE_WRITER</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga666a8001dbee4674469ba1124d4f0e2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>HTTP_RESPONSE_WR_STATE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga9e080e7ca1d16b175996b2cda2576672</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_STATUS_LINE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a5e5a98554127cc4161ac3e6f9a224fd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CONNECTION_CLOSE</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a802496a2dfbfa20b8949c1f826ada553</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CHUNKED</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a2046b96a1daddcaea11fedf47c9aef20</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_CONTENT_LENGTH</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a048c496846785f0052808907603e5a6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_HEADERS</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a9616a2518f4fdffcc45caf3458b2a7a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HTTP_RESPONSE_WR_EOF</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gga9e080e7ca1d16b175996b2cda2576672a42c49006e654df7b1449bbd96acb3b86</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HTTP_RESPONSE_WRITER_init</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>gabf2bcdffe3cecb7ca6484489009d8004</anchor>
      <arglist>(HTTP_RESPONSE_WRITER *writer, HTTP_RESPONSE *response)</arglist>
    </member>
    <member kind="function">
      <type>PARSER_STATUS</type>
      <name>HTTP_RESPONSE_WRITER_write</name>
      <anchorfile>a00029.html</anchorfile>
      <anchor>ga52fcf93eeb1c013427abde12a234adff</anchor>
      <arglist>(HTTP_RESPONSE_WRITER *writer, BF *data)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>URI</name>
    <title>URI</title>
    <filename>a00030.html</filename>
    <class kind="struct">tagURI</class>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_PARSE_PARTS</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga681056ca2fe0804b02c215e651dcbb2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_IS_ABSOLUTE_URI</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gafa8525689f71c3fcc360d66eb75b79ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_IS_OPAQUE</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab18d910ab6fe32e5e2bd49e138f1f1e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_IPv4</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga88117f2de53ca60461fdbfc19195c5e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_IPv6</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gabdb6298e0841c217c1799c247fd27e60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HOST_HOSTNAME</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gaa8b74e1e32f1bfcbdc1c35bc351d0c37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>URI_FLAGS_HAS_SCHEME</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae3a98cd8c957552df89bce7b42ad9692</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagURI</type>
      <name>URI</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga5297e3ae1364bb45d1ab31ecba29aeaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>URI_init</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga8ff72430ab547d0151fc8c58c8039825</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>URI_parse</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab9acb8adb11be6e657892d858f40e917</anchor>
      <arglist>(URI *url, char *line)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>URI_free</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gab82397c63ee9a3456c6bdb8e109a007a</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_scheme</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gad25a20b072ea1273ceae9c415f60885a</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_userinfo_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga7e7b36dd5a61ed237af86b2f61f18afe</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_userinfo</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gac9ffe857583c93e3f670fe91386b5007</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_host</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga49cd1027b83d86b0fb2feaeb9a5512b6</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>URI_port</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga1e247e963b134702005b8594f1208229</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_path_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga45df5100f7aaf9ff9b3a48a7e1b8058f</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_path</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gafc9b62fcf20ed6453067e40b73afda3e</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_query_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gacb7e21082a7179b1c28b1790f5add72b</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_query</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae515aeb08040a6457cc2e0100bd3be99</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_fragment_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga622da4084e73a2c02abde7034f8ad6ea</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_fragment</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gae6489119ac652510ee30ed253635f8b6</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_opaque_raw</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>gaff48eba33becc19347ac38fcf53d98b7</anchor>
      <arglist>(URI *url)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE const char *</type>
      <name>URI_opaque</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>ga773c3e4af0c6238a6412da8f6f59898f</anchor>
      <arglist>(URI *url)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHASHNAMEVALUE</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>HASH_Entry</type>
      <name>entry</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a9b0aa2a1d02e97c7c6cc418c6796719c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a07221fee023c96635ad91fc46453111f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a9a64ab3ffcef1e3ef1771dd98d31ceb7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHEADER_HASH_ACTION</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>HASH_Entry</type>
      <name>entry</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a6b9b4472573fb86e787436c2582d901e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HEADER_ACTION</type>
      <name>action</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a85615c856f9538b55f4739674e034f21</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>key</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a41da7c59b49a7ca0bcc5345fc603c9a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>key_size</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>afa5c2b37b34012114d61e1e9f730b854</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_MESSAGE</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>flags</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a7aa368f6b6bd57c4503b088bd60f6168</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>content_length</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a8db203330e6b1c3fb859697a1d81494c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLISTUNR</type>
      <name>header_values</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a15b288a53fc55ce8d89c1fdef6aab26e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_PARSER</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>HTTP_STATE_PARSING</type>
      <name>state</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ac18acb9d3ec3879248cee7e1f448a81a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH</type>
      <name>header_action</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a3ec9458f4c7de0bc079b002d0b64441d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>token</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>abb7efefca85ab0dd6f7058d3ba4b14e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>token_length</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ab092d2eec30e89f56c762ce5123f2d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>tokpos</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a6d0367b48bfcd2d8b670930d0a6a5e12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>content_left</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a390ab241d634a4041103ba39096f090f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_REQUEST</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>HTTP_MESSAGE</type>
      <name>base</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a66fc31b502c5ab5699c8f8263991f69b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Http_method_type</type>
      <name>method</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a002f168e0c3b2a616f3626867e09dd3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>URI</type>
      <name>url</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>aa43cab316cd65661dc36cf8f5d0b78c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>raw_url</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a429a80b15eec069ce2f6ddf3751ccbb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Http_version_type</type>
      <name>version</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a6225f594931f6ff84063b1affca2f582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>expect_100_continue</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a1a7c838fee849231ffe764fd004dadab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>has_host_header</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a013e5373d35354986ff53b318452edf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>host_header</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a2a389a3f86e4c557499801b8b9999000</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>host_header_port</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a53fed18a89d849dadd1d59b353c48553</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_REQUEST_PARSER</name>
    <filename>a00006.html</filename>
    <member kind="variable">
      <type>HTTP_PARSER</type>
      <name>base</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>aabce9198ce8df4a2bdda071cc49d2f98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>ctx</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ae225e688d56dd152ac4ffa8c36d38770</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQ_HEADER_PARSED</type>
      <name>ev_header</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>add769d72f649da147653730b07d10865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQ_MESSAGE_BODY_DATA</type>
      <name>ev_body</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a23d5b42db28063c39c624b2ffcb51d0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_REQ_FINISHED</type>
      <name>ev_finish</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ae45a5d00172db9563679af5fb3f6f808</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_RESPONSE</name>
    <filename>a00007.html</filename>
    <member kind="variable">
      <type>HTTP_MESSAGE</type>
      <name>base</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>af1abce2e9b1a425200c02c4c9fa8b710</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>status_code</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>abc01517233416f0822c535a25876ef2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Http_version_type</type>
      <name>version</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a648d45911f049bab758356e9dc4713b4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_RESPONSE_PARSER</name>
    <filename>a00008.html</filename>
    <member kind="variable">
      <type>HTTP_PARSER</type>
      <name>base</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aed394eb19316db4cf7a4269649489937</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_RESP_HEADER_PARSED</type>
      <name>ev_header</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a1c4042cbfe80f87e256d02899c4988f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_RESP_MESSAGE_BODY_DATA</type>
      <name>ev_body</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aa4022d67b56504039b6351c6cdbd1ffb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_RESP_FINISHED</type>
      <name>ev_finish</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aa6a6117b92ad2641df0cc4d5c491e9e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_STATE_PARSING</type>
      <name>state</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a89e4a9f3306c2644fa461cd4436cf3cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>ctx</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ae0027fa6c255faf0cc9ae2a1858c3186</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagHTTP_RESPONSE_WRITER</name>
    <filename>a00009.html</filename>
    <member kind="variable">
      <type>HTTP_RESPONSE *</type>
      <name>response</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>aa2d61ec9418183f40814fe75a576441d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HTTP_RESPONSE_WR_STATE</type>
      <name>state</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a5cdc7b37263211895aea5b90e695b774</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLISTUNR_position</type>
      <name>header_position</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>acdfd9571982ff46434b3d95001148d38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>state_header</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>af272831a354b83a6ef0f011e9673cf53</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagQUERYSTRING</name>
    <filename>a00010.html</filename>
    <member kind="variable">
      <type>HASH</type>
      <name>name_to_value</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a080441dc73529a84e29901e6184433fe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSTRINGPAIR</name>
    <filename>a00011.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>key</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ac2ecc9b0399e0309fbba83739f1c4c40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a396c2de767eded3dc6ec524ea1a714a8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagURI</name>
    <filename>a00012.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>flags</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a51cab48f1903d47847abd4fd2faa16dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a11ae40e294bcabbeb21d210fbc324a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>acae695b7cb9d303ce4180e0fa0ab44e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_is_escaped</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a3dda8a91c57bac6e9db0edc60c0572e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>scheme</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ac943b7c7bd534ddc1a84db68fdb9dbed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>userinfo_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ada1006ea50c7792e3645fc5be0dc3a2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>userinfo</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>afff11e0c3c453382ec37ea31aee23753</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>host</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a7ec586c2aee51d79c245ffce344a6c18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>port</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a90106354cb3a73f8035dd377eed540d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>path_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a928a5e3c37023a0a884a703077924eeb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>path</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a8fb5ba5e6bb70ec1b3599bdcaf813093</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>query_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a93e6702c354c94b3157df02701827a2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>query</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a398a57e4184218f02fc33b04da7819fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>fragment_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a474ccc0167a309bffaa8339f55ba51ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>fragment</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>afb4b0def9276bead2a348bab74e1034e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>opaque_raw</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a7dc74378d1a475a10d1528d7ad4dd968</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>opaque</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a591b8403463e8028851a7a84068672cf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagURIPARSECTX</name>
    <filename>a00013.html</filename>
    <member kind="variable">
      <type>URI *</type>
      <name>rep</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a247f676f1f828f9573fab8b77d18a162</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_pos</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>adb85398bbc31177e903ba48a431aabbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_pos_start</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ab0bbce6c84fdc9b47d66720287845852</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_pos_off</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ab5eec79fbd87a3faddbe6c6e37da4e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_raw_pos</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>aed0cfa54ff781d5f68562a02acbf3fba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cdata_is_escaped_start</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ab6c0fad059718b37ab69741ae5761f73</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
