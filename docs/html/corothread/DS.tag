<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>cthread.c</name>
    <path>/home/mmoser/archive/cstuff_bak/corothread/</path>
    <filename>a00005</filename>
    <includes id="a00006" name="cthread.h" local="yes" imported="no">cthread.h</includes>
    <member kind="typedef">
      <type>void(*</type>
      <name>MK_CTX_FUNC</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a26b2b60856b3116d3fa35ecb96564380</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="function">
      <type>CRETURN_POINT *</type>
      <name>CRETURN_POINT_init</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a25102758c4164a5b978062107b2f1c01</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_libinit</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0592112b80e974a450345f78c1aaf42b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cthread_init</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a942a3ddb28807653f25219ce07d1a0f0</anchor>
      <arglist>(CTHREAD *arg)</arglist>
    </member>
    <member kind="function">
      <type>CTHREAD *</type>
      <name>CTHREAD_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga92dd60a37f2db43fd9a55eef3da21bc7</anchor>
      <arglist>(STACKS *stacks, CTHREAD_PROC proc, void *ctx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>do_start</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a8dcb24ceb1c69153bb683b05eccd02f9</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga1423b211106739007fc916f1bac9a3a6</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start_ex</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gabad990037ec5121d988baabefba4729e</anchor>
      <arglist>(CTHREAD *thread, CRETURN_POINT *next)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_join</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafb48d6856e2d6ca97593523fb7894382</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_resume</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0d7806af6c4043f1c5aaa96c8975ba06</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_yield</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafa9ace97052a6ddb5764826b4e0a4b59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_free</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gae4767f31d1375f7edfa267ad62bf8ea3</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_set_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6c262644b9961eca998ad344df55ceb2</anchor>
      <arglist>(void *rvalue)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>CTHREAD_get_tid</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga5482f32cae1c40923e57c44dbcc870f7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_get_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gab80527a1cfae365de3473415f80c9510</anchor>
      <arglist>(CTHREAD *thread, void **rvalue)</arglist>
    </member>
    <member kind="variable">
      <type>__thread CTHREAD *</type>
      <name>tls_thread</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>abf34770f7ebd8e4f3560cb5348994fe4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint32_t</type>
      <name>next_tid</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a95bd239696fc78371e5a21b6cf39e931</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cthread.h</name>
    <path>/home/mmoser/archive/cstuff_bak/corothread/</path>
    <filename>a00006</filename>
    <includes id="a00008" name="stacks.h" local="no" imported="no">corothread/stacks.h</includes>
    <class kind="struct">tagCRETURN_POINT</class>
    <class kind="struct">tagCTHREAD</class>
    <member kind="typedef">
      <type>void(*</type>
      <name>CTHREAD_PROC</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>af0ad185700a9a4e97dc60bf65b98cb6c</anchor>
      <arglist>)(void *arg)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagCRETURN_POINT</type>
      <name>CRETURN_POINT</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>acb77203a46ebd97c8ba1febf07fcdca0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagCTHREAD</type>
      <name>CTHREAD</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga531a264d907809a7c19e882c995989c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>CTHREAD_STATE</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga87a29e5321e1ae9ab5a3e043e850e160</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_INIT</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160a480fa30892a6a2e7f742de0dba23bdc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_RUNNING</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160ac83851f1b0105868c14e98d8cb3a4c6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_SUSPENDED</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160abe71ffbb5113b86ca46f76144ab3030d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_EXIT</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160a3fc6ca6d7269ee6cc18ccdbfe5b042dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>CRETURN_POINT *</type>
      <name>CRETURN_POINT_init</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a25102758c4164a5b978062107b2f1c01</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_libinit</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0592112b80e974a450345f78c1aaf42b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>CTHREAD *</type>
      <name>CTHREAD_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga92dd60a37f2db43fd9a55eef3da21bc7</anchor>
      <arglist>(STACKS *stacks, CTHREAD_PROC proc, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga1423b211106739007fc916f1bac9a3a6</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_join</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafb48d6856e2d6ca97593523fb7894382</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_free</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gae4767f31d1375f7edfa267ad62bf8ea3</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start_ex</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gabad990037ec5121d988baabefba4729e</anchor>
      <arglist>(CTHREAD *thread, CRETURN_POINT *next)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_yield</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafa9ace97052a6ddb5764826b4e0a4b59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_resume</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0d7806af6c4043f1c5aaa96c8975ba06</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_set_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6c262644b9961eca998ad344df55ceb2</anchor>
      <arglist>(void *rvalue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_get_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gab80527a1cfae365de3473415f80c9510</anchor>
      <arglist>(CTHREAD *thread, void **rvalue)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>CTHREAD_get_tid</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga5482f32cae1c40923e57c44dbcc870f7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE CTHREAD_STATE</type>
      <name>CTHREAD_state</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gac84f3f78f212fb85214833923d755764</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stacks.c</name>
    <path>/home/mmoser/archive/cstuff_bak/corothread/</path>
    <filename>a00007</filename>
    <includes id="a00008" name="stacks.h" local="yes" imported="no">stacks.h</includes>
    <member kind="function">
      <type>STACK_DIR</type>
      <name>stack_dir</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aca3b0b9dea91da057a9d4f1e8c190a23</anchor>
      <arglist>(int *ptr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>stack_direction</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a4951230d1c901c4533d0a0a3bed8c95e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gacae5bd5f2d666108ed11d96c877999bd</anchor>
      <arglist>(STACKS *stack, int num_stacks, int pages_per_stack)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_destroy</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga416929f1d8b2f705c238c9a22fbe64cc</anchor>
      <arglist>(STACKS *stack)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>STACKS_get</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga363c86e7f58727269a1b09271b521c95</anchor>
      <arglist>(STACKS *stack, STACK_ENTRY **rentry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_release</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga6e3257e3fc51994cabf5bc418ed16648</anchor>
      <arglist>(STACK_ENTRY *entry)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stacks.h</name>
    <path>/home/mmoser/archive/cstuff_bak/corothread/</path>
    <filename>a00008</filename>
    <class kind="struct">tagSTACKS</class>
    <class kind="struct">tagSTACK_ENTRY</class>
    <member kind="typedef">
      <type>struct tagSTACKS</type>
      <name>STACKS</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gab29c025e7084c6eb9b5e8e901df32a0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTACK_ENTRY</type>
      <name>STACK_ENTRY</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga4302023640d9ce2f8e4c70f1b55648f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>STACK_DIR</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a79aa0db3fbf651329d7613f5de3dac9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STACK_DIR_GROWING_DOWN</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a79aa0db3fbf651329d7613f5de3dac9fa82b589a14df95f45bb876d3f44f8969c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STACK_DIR_GROWING_UP</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ga79aa0db3fbf651329d7613f5de3dac9fa621ce0873731906e2c682899a1718dd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gacae5bd5f2d666108ed11d96c877999bd</anchor>
      <arglist>(STACKS *stack, int num_stacks, int pages_per_stack)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_destroy</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga416929f1d8b2f705c238c9a22fbe64cc</anchor>
      <arglist>(STACKS *stack)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>STACKS_get</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga363c86e7f58727269a1b09271b521c95</anchor>
      <arglist>(STACKS *stack, STACK_ENTRY **rentry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_release</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga6e3257e3fc51994cabf5bc418ed16648</anchor>
      <arglist>(STACK_ENTRY *entry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>STACKS_get_stack_size</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga76bb6e2354e42c959cf28d7ad9be8387</anchor>
      <arglist>(STACKS *stack)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CTHREAD</name>
    <title>CTHREAD</title>
    <filename>a00009.html</filename>
    <class kind="struct">tagCTHREAD</class>
    <member kind="typedef">
      <type>struct tagCTHREAD</type>
      <name>CTHREAD</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga531a264d907809a7c19e882c995989c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>CTHREAD_STATE</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga87a29e5321e1ae9ab5a3e043e850e160</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_INIT</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160a480fa30892a6a2e7f742de0dba23bdc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_RUNNING</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160ac83851f1b0105868c14e98d8cb3a4c6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_SUSPENDED</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160abe71ffbb5113b86ca46f76144ab3030d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CTHREAD_STATE_EXIT</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gga87a29e5321e1ae9ab5a3e043e850e160a3fc6ca6d7269ee6cc18ccdbfe5b042dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_libinit</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0592112b80e974a450345f78c1aaf42b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>CTHREAD *</type>
      <name>CTHREAD_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga92dd60a37f2db43fd9a55eef3da21bc7</anchor>
      <arglist>(STACKS *stacks, CTHREAD_PROC proc, void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga1423b211106739007fc916f1bac9a3a6</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_join</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafb48d6856e2d6ca97593523fb7894382</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_free</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gae4767f31d1375f7edfa267ad62bf8ea3</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_start_ex</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gabad990037ec5121d988baabefba4729e</anchor>
      <arglist>(CTHREAD *thread, CRETURN_POINT *next)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_yield</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gafa9ace97052a6ddb5764826b4e0a4b59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_resume</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga0d7806af6c4043f1c5aaa96c8975ba06</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_set_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga6c262644b9961eca998ad344df55ceb2</anchor>
      <arglist>(void *rvalue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CTHREAD_get_return_value</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gab80527a1cfae365de3473415f80c9510</anchor>
      <arglist>(CTHREAD *thread, void **rvalue)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>CTHREAD_get_tid</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ga5482f32cae1c40923e57c44dbcc870f7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE CTHREAD_STATE</type>
      <name>CTHREAD_state</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>gac84f3f78f212fb85214833923d755764</anchor>
      <arglist>(CTHREAD *thread)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STACKS</name>
    <title>STACKS</title>
    <filename>a00010.html</filename>
    <class kind="struct">tagSTACKS</class>
    <class kind="struct">tagSTACK_ENTRY</class>
    <member kind="typedef">
      <type>struct tagSTACKS</type>
      <name>STACKS</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gab29c025e7084c6eb9b5e8e901df32a0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSTACK_ENTRY</type>
      <name>STACK_ENTRY</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga4302023640d9ce2f8e4c70f1b55648f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gacae5bd5f2d666108ed11d96c877999bd</anchor>
      <arglist>(STACKS *stack, int num_stacks, int pages_per_stack)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_destroy</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga416929f1d8b2f705c238c9a22fbe64cc</anchor>
      <arglist>(STACKS *stack)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>STACKS_get</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga363c86e7f58727269a1b09271b521c95</anchor>
      <arglist>(STACKS *stack, STACK_ENTRY **rentry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>STACKS_release</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga6e3257e3fc51994cabf5bc418ed16648</anchor>
      <arglist>(STACK_ENTRY *entry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>STACKS_get_stack_size</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga76bb6e2354e42c959cf28d7ad9be8387</anchor>
      <arglist>(STACKS *stack)</arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>mapping_length</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga8ee90716d7d4f9c1bf7974fd196ca352</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>num_stacks</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga7a15b42db6ba74d038ea49fdf51a4086</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>one_stack_size</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gab7c15d9df43b0f687f3c9c712fae0e82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLIST</type>
      <name>root</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga08030f5ed264a29c070d11287938979d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>stack_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga9c067ef2a750355a9ce3360ac2ead65b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>STACKS *</type>
      <name>stacks</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga3bf0755824c9a30451e247848157685b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagCRETURN_POINT</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>ucontext_t</type>
      <name>context</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a3d8f335720dd0ec325fff4b5544b65b9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagCTHREAD</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>ucontext_t</type>
      <name>context_coroutine</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a74b86a0255cf15d3cfcb21389f590fde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ucontext_t</type>
      <name>context_caller</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>aee14cdad40d62281ab3ccfaf25e76cd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CTHREAD_STATE</type>
      <name>state</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ae5e75d1abd9dbf7dee8d91e9864f8456</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>thread_id</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a3f7ef430812f309728071e027ec90532</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>ctx</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>abb4fa0f5cd863eb21ba15dd01b40881f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>rvalue</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a78287f59766e6a1e73868c22793d2ab8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CTHREAD_PROC</type>
      <name>proc</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a5ff356da73817c9a3d239dbde9e80c85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagCTHREAD *</type>
      <name>prev_thread</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ae6fb70fa6314738018642f89ca5c359c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>STACK_ENTRY *</type>
      <name>stack_entry</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ae435552c206d330befbad33b9fdfb3a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSTACK_ENTRY</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>DLIST_entry</type>
      <name>entry</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a9c3527c5b313fb4fd382f47f1fc4f8b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>stack_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga9c067ef2a750355a9ce3360ac2ead65b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>STACKS *</type>
      <name>stacks</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga3bf0755824c9a30451e247848157685b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSTACKS</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>mapping</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a3e089321a0d191b9291649c828fe9c48</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>mapping_length</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga8ee90716d7d4f9c1bf7974fd196ca352</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>num_stacks</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga7a15b42db6ba74d038ea49fdf51a4086</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>one_stack_size</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>gab7c15d9df43b0f687f3c9c712fae0e82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLIST</type>
      <name>root</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ga08030f5ed264a29c070d11287938979d</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
