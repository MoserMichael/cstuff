<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>cbarrier.c</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00006</filename>
    <includes id="a00007" name="cbarrier.h" local="yes" imported="no">cbarrier.h</includes>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>CYCLIC_BARRIER_wait_for_all_to_finish</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ac3e53070b568d3782fc1d669491808c5</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CYCLIC_BARRIER_init</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a0fc4eba3b18970f3e4e23d3db8c4f99d</anchor>
      <arglist>(CYCLIC_BARRIER *cond, int num)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_reset</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>ab970fd60f07e77be074031e5c52a45fc</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_free</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a28aee366454e6f7563c963a2ef96b0ad</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_await</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a60330e74834b5137fd27b8e3ea68351b</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cbarrier.h</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00007</filename>
    <class kind="struct">CYCLIC_BARRIER</class>
    <member kind="function">
      <type>void</type>
      <name>CYCLIC_BARRIER_init</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a0fc4eba3b18970f3e4e23d3db8c4f99d</anchor>
      <arglist>(CYCLIC_BARRIER *cond, int num)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_free</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a28aee366454e6f7563c963a2ef96b0ad</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_await</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a60330e74834b5137fd27b8e3ea68351b</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>CYCLIC_BARRIER_reset</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ab970fd60f07e77be074031e5c52a45fc</anchor>
      <arglist>(CYCLIC_BARRIER *cond)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tpool.c</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00008</filename>
    <includes id="a00009" name="tpool.h" local="yes" imported="no">tpool.h</includes>
    <member kind="function">
      <type>void</type>
      <name>RUNNABLE_init</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a26254071747a14c26402a397d3766f9f</anchor>
      <arglist>(RUNNABLE *runnable, RUNNABLE_HANDLER handler)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>worker_thread</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>adea6ef467308139cf432ac8bab181b3b</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="function">
      <type>THREADPOOL *</type>
      <name>THREADPOOL_init</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a7b909aed828b50e11051b9147aebb82f</anchor>
      <arglist>(RUNNABLE_HANDLER process_result, int queue_size, int num_threads, int stack_size_kb)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>THREADPOOL_close</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ad367dbd16ee8e8924423fdd0a8e737f3</anchor>
      <arglist>(THREADPOOL *pool)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>THREADPOOL_send_block_on_queue_full</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a47bed275260b9a13f7ccc2b1fbe8c76a</anchor>
      <arglist>(THREADPOOL *pool, RUNNABLE *request)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>THREADPOOL_send_fail_on_queue_full</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ac119ddd51197dc5e0d8e709f3994a5ae</anchor>
      <arglist>(THREADPOOL *pool, RUNNABLE *request)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tpool.h</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00009</filename>
    <includes id="a00011" name="tqueue.h" local="no" imported="no">tutils/tqueue.h</includes>
    <includes id="a00007" name="cbarrier.h" local="no" imported="no">tutils/cbarrier.h</includes>
    <class kind="struct">tagRUNNABLE</class>
    <class kind="struct">tagTHREADPOOL</class>
    <member kind="typedef">
      <type>void(*</type>
      <name>RUNNABLE_HANDLER</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>af16d7d96e7ed5b618a81394d68de79e4</anchor>
      <arglist>)(struct tagRUNNABLE *request)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagRUNNABLE</type>
      <name>RUNNABLE</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a3f3ff8a5ef0014f681395dda3e8e5ca6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagTHREADPOOL</type>
      <name>THREADPOOL</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a23469035543918e40160a7c520fa4cac</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RUNNABLE_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a26254071747a14c26402a397d3766f9f</anchor>
      <arglist>(RUNNABLE *runnable, RUNNABLE_HANDLER handler)</arglist>
    </member>
    <member kind="function">
      <type>THREADPOOL *</type>
      <name>THREADPOOL_init</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a7b909aed828b50e11051b9147aebb82f</anchor>
      <arglist>(RUNNABLE_HANDLER process_result, int queue_size, int num_threads, int stack_size_kb)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>THREADPOOL_close</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ad367dbd16ee8e8924423fdd0a8e737f3</anchor>
      <arglist>(THREADPOOL *pool)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>THREADPOOL_send_block_on_queue_full</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>a47bed275260b9a13f7ccc2b1fbe8c76a</anchor>
      <arglist>(THREADPOOL *pool, RUNNABLE *request)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>THREADPOOL_send_fail_on_queue_full</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ac119ddd51197dc5e0d8e709f3994a5ae</anchor>
      <arglist>(THREADPOOL *pool, RUNNABLE *request)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tqueue.c</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00010</filename>
    <includes id="a00011" name="tqueue.h" local="yes" imported="no">tqueue.h</includes>
    <class kind="struct">tagTQUEUE_Entry</class>
    <member kind="typedef">
      <type>struct tagTQUEUE_Entry</type>
      <name>TQUEUE_Entry</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>affa06b27942b5d43f6590bb23f799b14</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_init</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a4ae0a31bb2c0162bd58249e5a57a5af9</anchor>
      <arglist>(TQUEUE *queue, size_t max_count)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_free</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ab21f95b31bed20649a5ffa6a3902ce7d</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>push_entry</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>aa74149cbaa139844b365ade5d723f17d</anchor>
      <arglist>(DLIST *lst, void *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_exit_message</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ae792121619b59c4ee95afbc8f3ccfc3f</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_block_on_queue_full</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ab61172de6cf3a4f11122696a48665c1b</anchor>
      <arglist>(TQUEUE *queue, void *entry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_fail_on_queue_full</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a371657b010acd669b760f72307d7ab99</anchor>
      <arglist>(TQUEUE *queue, void *entry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_pop_non_blocking</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a6c1810568ce82ed03f5a364ac6e5b151</anchor>
      <arglist>(TQUEUE *queue, void **rret)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>TQUEUE_pop</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a5e62b4b0ebe2d1460cf45f984d0ceed4</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tqueue.h</name>
    <path>/home/mmoser/archive/cstuff_bak/tutils/</path>
    <filename>a00011</filename>
    <class kind="struct">tagTQUEUE</class>
    <member kind="typedef">
      <type>struct tagTQUEUE</type>
      <name>TQUEUE</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a49b01bcaf1b915cafb33b0d8a0c0b267</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_init</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a4ae0a31bb2c0162bd58249e5a57a5af9</anchor>
      <arglist>(TQUEUE *queue, size_t max_count)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_free</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ab21f95b31bed20649a5ffa6a3902ce7d</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_block_on_queue_full</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ab61172de6cf3a4f11122696a48665c1b</anchor>
      <arglist>(TQUEUE *queue, void *entry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_fail_on_queue_full</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a371657b010acd669b760f72307d7ab99</anchor>
      <arglist>(TQUEUE *queue, void *entry)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_push_exit_message</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>ae792121619b59c4ee95afbc8f3ccfc3f</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>TQUEUE_pop</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a5e62b4b0ebe2d1460cf45f984d0ceed4</anchor>
      <arglist>(TQUEUE *queue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>TQUEUE_pop_non_blocking</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a6c1810568ce82ed03f5a364ac6e5b151</anchor>
      <arglist>(TQUEUE *queue, void **rret)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CYCLIC_BARRIER</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>pthread_mutex_t</type>
      <name>mutex</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>ae7d60bb4f5f20f2e16e03416357283c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pthread_cond_t</type>
      <name>cond</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>af244f45e851fa7682ff656a70e28060b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>initial_count</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a062d6ad274f1e91ce8cedc514edb6b9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>tcount</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>ac9d77335499bee9ef32f865aca90b909</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>is_finished</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a775b38f6130cfc743e2032d387ab38d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>left</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a70c52685d88e3568279eaab494145035</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagRUNNABLE</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>RUNNABLE_HANDLER</type>
      <name>handle_request</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a127e2ce895ac78923ce28190c1c793f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTHREADPOOL</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>num_threads</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a0bbc9bfb92f691411651c63f2de40465</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TQUEUE</type>
      <name>request_queue</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a4b67f6780a55d06dda8ad7f893b0b95b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CYCLIC_BARRIER</type>
      <name>all_finished</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a57ebff173542131630ab9542df496a2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RUNNABLE_HANDLER</type>
      <name>process_result</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a135b256b03a34d96456263b584bbddcf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTQUEUE</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>DLIST</type>
      <name>dlist</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a79e4b100329cacd8b50b0775a94bad2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>max_count</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a2c2ab1be9494f82bdd88062fa54d7b5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>waiting_empty</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a54bfe8c12f38b408a6bbf6fa6a8ce32f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pthread_mutex_t</type>
      <name>mutex</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a5d4dd66ddcc9fc77ef2eff43b84740eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pthread_cond_t</type>
      <name>cond_empty</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a8d1f63a8509a6561bd42e36e983591a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pthread_cond_t</type>
      <name>cond_max</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>aee83458802825efb914c1a60c75290e3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTQUEUE_Entry</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>DLIST_entry</type>
      <name>dlist</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>af2aa6c28f5f39d56d69213d6014346b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>entry</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a109c2451a737f4763976b7bff39cc899</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
