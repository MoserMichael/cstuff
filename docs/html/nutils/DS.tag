<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>sock.c</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00002</filename>
    <includes id="a00003" name="sock.h" local="yes" imported="no">sock.h</includes>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga4d44efb92c4cd8cc45d2f7434d0f34bf</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga29f9645369f83530169a0009c19ed35c</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *bmsg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sock.h</name>
    <path>/home/mmoser/archive/cstuff_bak/nutils/</path>
    <filename>a00003</filename>
    <class kind="struct">tagSOCKCTX</class>
    <member kind="define">
      <type>#define</type>
      <name>LINGER_OPTION_VALUE</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga3adf1d96e791aba989b3bcd0ff855d44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKCTX_FLAGS_NAGLE_ON</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga387cd63d9202db79a473a4312e50a6d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga4e5ab8d07bf7233fe5358f74f89192d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSOCKCTX</type>
      <name>SOCKCTX</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga33ae627ac286035a2b486c6ab593d8af</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga29f9645369f83530169a0009c19ed35c</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga4d44efb92c4cd8cc45d2f7434d0f34bf</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SOCKET</name>
    <title>SOCKET</title>
    <filename>a00004.html</filename>
    <class kind="struct">tagSOCKCTX</class>
    <member kind="define">
      <type>#define</type>
      <name>LINGER_OPTION_VALUE</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga3adf1d96e791aba989b3bcd0ff855d44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKCTX_FLAGS_NAGLE_ON</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga387cd63d9202db79a473a4312e50a6d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SOCKTCX_FLAGS_DONT_CLOSE_ON_PEER_CLOSE</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga4e5ab8d07bf7233fe5358f74f89192d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSOCKCTX</type>
      <name>SOCKCTX</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga33ae627ac286035a2b486c6ab593d8af</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_init</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa58093540ffae6b2fd5114af70ea9249</anchor>
      <arglist>(SOCKCTX *ctx, int verbose, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send_buffer_sizes</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga691ca5d5947940ef7bb8e05f5f4381ee</anchor>
      <arglist>(SOCKCTX *ctx, int read_buffer_size, int write_buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_connect</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga9a6c6d2ae46cdab6fe52e285f9a8990d</anchor>
      <arglist>(SOCKCTX *ctx, void *addr, int addr_size, int connect_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga29f9645369f83530169a0009c19ed35c</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_recv_all</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ga4d44efb92c4cd8cc45d2f7434d0f34bf</anchor>
      <arglist>(SOCKCTX *ctx, char *msg, size_t length, int read_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_send</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gabbd75054a6005f6da12fa57a9032495a</anchor>
      <arglist>(SOCKCTX *ctx, void *msg, size_t length, int write_timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gad78589add776d85bd1c2e25a9d960eef</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>SOCK_close_with_reset</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>gaa4e7c6427b947269086042413e060ed3</anchor>
      <arglist>(SOCKCTX *ctx)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSOCKCTX</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>verbose</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>aaa34bd395f2c627e0497599095582550</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>connected</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a5283ea4aa0d2447d230e37074a0589ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>close_on_peer_close</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a21ecb44f25561d43dd4b154348562d6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>fd</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>addcdd353858c7738ccde887e2fa48501</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>addr</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a3d6d0b0910922102ef30609abf8e9abf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>addr_size</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a090065b6dfa838ecdf5ca535d6befa24</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
