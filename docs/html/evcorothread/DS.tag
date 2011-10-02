<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>evthread.c</name>
    <path>/home/mmoser/archive/cstuff_bak/evcorothread/</path>
    <filename>a00007</filename>
    <includes id="a00008" name="evthread.h" local="no" imported="no">evthread.h</includes>
    <member kind="function">
      <type>EVLOOP *</type>
      <name>EVLOOP_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gac2c9758c338da8cf0bd5692db7547564</anchor>
      <arglist>(STACKS *stacks)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_run</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gaa25fcfb07b6227a4749e2d148bb053b3</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_break</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6886f671cea83756af4d537b31c53436</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_free</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>af6327bf0e0278d10f1ff67a1886359af</anchor>
      <arglist>(EVTHREAD *thread)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>evthread_proc</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ad705fb6f9cf66a02d338f10a77f377e7</anchor>
      <arglist>(void *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>thread_timer_cb</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a205141cb4354a9ea6d692c30c0a62bd4</anchor>
      <arglist>(int fd, short event, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>EVTHREAD *</type>
      <name>EVTHREAD_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gaa23e69897b0c289d7da8f8d10435a140</anchor>
      <arglist>(EVLOOP *loop, EVTHREAD_PROC thread_proc, void *user_ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gad53dee62668086eb89016bb5b7e14fa5</anchor>
      <arglist>(EVTHREAD *thread, struct tagEVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_delay</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga51f061f7921e0bbce68d3a16e043a0ff</anchor>
      <arglist>(EVTHREAD *thread, struct timeval delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EVTHREAD_OBJECT_init</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aeb35db9a7b14e4fbc3f17f4879742b5b</anchor>
      <arglist>(EVTHREAD_OBJECT *obj, int type, EVTHREAD *owner)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EVTHREAD_OBJECT_free</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>afac4b846bbebc9e8bb60ec8c4d527eac</anchor>
      <arglist>(EVTHREAD_OBJECT *obj)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>ev_mktimer</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ae7d66724a3893b47b092d8ab33d1781d</anchor>
      <arglist>(EVTHREAD *thread, int timer_id, int is_recurrent, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_one_shot</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaa7702b36f1620f2526f4e57e7f4f90f4</anchor>
      <arglist>(EVTHREAD *thread, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_recurrent</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaf4c9953fa5cdd6b815c2f5e06aea2af6</anchor>
      <arglist>(EVTHREAD *thread, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_cb</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a9152704ad0af03f1ed087d0c20802fcf</anchor>
      <arglist>(int fd, short event, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_start</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9f5c178bcc529edd33a09632930bc7f7</anchor>
      <arglist>(EVTIMER *ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_cancel</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9108a7e52c0f3094b41fd0d681b3ef07</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_free</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga69cdac6c1ab58a63245fb14dfb43cc70</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_thread_proc</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ac7a8d32bdcb9305cc6a7b3fec9028bb7</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>socket_cb</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a291d3071c09facfdc2359fe2ca0fa1a5</anchor>
      <arglist>(int fd, short event, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>EVSOCKET *</type>
      <name>EVSOCKET_init</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2fd5da1f97d6ab55cc7c977d5759558e</anchor>
      <arglist>(EVTHREAD *thread, int fd, int is_connected)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_close</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga152040d20eb847259e1a5b129d08280d</anchor>
      <arglist>(EVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_timeout_proc</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a263766876e9dd593e9f37a825aa6bd3f</anchor>
      <arglist>(EVTIMER *timer, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_connect</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9611ce37f2fb0b914d624fcfeaf14045</anchor>
      <arglist>(EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EVSOCKET_set_idle_timeout</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9f1eadc43b53273402e301cbecd5af2f</anchor>
      <arglist>(EVSOCKET *socket, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2400bf324d24d3d0b011f32c7eceecda</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_send_internal</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>abcb6b7f63c1fe247e757aab22311098a</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv_all</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gae70737035f6cd136a85b6b24e31fe8be</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_send</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6c9dc88bb3fbe603473bc694664cdc7d</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>socket_listener_cb</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aaf2800e17c660488fc490769a930c43c</anchor>
      <arglist>(int fd, short event, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>EVTCPACCEPTOR *</type>
      <name>EVTCPACCEPTOR_init</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga529f6caf19c2f1a310eedffd93cfc9c4</anchor>
      <arglist>(EVLOOP *loop, int fd, EVTHREAD_FACTORY factory)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>custom_timeout_handling</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a42e820bb41711d718897b1989348fd0c</anchor>
      <arglist>(struct event_base *base, struct timeval *tv)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>evthread.h</name>
    <path>/home/mmoser/archive/cstuff_bak/evcorothread/</path>
    <filename>a00008</filename>
    <class kind="struct">tagEVLOOP</class>
    <class kind="struct">tagEVTHREAD</class>
    <class kind="struct">tagEVTHREAD_OBJECT</class>
    <class kind="struct">tagEVTIMER</class>
    <class kind="struct">tagEVSOCKET</class>
    <class kind="struct">tagTCPACCEPTOR</class>
    <member kind="typedef">
      <type>struct tagEVLOOP</type>
      <name>EVLOOP</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga962e10cc0631184c27e12a42a78e2f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>EVTHREAD_PROC</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a234cc8fab2090c3bf2314d41f23f66d5</anchor>
      <arglist>)(struct tagEVTHREAD *thread, struct tagEVSOCKET *socket, void *user_ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagEVTHREAD</type>
      <name>EVTHREAD</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gadbf6a6fe0e60307b58ee163176c1839a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagEVTHREAD_OBJECT</type>
      <name>EVTHREAD_OBJECT</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aec66d4972a98f0832cbb0426f7a2fb2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>EVTIMER_PROC</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gade7ae470b6904a89f3467e8535eb197f</anchor>
      <arglist>)(struct tagEVTIMER *timer, void *user_data)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagEVTIMER</type>
      <name>EVTIMER</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gac4efc7b102f8c909be276232fddd95e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagEVSOCKET</type>
      <name>EVSOCKET</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gaa657a3b712a0f84e9b7aeff7059d96a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>EVTHREAD_FACTORY</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga51673c2cea9c3ed041a2511977c04148</anchor>
      <arglist>)(int fd, EVTHREAD_PROC *proc, void **ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagTCPACCEPTOR</type>
      <name>EVTCPACCEPTOR</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga04e932952b59a41f70cbd49ce5680032</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EVTHREAD_OBJECT_TYPE</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a3cbb545676ecd40a740416851b4fff80</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTHREAD_OBJECT_SOCKET</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a3cbb545676ecd40a740416851b4fff80a2411a18bbeb8291e5d6db0f70e1d4cce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTHREAD_OBJECT_TIMER</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a3cbb545676ecd40a740416851b4fff80a6da017e2006cb13522323794dae0e2a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EVTIMERSTATE</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga2e6068a47dd625c7ac59d77a4615a181</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_INIT</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ad4b9bc14f751efa2be25f60604052ed8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_SCHEDULED</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ad61503039fa7c8bf1167420ef46f8cf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_HANDLER_RUNNING</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ae0c37a0c0a051faa089c7eab772da813</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EVSOCKET_STATE</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2c41f6288c8bc944dd898419b7fbbb0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_INIT</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca58af0fd1980fbcf50615f79a5f5e1bb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CONNECTING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca6fb24f7071ea4cae87a9fa24583dbff0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CONNECTED</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0caf309c7d0adb12a6b6c2db8568b2360a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_READING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca11d241b716a59cdbc6c07458fbefede5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_WRITING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca488819d6df0a5d336bec468b8e0ab8cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CLOSED</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca409d2782e34636815d4b096daa659f07</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_ERROR</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca9a409c3600e382e41ace36599966aec7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVLOOP *</type>
      <name>EVLOOP_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gac2c9758c338da8cf0bd5692db7547564</anchor>
      <arglist>(STACKS *stacks)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_run</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gaa25fcfb07b6227a4749e2d148bb053b3</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_break</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6886f671cea83756af4d537b31c53436</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
    <member kind="function">
      <type>EVTHREAD *</type>
      <name>EVTHREAD_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gaa23e69897b0c289d7da8f8d10435a140</anchor>
      <arglist>(EVLOOP *loop, EVTHREAD_PROC thread_proc, void *user_ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gad53dee62668086eb89016bb5b7e14fa5</anchor>
      <arglist>(EVTHREAD *thread, struct tagEVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_delay</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga51f061f7921e0bbce68d3a16e043a0ff</anchor>
      <arglist>(EVTHREAD *thread, struct timeval delay)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_one_shot</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaa7702b36f1620f2526f4e57e7f4f90f4</anchor>
      <arglist>(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_recurrent</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaf4c9953fa5cdd6b815c2f5e06aea2af6</anchor>
      <arglist>(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_start</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9f5c178bcc529edd33a09632930bc7f7</anchor>
      <arglist>(EVTIMER *ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_cancel</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9108a7e52c0f3094b41fd0d681b3ef07</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_free</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga69cdac6c1ab58a63245fb14dfb43cc70</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
    <member kind="function">
      <type>EVSOCKET *</type>
      <name>EVSOCKET_init</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2fd5da1f97d6ab55cc7c977d5759558e</anchor>
      <arglist>(EVTHREAD *thread, int fd, int is_connected)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_close</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga152040d20eb847259e1a5b129d08280d</anchor>
      <arglist>(EVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_connect</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9611ce37f2fb0b914d624fcfeaf14045</anchor>
      <arglist>(EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EVSOCKET_set_idle_timeout</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9f1eadc43b53273402e301cbecd5af2f</anchor>
      <arglist>(EVSOCKET *socket, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2400bf324d24d3d0b011f32c7eceecda</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv_all</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gae70737035f6cd136a85b6b24e31fe8be</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_send</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6c9dc88bb3fbe603473bc694664cdc7d</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>EVTCPACCEPTOR *</type>
      <name>EVTCPACCEPTOR_init</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga529f6caf19c2f1a310eedffd93cfc9c4</anchor>
      <arglist>(EVLOOP *loop, int fd, EVTHREAD_FACTORY factory)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>EVLOOP</name>
    <title>EVLOOP</title>
    <filename>a00009.html</filename>
    <class kind="struct">tagEVLOOP</class>
    <member kind="typedef">
      <type>struct tagEVLOOP</type>
      <name>EVLOOP</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga962e10cc0631184c27e12a42a78e2f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVLOOP *</type>
      <name>EVLOOP_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gac2c9758c338da8cf0bd5692db7547564</anchor>
      <arglist>(STACKS *stacks)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_run</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gaa25fcfb07b6227a4749e2d148bb053b3</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVLOOP_break</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6886f671cea83756af4d537b31c53436</anchor>
      <arglist>(EVLOOP *loop)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>EVTHREAD</name>
    <title>EVTHREAD</title>
    <filename>a00010.html</filename>
    <class kind="struct">tagEVTHREAD</class>
    <member kind="typedef">
      <type>struct tagEVTHREAD</type>
      <name>EVTHREAD</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gadbf6a6fe0e60307b58ee163176c1839a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVTHREAD *</type>
      <name>EVTHREAD_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gaa23e69897b0c289d7da8f8d10435a140</anchor>
      <arglist>(EVLOOP *loop, EVTHREAD_PROC thread_proc, void *user_ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gad53dee62668086eb89016bb5b7e14fa5</anchor>
      <arglist>(EVTHREAD *thread, struct tagEVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTHREAD_delay</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga51f061f7921e0bbce68d3a16e043a0ff</anchor>
      <arglist>(EVTHREAD *thread, struct timeval delay)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>EVTIMER</name>
    <title>EVTIMER</title>
    <filename>a00011.html</filename>
    <class kind="struct">tagEVTIMER</class>
    <member kind="typedef">
      <type>void(*</type>
      <name>EVTIMER_PROC</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gade7ae470b6904a89f3467e8535eb197f</anchor>
      <arglist>)(struct tagEVTIMER *timer, void *user_data)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagEVTIMER</type>
      <name>EVTIMER</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gac4efc7b102f8c909be276232fddd95e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EVTIMERSTATE</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga2e6068a47dd625c7ac59d77a4615a181</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_INIT</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ad4b9bc14f751efa2be25f60604052ed8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_SCHEDULED</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ad61503039fa7c8bf1167420ef46f8cf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVTIMER_STATE_HANDLER_RUNNING</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gga2e6068a47dd625c7ac59d77a4615a181ae0c37a0c0a051faa089c7eab772da813</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_one_shot</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaa7702b36f1620f2526f4e57e7f4f90f4</anchor>
      <arglist>(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>EVTIMER *</type>
      <name>EVTIMER_init_recurrent</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>gaf4c9953fa5cdd6b815c2f5e06aea2af6</anchor>
      <arglist>(EVTHREAD *loop, int timer_id, struct timeval tm, EVTIMER_PROC proc, void *user_data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_start</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9f5c178bcc529edd33a09632930bc7f7</anchor>
      <arglist>(EVTIMER *ret)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_cancel</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga9108a7e52c0f3094b41fd0d681b3ef07</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVTIMER_free</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ga69cdac6c1ab58a63245fb14dfb43cc70</anchor>
      <arglist>(EVTIMER *timer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>EVSOCKET</name>
    <title>EVSOCKET</title>
    <filename>a00012.html</filename>
    <class kind="struct">tagEVSOCKET</class>
    <member kind="typedef">
      <type>struct tagEVSOCKET</type>
      <name>EVSOCKET</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gaa657a3b712a0f84e9b7aeff7059d96a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EVSOCKET_STATE</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2c41f6288c8bc944dd898419b7fbbb0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_INIT</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca58af0fd1980fbcf50615f79a5f5e1bb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CONNECTING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca6fb24f7071ea4cae87a9fa24583dbff0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CONNECTED</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0caf309c7d0adb12a6b6c2db8568b2360a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_READING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca11d241b716a59cdbc6c07458fbefede5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_WRITING</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca488819d6df0a5d336bec468b8e0ab8cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_CLOSED</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca409d2782e34636815d4b096daa659f07</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVSOCKET_STATE_ERROR</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gga2c41f6288c8bc944dd898419b7fbbb0ca9a409c3600e382e41ace36599966aec7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVSOCKET *</type>
      <name>EVSOCKET_init</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2fd5da1f97d6ab55cc7c977d5759558e</anchor>
      <arglist>(EVTHREAD *thread, int fd, int is_connected)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_close</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga152040d20eb847259e1a5b129d08280d</anchor>
      <arglist>(EVSOCKET *socket)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_connect</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9611ce37f2fb0b914d624fcfeaf14045</anchor>
      <arglist>(EVSOCKET *socket, struct sockaddr *address, socklen_t socklen, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EVSOCKET_set_idle_timeout</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga9f1eadc43b53273402e301cbecd5af2f</anchor>
      <arglist>(EVSOCKET *socket, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga2400bf324d24d3d0b011f32c7eceecda</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_recv_all</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>gae70737035f6cd136a85b6b24e31fe8be</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>EVSOCKET_send</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>ga6c9dc88bb3fbe603473bc694664cdc7d</anchor>
      <arglist>(EVSOCKET *socket, void *buf, size_t buf_size, int flags, struct timeval timeout)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>TCPACCEPTOR</name>
    <title>TCPACCEPTOR</title>
    <filename>a00013.html</filename>
    <class kind="struct">tagTCPACCEPTOR</class>
    <member kind="typedef">
      <type>int(*</type>
      <name>EVTHREAD_FACTORY</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga51673c2cea9c3ed041a2511977c04148</anchor>
      <arglist>)(int fd, EVTHREAD_PROC *proc, void **ctx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagTCPACCEPTOR</type>
      <name>EVTCPACCEPTOR</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga04e932952b59a41f70cbd49ce5680032</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>EVTCPACCEPTOR *</type>
      <name>EVTCPACCEPTOR_init</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>ga529f6caf19c2f1a310eedffd93cfc9c4</anchor>
      <arglist>(EVLOOP *loop, int fd, EVTHREAD_FACTORY factory)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagEVLOOP</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>struct event_base *</type>
      <name>ev_base</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a3ef46ca89266616a4ca06b62e60e8062</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>STACKS *</type>
      <name>stacks</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>abc5af16a5c530aebd2015bbec8756a52</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagEVSOCKET</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>EVTHREAD_OBJECT</type>
      <name>object_base</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>aa2f8699d28dc94e1acadc99a1d0aab2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>fd</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ac4f60cb252957d83ab97420d9ba75a7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct event</type>
      <name>read_event</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a424a834569ee2593b2d04b6e6ae9799b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct event</type>
      <name>write_event</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a25b5c3a94163a89a7bc0dff0dbdeb277</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVSOCKET_STATE</type>
      <name>state</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a8841e3dec607bf1eaeee342eda681adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTIMER *</type>
      <name>timer_idle_timeout</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a25d6aeb264edc059926e412bbf0d8900</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTIMER *</type>
      <name>timer_io_timeout</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ad73f5d6d015fd3f464b9e6c1894e9836</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVLOOP *</type>
      <name>loop</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ab7a9905daf7aa5dc109ac87e5dbb33d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTHREAD *</type>
      <name>thread</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ac7bd1c84d925b7c459220750b47d3d09</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagEVTHREAD</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>EVLOOP *</type>
      <name>loop</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>abd1b6bd8c48500d617d9779fcac65cdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTHREAD_PROC</type>
      <name>thread_proc</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>abb9ebfa4591c3e6538abe7aa8c7cc8ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>user_context</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>afc9604c7677fa92279f144561a816438</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CTHREAD *</type>
      <name>cthread</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>add57a915bbd0ab5ee1ba23f2f000ced6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagEVSOCKET *</type>
      <name>socket</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>acbc4c1ff508c758eb16d77dc9729fd3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct event</type>
      <name>timer_event</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>acd108dfbd9acff91f9f19be88df46369</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLIST</type>
      <name>object_list</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ab6d295983287160d97863ce19955cb34</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagEVTHREAD_OBJECT</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>DLIST_entry</type>
      <name>entry</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a55a3a585eee64b4f19b168df6388523d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>object_type</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a3e9332299f5b805b4c57003aef3d44a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTHREAD *</type>
      <name>owner</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a6b5d3c3169d6b9e8770d15c92dc5b81f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagEVTIMER</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>EVTHREAD_OBJECT</type>
      <name>object_base</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a56cb98d864f6dcbdc1d95c81b6fc5270</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVLOOP *</type>
      <name>loop</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a742e8b7259a65ba5a186a7298e4b191c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTIMER_PROC</type>
      <name>proc</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a4336a1004a67d947772edcf5151f84a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>user_data</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a060c8465c9ad91aa4690d86ef2029623</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>timer_id</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>ab0896ef55a4486c2555d8e8e851e8232</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct event</type>
      <name>timer_event</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a11f2b0cfebce383190acf1ca2e435240</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct timeval</type>
      <name>timer_period</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a539b7675429899de8dd3a18b237fc872</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_recurrent</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a76a08cfecc3cb0d919473a0d10aa3d22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct timeval</type>
      <name>tm</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a43ce976f10a5015e918295d8f6950046</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct timeval</type>
      <name>next_due_date</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a60f4469b2b053810d66cea5b76196f6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTIMERSTATE</type>
      <name>state</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>ac2e80bcde19b90fdc4f0c986c370e592</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTCPACCEPTOR</name>
    <filename>a00006.html</filename>
    <member kind="variable">
      <type>EVLOOP *</type>
      <name>loop</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>aa4067896ed166c8c989986d53c22d278</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>fd</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a4606db4461a29987675ef98ff69f96d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct event</type>
      <name>read_event</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a85a6ecdfac5b71883114be1a3b554cd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EVTHREAD_FACTORY</type>
      <name>factory</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>af8f56cf09d1124217ec73e367348662f</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
