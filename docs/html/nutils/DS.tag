<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>addrutil.c</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00006</filename>
    <includes id="a00007" name="addrutil.h" local="yes" imported="no">addrutil.h</includes>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_any</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga8bf8fe08abbc29ba2dc575b969aea8f8</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IPADDRESS_broadcast_ipv4</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gabcbd13c3fd7439ec933ed3ecb3a661cf</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_loopback</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga166e784e109da4273bbe671ec99a0fbb</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_parse_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga420fe3e499516aaa19b3001e10b0405e</anchor>
      <arglist>(IPADDRESS *addr, const char *str)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>IPADDRESS_to_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga61d42e251f3be7d8eb7b542b2faf8b29</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gac2567e6afab6202d7730db868ea61edd</anchor>
      <arglist>(SOCKADDR *saddr, IPADDRESS *addr, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>socklen_t</type>
      <name>SOCKADDR_length</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga90e39c77eee09be79644a9b922f0133d</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_family</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga528e22bad3feab1129fe4751b0fe5347</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>SOCKADDR_ipaddr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5fbd817c5f4de91869ec7dbd2fcba71f</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>SOCKADDR_to_string</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga584b051d6830e5c7dbef4238a7bddc17</anchor>
      <arglist>(SOCKADDR *saddr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>addrutil.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00007</filename>
    <class kind="struct">IPADDRESS</class>
    <class kind="union">SOCKADDR</class>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_any</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga8bf8fe08abbc29ba2dc575b969aea8f8</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_loopback</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga166e784e109da4273bbe671ec99a0fbb</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IPADDRESS_broadcast_ipv4</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gabcbd13c3fd7439ec933ed3ecb3a661cf</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_parse_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga420fe3e499516aaa19b3001e10b0405e</anchor>
      <arglist>(IPADDRESS *addr, const char *str)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>IPADDRESS_to_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga61d42e251f3be7d8eb7b542b2faf8b29</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>IPADDRESS_family</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gad3342d7efb48d719ca80058b70933d43</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gac2567e6afab6202d7730db868ea61edd</anchor>
      <arglist>(SOCKADDR *saddr, IPADDRESS *addr, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_family</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga528e22bad3feab1129fe4751b0fe5347</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>SOCKADDR_ipaddr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5fbd817c5f4de91869ec7dbd2fcba71f</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE struct sockaddr *</type>
      <name>SOCKADDR_saddr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gacdc79ddce2ab4d38601e79b011da9f36</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>socklen_t</type>
      <name>SOCKADDR_length</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga90e39c77eee09be79644a9b922f0133d</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>SOCKADDR_to_string</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga584b051d6830e5c7dbef4238a7bddc17</anchor>
      <arglist>(SOCKADDR *saddr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>bf.c</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00008</filename>
    <includes id="a00009" name="bf.h" local="yes" imported="no">bf.h</includes>
    <member kind="function">
      <type>int</type>
      <name>BF_compact</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga37819d2886490c212763c865c33364db</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gae27decac1bfa136183b23cea293464a2</anchor>
      <arglist>(BF *bf, int eof)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line_ext</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga42852c881989c7925a70e7fa3a87ee6c</anchor>
      <arglist>(BF *bf, const char *eof_line, size_t eof_line_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_check</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga9a5012a3895ade56ca7b1a8bb7309c72</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_start</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga18fdd2d33720298f25059429d226d9b2</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_end</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga56338e4b6e732eea198fb266dfa55198</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_get</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaacf9d4de524e47e30360e243d39c0f73</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_put</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga3d4956583a84d460393dd68dca32f6ac</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>bf.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00009</filename>
    <includes id="a00010" name="endutil.h" local="no" imported="no">nutils/endutil.h</includes>
    <class kind="struct">tagBF</class>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_PUT_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga1b2a5ce67eaea3e8012a2552924c4d7b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_GET_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaef54a2ad2476088707dd143fd0c36c6e</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_PUT_UNSAFE_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga543fba03a17673a12a4b217faa808b05</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_GET_UNSAFE_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga94ea6669521af770786a977fb521707b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagBF</type>
      <name>BF</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gac141d99aa0f78cafd2acf074517a38cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_init</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gae0c0c2e9bd1e5315aead6fe6554b55b2</anchor>
      <arglist>(BF *bf, void *buffer, size_t buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_check</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga9a5012a3895ade56ca7b1a8bb7309c72</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_get_mode</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga4ef6aded05e4a4d6669d269775573d3f</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_put_mode</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga26e937b842b116933b58d6a1dd6e6b3a</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_get_size</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaed0d381d2cbad849c7949245ad986f40</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_put_size</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gadf903eda486452455390dade47cd9898</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_start</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga18fdd2d33720298f25059429d226d9b2</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_end</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga56338e4b6e732eea198fb266dfa55198</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_get</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaacf9d4de524e47e30360e243d39c0f73</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_put</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga3d4956583a84d460393dd68dca32f6ac</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_can_get</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga95f015cf316f26ce7d0ae42feef8a45f</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_can_put</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga9e4f956930808566d42dd85d4b0ce24d</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>BF_is_full</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gafb9836bdbbbd3b80a05a72705b256dbd</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_compact</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga37819d2886490c212763c865c33364db</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gae27decac1bfa136183b23cea293464a2</anchor>
      <arglist>(BF *bf, int eof_line)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line_ext</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga42852c881989c7925a70e7fa3a87ee6c</anchor>
      <arglist>(BF *bf, const char *eof_line, size_t eof_line_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>BF_putn</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga225b6fc0269c02bba59cf099b9e480b2</anchor>
      <arglist>(BF *bf, void *data, size_t data_size)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>endutil.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00010</filename>
    <class kind="union">QUAD</class>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga726255f8ab5a5a70073ac3f3ab086839</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gaf368243836ff0d14473c1843df997767</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae404a6642bd93fb60e1bead5bc98fcec</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gac9e2c714095efb500ded4e9f89a8b2fa</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga6a21ec5233e121a1eae0098d77f35839</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga7e8a9e2d792b9d5e86bca73df54f5606</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga62e912357bd5958e11b6e7b2e9bab027</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga2f1a37771facd2363384ce2271e57611</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae8bed63462030f0808c9285d93282227</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga3cb4be5234746b5a549dce04434bb007</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga464eac4c5b9387c7ed39e41edc6f3cd9</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga1f036bd2a229c2beed12f7d2dcf9673e</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga5107680dae342ada3d353b2e6ee66cca</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gaedf98671d5859cac442b0056d94ed4d4</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint64_t</type>
      <name>n2h_uint64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga47c8a374922a6c8a3e1e41d2bcf9023e</anchor>
      <arglist>(uint64_t val)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint64_t</type>
      <name>h2n_uint64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae3c10aa4c4f3390f1c5dd323ba3be665</anchor>
      <arglist>(uint64_t val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ioutils.c</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00011</filename>
    <includes id="a00012" name="ioutils.h" local="yes" imported="no">ioutils.h</includes>
    <member kind="function">
      <type>int</type>
      <name>disable_sigpipe</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga87e2c01b17992795a3f9dc192ad79b87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_blocking</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6e6e3c7f96d02d8c98ef4ccc252915f0</anchor>
      <arglist>(int fd, int is_blocking)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_bytes_available</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3ff21ed80456eaef7e8c8d4664ce4b52</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_error</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gac28f199e5ff2472676ce666c2b26bd1b</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6c87321a99f019ef47f9b54e37011b56</anchor>
      <arglist>(int fd, Buffsize_op op, int size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaed555d93fa05a5296893eea63e3f0f1d</anchor>
      <arglist>(int fd, Buffsize_op op)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaf3fdd881c27e4e151b8416655a078d06</anchor>
      <arglist>(int fd, int reuse_on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga58ef368196b8397a2d24b225f325ab49</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_nagling</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga9d2212efa865252b2cc1f81f006f1af1</anchor>
      <arglist>(int fd, int on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_linger_option</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa78ceabb6e37ea92f269eea695729e7f</anchor>
      <arglist>(int fd, int on, int linger_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_close_by_RST</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa222e4e337f8a8a1a9978291f496a7b6</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_make_tcp_listener</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gab59c99d141b6899eec4d6cf43a1b64c5</anchor>
      <arglist>(SOCKADDR *saddr, int backlog)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ioutils.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00012</filename>
    <includes id="a00007" name="addrutil.h" local="no" imported="no">nutils/addrutil.h</includes>
    <member kind="typedef">
      <type>enum _Buffsize_op</type>
      <name>Buffsize_op</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaba11ab2fe46802ab794df85e4c6dd502</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_Buffsize_op</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaee27e07f008622fa5249794ed32b926f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Receive_buffer</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ggaee27e07f008622fa5249794ed32b926fa31333bbcaf5149721fd74541a44396b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Send_buffer</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ggaee27e07f008622fa5249794ed32b926fa94c598772118c50c0f21482fab49178c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>disable_sigpipe</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga87e2c01b17992795a3f9dc192ad79b87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_blocking</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6e6e3c7f96d02d8c98ef4ccc252915f0</anchor>
      <arglist>(int fd, int is_blocking)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_bytes_available</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3ff21ed80456eaef7e8c8d4664ce4b52</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_error</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gac28f199e5ff2472676ce666c2b26bd1b</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6c87321a99f019ef47f9b54e37011b56</anchor>
      <arglist>(int fd, Buffsize_op op, int size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaed555d93fa05a5296893eea63e3f0f1d</anchor>
      <arglist>(int fd, Buffsize_op op)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaf3fdd881c27e4e151b8416655a078d06</anchor>
      <arglist>(int fd, int reuse_on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga58ef368196b8397a2d24b225f325ab49</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_nagling</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga9d2212efa865252b2cc1f81f006f1af1</anchor>
      <arglist>(int fd, int on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_linger_option</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa78ceabb6e37ea92f269eea695729e7f</anchor>
      <arglist>(int fd, int on, int linger_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_close_by_RST</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa222e4e337f8a8a1a9978291f496a7b6</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_make_tcp_listener</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gab59c99d141b6899eec4d6cf43a1b64c5</anchor>
      <arglist>(SOCKADDR *saddr, int backlog)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sock.c</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00013</filename>
    <includes id="a00014" name="sock.h" local="yes" imported="no">sock.h</includes>
    <includes id="a00012" name="ioutils.h" local="yes" imported="no">ioutils.h</includes>
    <member kind="function">
      <type>int</type>
      <name>SOCK_attach</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga00e3c145efbdb0861247b6f3fe61a870</anchor>
      <arglist>(SOCKCTX *ctx, int fd, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga94bdf0c4fa82a188848fc5b63022a258</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gafa03c58e51fbfd0ed3d659f3830e046a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *bmsg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sock.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00014</filename>
    <class kind="struct">tagSOCKCTX</class>
    <member kind="define">
      <type>#define</type>
      <name>LINGER_OPTION_VALUE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga3adf1d96e791aba989b3bcd0ff855d44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKCTX_FLAGS_NAGLE_ON</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga387cd63d9202db79a473a4312e50a6d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga4e5ab8d07bf7233fe5358f74f89192d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSOCKCTX</type>
      <name>SOCKCTX</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga33ae627ac286035a2b486c6ab593d8af</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_attach</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga00e3c145efbdb0861247b6f3fe61a870</anchor>
      <arglist>(SOCKCTX *ctx, int fd, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga94bdf0c4fa82a188848fc5b63022a258</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gafa03c58e51fbfd0ed3d659f3830e046a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>IPADDRESS</name>
    <title>IPADDRESS</title>
    <filename>a00015.html</filename>
    <class kind="struct">IPADDRESS</class>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_any</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga8bf8fe08abbc29ba2dc575b969aea8f8</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_loopback</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga166e784e109da4273bbe671ec99a0fbb</anchor>
      <arglist>(IPADDRESS *addr, int family)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IPADDRESS_broadcast_ipv4</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gabcbd13c3fd7439ec933ed3ecb3a661cf</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>IPADDRESS_parse_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga420fe3e499516aaa19b3001e10b0405e</anchor>
      <arglist>(IPADDRESS *addr, const char *str)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>IPADDRESS_to_string</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>ga61d42e251f3be7d8eb7b542b2faf8b29</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>IPADDRESS_family</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>gad3342d7efb48d719ca80058b70933d43</anchor>
      <arglist>(IPADDRESS *addr)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SOCKADDR</name>
    <title>SOCKADDR</title>
    <filename>a00016.html</filename>
    <class kind="union">SOCKADDR</class>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_init</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gac2567e6afab6202d7730db868ea61edd</anchor>
      <arglist>(SOCKADDR *saddr, IPADDRESS *addr, uint16_t port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCKADDR_family</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga528e22bad3feab1129fe4751b0fe5347</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>SOCKADDR_ipaddr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga5fbd817c5f4de91869ec7dbd2fcba71f</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE struct sockaddr *</type>
      <name>SOCKADDR_saddr</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>gacdc79ddce2ab4d38601e79b011da9f36</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>socklen_t</type>
      <name>SOCKADDR_length</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga90e39c77eee09be79644a9b922f0133d</anchor>
      <arglist>(SOCKADDR *addr)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>SOCKADDR_to_string</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>ga584b051d6830e5c7dbef4238a7bddc17</anchor>
      <arglist>(SOCKADDR *saddr)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>BF</name>
    <title>BF</title>
    <filename>a00017.html</filename>
    <class kind="struct">tagBF</class>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_PUT_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga1b2a5ce67eaea3e8012a2552924c4d7b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_GET_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaef54a2ad2476088707dd143fd0c36c6e</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_PUT_UNSAFE_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga543fba03a17673a12a4b217faa808b05</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BF_DECLARE_GET_UNSAFE_MACRO</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga94ea6669521af770786a977fb521707b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagBF</type>
      <name>BF</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gac141d99aa0f78cafd2acf074517a38cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_init</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gae0c0c2e9bd1e5315aead6fe6554b55b2</anchor>
      <arglist>(BF *bf, void *buffer, size_t buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_check</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga9a5012a3895ade56ca7b1a8bb7309c72</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_get_mode</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga4ef6aded05e4a4d6669d269775573d3f</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>BF_put_mode</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga26e937b842b116933b58d6a1dd6e6b3a</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_get_size</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaed0d381d2cbad849c7949245ad986f40</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_put_size</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gadf903eda486452455390dade47cd9898</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_start</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga18fdd2d33720298f25059429d226d9b2</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_end</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga56338e4b6e732eea198fb266dfa55198</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_get</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gaacf9d4de524e47e30360e243d39c0f73</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_set_put</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga3d4956583a84d460393dd68dca32f6ac</anchor>
      <arglist>(BF *bf, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_can_get</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga95f015cf316f26ce7d0ae42feef8a45f</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>BF_can_put</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga9e4f956930808566d42dd85d4b0ce24d</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>BF_is_full</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gafb9836bdbbbd3b80a05a72705b256dbd</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>BF_compact</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga37819d2886490c212763c865c33364db</anchor>
      <arglist>(BF *bf)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>gae27decac1bfa136183b23cea293464a2</anchor>
      <arglist>(BF *bf, int eof_line)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>BF_get_line_ext</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga42852c881989c7925a70e7fa3a87ee6c</anchor>
      <arglist>(BF *bf, const char *eof_line, size_t eof_line_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>BF_putn</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>ga225b6fc0269c02bba59cf099b9e480b2</anchor>
      <arglist>(BF *bf, void *data, size_t data_size)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>ENDIAN</name>
    <title>ENDIAN</title>
    <filename>a00018.html</filename>
    <class kind="union">QUAD</class>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga726255f8ab5a5a70073ac3f3ab086839</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gaf368243836ff0d14473c1843df997767</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae404a6642bd93fb60e1bead5bc98fcec</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gac9e2c714095efb500ded4e9f89a8b2fa</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_uint32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga6a21ec5233e121a1eae0098d77f35839</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga7e8a9e2d792b9d5e86bca73df54f5606</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>n2h_int64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga62e912357bd5958e11b6e7b2e9bab027</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga2f1a37771facd2363384ce2271e57611</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int8_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae8bed63462030f0808c9285d93282227</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga3cb4be5234746b5a549dce04434bb007</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int16_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga464eac4c5b9387c7ed39e41edc6f3cd9</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_uint32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga1f036bd2a229c2beed12f7d2dcf9673e</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int32_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga5107680dae342ada3d353b2e6ee66cca</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>h2n_int64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gaedf98671d5859cac442b0056d94ed4d4</anchor>
      <arglist>(val)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint64_t</type>
      <name>n2h_uint64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ga47c8a374922a6c8a3e1e41d2bcf9023e</anchor>
      <arglist>(uint64_t val)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint64_t</type>
      <name>h2n_uint64_t</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>gae3c10aa4c4f3390f1c5dd323ba3be665</anchor>
      <arglist>(uint64_t val)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>IOUTILS</name>
    <title>IOUTILS</title>
    <filename>a00019.html</filename>
    <member kind="typedef">
      <type>enum _Buffsize_op</type>
      <name>Buffsize_op</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaba11ab2fe46802ab794df85e4c6dd502</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_Buffsize_op</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaee27e07f008622fa5249794ed32b926f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Receive_buffer</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ggaee27e07f008622fa5249794ed32b926fa31333bbcaf5149721fd74541a44396b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Send_buffer</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ggaee27e07f008622fa5249794ed32b926fa94c598772118c50c0f21482fab49178c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>disable_sigpipe</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga87e2c01b17992795a3f9dc192ad79b87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_blocking</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6e6e3c7f96d02d8c98ef4ccc252915f0</anchor>
      <arglist>(int fd, int is_blocking)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_bytes_available</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga3ff21ed80456eaef7e8c8d4664ce4b52</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_error</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gac28f199e5ff2472676ce666c2b26bd1b</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga6c87321a99f019ef47f9b54e37011b56</anchor>
      <arglist>(int fd, Buffsize_op op, int size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_buf_size</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaed555d93fa05a5296893eea63e3f0f1d</anchor>
      <arglist>(int fd, Buffsize_op op)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaf3fdd881c27e4e151b8416655a078d06</anchor>
      <arglist>(int fd, int reuse_on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_get_reuse_address</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga58ef368196b8397a2d24b225f325ab49</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_nagling</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>ga9d2212efa865252b2cc1f81f006f1af1</anchor>
      <arglist>(int fd, int on)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_set_linger_option</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa78ceabb6e37ea92f269eea695729e7f</anchor>
      <arglist>(int fd, int on, int linger_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_close_by_RST</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gaa222e4e337f8a8a1a9978291f496a7b6</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fd_make_tcp_listener</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>gab59c99d141b6899eec4d6cf43a1b64c5</anchor>
      <arglist>(SOCKADDR *saddr, int backlog)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SOCKET</name>
    <title>SOCKET</title>
    <filename>a00020.html</filename>
    <class kind="struct">tagSOCKCTX</class>
    <member kind="define">
      <type>#define</type>
      <name>LINGER_OPTION_VALUE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga3adf1d96e791aba989b3bcd0ff855d44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKCTX_FLAGS_NAGLE_ON</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga387cd63d9202db79a473a4312e50a6d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga4e5ab8d07bf7233fe5358f74f89192d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSOCKCTX</type>
      <name>SOCKCTX</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga33ae627ac286035a2b486c6ab593d8af</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_attach</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga00e3c145efbdb0861247b6f3fe61a870</anchor>
      <arglist>(SOCKCTX *ctx, int fd, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>ga94bdf0c4fa82a188848fc5b63022a258</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gafa03c58e51fbfd0ed3d659f3830e046a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>IPADDRESS</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>family</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a737ed7b18819a9ff2c7cddaaaab0d671</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union IPADDRESS::@0</type>
      <name>addr</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>aef3b517834c8a356aa1b50a65540fd16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct in_addr</type>
      <name>ipv4</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a5bcb9e77b2836175b18e0e5548f28dfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct in6_addr</type>
      <name>ipv6</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a5dc190dcf8f6cf51be15501254eb6c67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>QUAD</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>struct QUAD::@1</type>
      <name>n32</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a6168536273a5629de492bf590bf646ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>word_a</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>a09c5a5ea9a532f6117fb409c667e7057</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>word_b</name>
      <anchorfile>a00025.html</anchorfile>
      <anchor>aabbcd8dc7162188d7560a88f7c5b5598</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>n64</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a70b5746bfc7e6c117f070976f6721e33</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="union">
    <name>SOCKADDR</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>struct sockaddr_in6</type>
      <name>addr_ipv6</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>af0685557dc0ec6dcd9897692a99586b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct sockaddr_in</type>
      <name>addr_ipv4</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ae196aa6f30f3a9906d40dd33b073b723</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagBF</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>bf</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a8c4f938c3bda8c2f8c7642c285cd5d07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>bf_size</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a709816af50943e6e155436fba2fc892c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>start</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ad13dc2d65892a509ccd131031b401a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>get_pos</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>aa86484010e3b771e8dd1fe54cc8a0318</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>put_pos</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ac9b79e4f9d3b631ff510229a4e6bf778</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>end</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a8bad05ec0bc08aa74b2cbf13a7c9aac5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSOCKCTX</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>verbose</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>aaa34bd395f2c627e0497599095582550</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>connected</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a5283ea4aa0d2447d230e37074a0589ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>close_on_peer_close</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a21ecb44f25561d43dd4b154348562d6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>fd</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>addcdd353858c7738ccde887e2fa48501</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>addr</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a3d6d0b0910922102ef30609abf8e9abf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>addr_size</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a090065b6dfa838ecdf5ca535d6befa24</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
