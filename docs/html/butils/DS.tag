<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>errorp.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00003</filename>
    <includes id="a00004" name="errorp.h" local="yes" imported="no">errorp.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_TOKEN</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a2983b695e827265a5b46ab9405446e36</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STACK_FRAMES</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>afa5bb6221801445761a1f1c41101284f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STACK_START</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a1763c9556bd80b7806528729edc98a05</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STACK_EOF</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ae6fd4cb094a69a8f1f658790e4b4d382</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOKENS_COUNT</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a1503f41ca9996e1cff0c0bd0d52df9b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOKENS_ADDRESS</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a0b70f2e92ed8af3ef8c75034dd136442</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOKENS_PERMISSIONS</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a938e366626f3071da9742542d78db21a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOKENS_FILEOFFSET</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ad1f1c63dd79ab59b3cc6b09666323cb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOKENS_PATH</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a21e4c441c78ad666731d68b29c57e365</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>errorp_open_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gaeb8c47ac00ab256e031f9897e3cc4be4</anchor>
      <arglist>(int rval, const char *fmt,...)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>FD_OUT</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>aeabf08bbf5921db02b75ca07b49ee34a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>FD_ASSIGNED</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a5ef1a826bfb22c8590a5773ba030fdd7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>errorp.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00004</filename>
    <member kind="function">
      <type>int</type>
      <name>errorp_open_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gaeb8c47ac00ab256e031f9897e3cc4be4</anchor>
      <arglist>(int rval, const char *fmt,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hex.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00005</filename>
    <includes id="a00006" name="hex.h" local="yes" imported="no">hex.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>HEX_LINE_LENGTH</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a1163f5b565bd25e41c6369c510530511</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>make_hex_dump</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>parse_hexa_string</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga171452af1102322f5c77dfcf1aa5e8d2</anchor>
      <arglist>(const char *str, int len, uint32_t *rsize)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hex.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00006</filename>
    <member kind="function">
      <type>void *</type>
      <name>parse_hexa_string</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga171452af1102322f5c77dfcf1aa5e8d2</anchor>
      <arglist>(const char *str, int len, uint32_t *rsize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>make_hex_dump</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>misc.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00007</filename>
    <member kind="function">
      <type>int</type>
      <name>strdup_pair</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a2024408084a1958cda98fb62170a479b</anchor>
      <arglist>(char **first, char **second, const char *in_first, const char *in_second)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtk.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00008</filename>
    <includes id="a00009" name="strtk.h" local="yes" imported="no">strtk.h</includes>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtk.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00009</filename>
    <class kind="struct">tagSTRTK</class>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_PATTERN_SIZE</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a4718a59241012667de9862df37383a48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_IS_CHAR</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga6af25e1d072d3e1ed7d7c0682831155c</anchor>
      <arglist>(tok, ch)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTRTK</type>
      <name>STRTK</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga76f725215889db6e091bec0b60be9d33</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>STRTK_add_char</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gae43120a54e8902f8bd356ee57a9fc7de</anchor>
      <arglist>(STRTK *tok, uint8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_spn</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gacbaa999a7ed8334ee4f9c074190522f5</anchor>
      <arglist>(STRTK *tok, const char *hay)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_nspn</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaa298a57dd2c32290c6f0d7932d73b031</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_ntok</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaf5f9a2448a798742f4c632df8207bee6</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize, char **end_tok)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_tok</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga970da7da688e9d0412ef1e688b340f32</anchor>
      <arglist>(STRTK *tok, const char *hay, char **end_tok)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tokparser.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00010</filename>
    <includes id="a00011" name="tokparser.h" local="yes" imported="no">tokparser.h</includes>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tokparser.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00011</filename>
    <class kind="struct">tagTOKENDEF</class>
    <member kind="typedef">
      <type>struct tagTOKENDEF</type>
      <name>TOKENDEF</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga31b87f2e9234f4d8a6844ffa2a401efa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>ERRORP</name>
    <title>ERRORP</title>
    <filename>a00012.html</filename>
    <member kind="function">
      <type>int</type>
      <name>errorp_open_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gaeb8c47ac00ab256e031f9897e3cc4be4</anchor>
      <arglist>(int rval, const char *fmt,...)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HEX</name>
    <title>HEX</title>
    <filename>a00013.html</filename>
    <member kind="function">
      <type>void *</type>
      <name>parse_hexa_string</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga171452af1102322f5c77dfcf1aa5e8d2</anchor>
      <arglist>(const char *str, int len, uint32_t *rsize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>make_hex_dump</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STRTK</name>
    <title>STRTK</title>
    <filename>a00014.html</filename>
    <class kind="struct">tagSTRTK</class>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_IS_CHAR</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga6af25e1d072d3e1ed7d7c0682831155c</anchor>
      <arglist>(tok, ch)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTRTK</type>
      <name>STRTK</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga76f725215889db6e091bec0b60be9d33</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>STRTK_add_char</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gae43120a54e8902f8bd356ee57a9fc7de</anchor>
      <arglist>(STRTK *tok, uint8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_spn</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gacbaa999a7ed8334ee4f9c074190522f5</anchor>
      <arglist>(STRTK *tok, const char *hay)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_nspn</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaa298a57dd2c32290c6f0d7932d73b031</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_ntok</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaf5f9a2448a798742f4c632df8207bee6</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize, char **end_tok)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_tok</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga970da7da688e9d0412ef1e688b340f32</anchor>
      <arglist>(STRTK *tok, const char *hay, char **end_tok)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>TOKENDEF</name>
    <title>TOKENDEF</title>
    <filename>a00015.html</filename>
    <class kind="struct">tagTOKENDEF</class>
    <member kind="typedef">
      <type>struct tagTOKENDEF</type>
      <name>TOKENDEF</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga31b87f2e9234f4d8a6844ffa2a401efa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>type</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gab7bce27d71c22f9e38d80845e52f5142</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSTRTK</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>pattern</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a76f43694b03a12f46a232342054e01ae</anchor>
      <arglist>[STRTK_PATTERN_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTOKENDEF</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>token_type</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a44e79ba4222415bf07f916e042e7d822</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>type</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gab7bce27d71c22f9e38d80845e52f5142</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
