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
      <name>errorp_is_file_open</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga581e39706983163a300cd50728794634</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>errorp_open_file</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>gaeb8c47ac00ab256e031f9897e3cc4be4</anchor>
      <arglist>(int rval, const char *fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>error_dump_string</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ac8abec5b575fdad732d9b5fa3b95d608</anchor>
      <arglist>(const char *msg, char *buff, size_t buff_size)</arglist>
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
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>errorp_is_file_open</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga581e39706983163a300cd50728794634</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00013.html</anchorfile>
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
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>parse_hexa_string</name>
      <anchorfile>a00014.html</anchorfile>
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
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga171452af1102322f5c77dfcf1aa5e8d2</anchor>
      <arglist>(const char *str, int len, uint32_t *rsize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>make_hex_dump</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logg.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00007</filename>
    <includes id="a00004" name="errorp.h" local="no" imported="no">butils/errorp.h</includes>
    <includes id="a00008" name="logg.h" local="no" imported="no">butils/logg.h</includes>
    <member kind="function">
      <type>void</type>
      <name>error_dump_string</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ac8abec5b575fdad732d9b5fa3b95d608</anchor>
      <arglist>(const char *msg, char *buff, size_t buff_size)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>tls_free</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a0a8a663cc73982f823669e035fe73df6</anchor>
      <arglist>(void *val)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_init</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga33c82ea4ab2a41cdea9054948c9b3ec5</anchor>
      <arglist>(MLOG_LEVEL current, MLOG_ACTION action, void *arg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_alloc_option</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gafda3d5be4bd7fe8938c87e71e835f52a</anchor>
      <arglist>(MLOG_ALLOC option, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MLOG_dump_stack_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaec863161f6870f16f8d9934d83e61e6d</anchor>
      <arglist>(MLOG_LEVEL level)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_printf</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga0150a13359adcc1be1d1aa0f6c136d80</anchor>
      <arglist>(MLOG_LEVEL current, const char *file, int line, const char *format,...)</arglist>
    </member>
    <member kind="variable">
      <type>MLOG_LEVEL</type>
      <name>m_current_log_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga1b7a2c0199f158ae15f81e61ec18ad45</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static MLOG_ACTION</type>
      <name>log_action</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ada0962f3c9bfc9ac2ed216c787d5a58d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static MLOG_ALLOC</type>
      <name>alloc_strategy</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a8d3637cbadc4f3b006664083b0a9203a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static MLOG_LEVEL</type>
      <name>stack_dump_level</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a4f00fac0b595ea84604ab3105a323930</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static size_t</type>
      <name>alloc_size</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a17be629b5c7f3f19ca40d24c42ec7636</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static pthread_key_t</type>
      <name>log_tls_entry</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ad9b52b6f17eeecc8f9c65ac2bf83ce51</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>log_fd</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ad9f84b0791cb1cac996456b223aeffec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>log_prefix_level</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a6fb674fec51d550595311a8061028185</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char *</type>
      <name>log_level_names</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a3d1bf03f51fd0feb01a65fffd5ff4f7c</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logg.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00008</filename>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_ERROR</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaccfe4b146657dc08bac399764fe488a9</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_WARN</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga1e3c71175e1cfb16a658bf70afb840f0</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_INFO</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga25c1dbc5e47207e6b29673c15d817feb</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_DEBUG</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gad9b5ce484e6e5aeea0a1f231bc9cb05a</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2c0b6b38329f95e906198b7b23ef44dd</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_LEVEL</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga58ee0f237f34bb12ecd3f3563bae2b43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_TURN_OFF_LOGGING</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43aacb3fe0cbf1a107f7cf442a4390dfc51</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_ERROR</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43a987dfdab40fb4cff7066880de918ac46</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_WARN</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43ab654f698fdb86e39d3a055ca607b0392</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_INFO</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43a8ffc43a57f8b07b226fcb7e8ddf0cf84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_DEBUG</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43afab730f69616ab81b856a263f771f572</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43ad2199d8488ecbea5ae4912eccb28ff7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_ACTION</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga4a5ea034951d1f43cb4e83681ada1450</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_TO_FILE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a352870a22c79768f7266dede995bef49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_SYSLOG_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a9420de7fcaf1bcf0ef84b2f90e94f397</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_CONSOLE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a28b468eda2e029c2e9ae441911b6aea6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_PREFIX</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2d7c021b73763bff61683052e2e1e695</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_LOG_LEVEL</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695afec2d2208fccc9c49688e81d39c15969</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_SOURCE_FILE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695aa01d063ac6bbda5767f8ea78ae53d98f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_TIME</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695a14cbf3164f8c23ee45f099bb7cc3f6ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_ALLOC</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga64789137cb6d20255c4ae2f694860956</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_STACK</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956aa30f577c355d512b02dc3ad0c1b5b3c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_HEAP</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956a0417454d43c698e83982f3544ae84e28</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_TLS_HEAP</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956a8bb0a30df2cc7447c02f3f72a09109d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_init</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga33c82ea4ab2a41cdea9054948c9b3ec5</anchor>
      <arglist>(MLOG_LEVEL current, MLOG_ACTION action, void *arg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MLOG_set_prefix_features</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaf54b9c4ac40e86f37fde4a347eda9413</anchor>
      <arglist>(int prefix)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_alloc_option</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gafda3d5be4bd7fe8938c87e71e835f52a</anchor>
      <arglist>(MLOG_ALLOC option, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MLOG_dump_stack_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaec863161f6870f16f8d9934d83e61e6d</anchor>
      <arglist>(MLOG_LEVEL stack_dump_level)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_printf</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga0150a13359adcc1be1d1aa0f6c136d80</anchor>
      <arglist>(MLOG_LEVEL current, const char *file, int line, const char *format,...)</arglist>
    </member>
    <member kind="variable">
      <type>MLOG_LEVEL</type>
      <name>m_current_log_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga1b7a2c0199f158ae15f81e61ec18ad45</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtk.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00009</filename>
    <includes id="a00010" name="strtk.h" local="yes" imported="no">strtk.h</includes>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtk.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00010</filename>
    <class kind="struct">tagSTRTK</class>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_PATTERN_SIZE</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a4718a59241012667de9862df37383a48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_IS_CHAR</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga6af25e1d072d3e1ed7d7c0682831155c</anchor>
      <arglist>(tok, ch)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTRTK</type>
      <name>STRTK</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga76f725215889db6e091bec0b60be9d33</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>STRTK_add_char</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gae43120a54e8902f8bd356ee57a9fc7de</anchor>
      <arglist>(STRTK *tok, uint8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_spn</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gacbaa999a7ed8334ee4f9c074190522f5</anchor>
      <arglist>(STRTK *tok, const char *hay)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_nspn</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gaa298a57dd2c32290c6f0d7932d73b031</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_ntok</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gaf5f9a2448a798742f4c632df8207bee6</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize, char **end_tok)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_tok</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga970da7da688e9d0412ef1e688b340f32</anchor>
      <arglist>(STRTK *tok, const char *hay, char **end_tok)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tokparser.c</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00011</filename>
    <includes id="a00012" name="tokparser.h" local="yes" imported="no">tokparser.h</includes>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tokparser.h</name>
    <path>/home/mmoser/archive/cstuff_bak/butils/</path>
    <filename>a00012</filename>
    <class kind="struct">tagTOKENDEF</class>
    <member kind="typedef">
      <type>struct tagTOKENDEF</type>
      <name>TOKENDEF</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga31b87f2e9234f4d8a6844ffa2a401efa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>ERRORP</name>
    <title>ERRORP</title>
    <filename>a00013.html</filename>
    <member kind="function">
      <type>int</type>
      <name>errorp_open_file</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga766648cd1050284e3170e1122e776b4f</anchor>
      <arglist>(const char *file)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>errorp_is_file_open</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga581e39706983163a300cd50728794634</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp_close_file</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga6911b37ee103e7778baa57f3ef4e1f58</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errorp</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>gaeb8c47ac00ab256e031f9897e3cc4be4</anchor>
      <arglist>(int rval, const char *fmt,...)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HEX</name>
    <title>HEX</title>
    <filename>a00014.html</filename>
    <member kind="function">
      <type>void *</type>
      <name>parse_hexa_string</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ga171452af1102322f5c77dfcf1aa5e8d2</anchor>
      <arglist>(const char *str, int len, uint32_t *rsize)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>make_hex_dump</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>gaebdad1efeb939693a66ba96041b0a7cb</anchor>
      <arglist>(const char *data, size_t len)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>logging</name>
    <title>library.</title>
    <filename>a00015.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_ERROR</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaccfe4b146657dc08bac399764fe488a9</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_WARN</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga1e3c71175e1cfb16a658bf70afb840f0</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_INFO</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga25c1dbc5e47207e6b29673c15d817feb</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_DEBUG</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gad9b5ce484e6e5aeea0a1f231bc9cb05a</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MLOG_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2c0b6b38329f95e906198b7b23ef44dd</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_LEVEL</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga58ee0f237f34bb12ecd3f3563bae2b43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_TURN_OFF_LOGGING</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43aacb3fe0cbf1a107f7cf442a4390dfc51</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_ERROR</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43a987dfdab40fb4cff7066880de918ac46</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_WARN</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43ab654f698fdb86e39d3a055ca607b0392</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_INFO</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43a8ffc43a57f8b07b226fcb7e8ddf0cf84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_DEBUG</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43afab730f69616ab81b856a263f771f572</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_LEVEL_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga58ee0f237f34bb12ecd3f3563bae2b43ad2199d8488ecbea5ae4912eccb28ff7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_ACTION</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga4a5ea034951d1f43cb4e83681ada1450</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_TO_FILE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a352870a22c79768f7266dede995bef49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_SYSLOG_TRACE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a9420de7fcaf1bcf0ef84b2f90e94f397</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ACTION_CONSOLE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga4a5ea034951d1f43cb4e83681ada1450a28b468eda2e029c2e9ae441911b6aea6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_PREFIX</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga2d7c021b73763bff61683052e2e1e695</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_LOG_LEVEL</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695afec2d2208fccc9c49688e81d39c15969</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_SOURCE_FILE</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695aa01d063ac6bbda5767f8ea78ae53d98f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_PREFIX_TIME</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga2d7c021b73763bff61683052e2e1e695a14cbf3164f8c23ee45f099bb7cc3f6ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>MLOG_ALLOC</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga64789137cb6d20255c4ae2f694860956</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_STACK</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956aa30f577c355d512b02dc3ad0c1b5b3c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_HEAP</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956a0417454d43c698e83982f3544ae84e28</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MLOG_ALLOC_TLS_HEAP</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gga64789137cb6d20255c4ae2f694860956a8bb0a30df2cc7447c02f3f72a09109d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_init</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga33c82ea4ab2a41cdea9054948c9b3ec5</anchor>
      <arglist>(MLOG_LEVEL current, MLOG_ACTION action, void *arg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MLOG_set_prefix_features</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaf54b9c4ac40e86f37fde4a347eda9413</anchor>
      <arglist>(int prefix)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_alloc_option</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gafda3d5be4bd7fe8938c87e71e835f52a</anchor>
      <arglist>(MLOG_ALLOC option, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MLOG_dump_stack_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gaec863161f6870f16f8d9934d83e61e6d</anchor>
      <arglist>(MLOG_LEVEL stack_dump_level)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MLOG_printf</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga0150a13359adcc1be1d1aa0f6c136d80</anchor>
      <arglist>(MLOG_LEVEL current, const char *file, int line, const char *format,...)</arglist>
    </member>
    <member kind="variable">
      <type>MLOG_LEVEL</type>
      <name>m_current_log_level</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga1b7a2c0199f158ae15f81e61ec18ad45</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STRTK</name>
    <title>STRTK</title>
    <filename>a00016.html</filename>
    <class kind="struct">tagSTRTK</class>
    <member kind="define">
      <type>#define</type>
      <name>STRTK_IS_CHAR</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga6af25e1d072d3e1ed7d7c0682831155c</anchor>
      <arglist>(tok, ch)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTRTK</type>
      <name>STRTK</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga76f725215889db6e091bec0b60be9d33</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>STRTK_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5e8e0f7718ea406cdc754e4d6d9f40fd</anchor>
      <arglist>(STRTK *tok, const char *stop_chars)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>STRTK_add_char</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gae43120a54e8902f8bd356ee57a9fc7de</anchor>
      <arglist>(STRTK *tok, uint8_t ch)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_spn</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gacbaa999a7ed8334ee4f9c074190522f5</anchor>
      <arglist>(STRTK *tok, const char *hay)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_nspn</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gaa298a57dd2c32290c6f0d7932d73b031</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_ntok</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gaf5f9a2448a798742f4c632df8207bee6</anchor>
      <arglist>(STRTK *tok, const char *hay, size_t nsize, char **end_tok)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>STRTK_tok</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga970da7da688e9d0412ef1e688b340f32</anchor>
      <arglist>(STRTK *tok, const char *hay, char **end_tok)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>TOKENDEF</name>
    <title>TOKENDEF</title>
    <filename>a00017.html</filename>
    <class kind="struct">tagTOKENDEF</class>
    <member kind="typedef">
      <type>struct tagTOKENDEF</type>
      <name>TOKENDEF</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga31b87f2e9234f4d8a6844ffa2a401efa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>parse_token</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gadbb6d1869c2d60a2e4f07218472d0039</anchor>
      <arglist>(const char *token, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>token_string_for_int</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga2720d8525084394b4961286ba62371e5</anchor>
      <arglist>(int value, TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>token_possible_values</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga32074c05714bf643b440ca28b556a998</anchor>
      <arglist>(TOKENDEF *tkdef)</arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>type</name>
      <anchorfile>a00017.html</anchorfile>
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
      <anchorfile>a00017.html</anchorfile>
      <anchor>gab7bce27d71c22f9e38d80845e52f5142</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
