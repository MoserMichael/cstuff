<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>array.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00012</filename>
    <includes id="a00013" name="array.h" local="no" imported="no">cutils/array.h</includes>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>ARRAY_grow_default</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>a1bd82a22c9afadac7a88445e92161214</anchor>
      <arglist>(ARRAY *arr, size_t request_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_insert_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga178c95e99fb2c218963f440c295c8380</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_set_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa527efc4a7222882143f6eaf88474c2e</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>array.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00013</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">ARRAY</class>
    <member kind="define">
      <type>#define</type>
      <name>ARRAY_init_stack</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga563145759e08f5ec765057251f720ebf</anchor>
      <arglist>(arr, elmsize, numofelem)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>ARRAY_VISITOR</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga4787a36c1aa147cb273c6ea71d28f47b</anchor>
      <arglist>)(int index, void *elm, size_t elmsize, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_init</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga694aa4139823d620f5bb031b49aa4f0f</anchor>
      <arglist>(ARRAY *arr, size_t elmsize, size_t numofelem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_init_fixed</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gafd44eaffad9a4b06e18dfdfb7e6c9652</anchor>
      <arglist>(ARRAY *arr, size_t elmsize, void *ptr, size_t bufsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_free</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gafed6207d4e648f9653cb2bb5a5ceec43</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_reset</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga0a21f05148b610f008ab77af9146971e</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>ARRAY_size</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaac3845deec6b60a387315965cf64d267</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>ARRAY_maxsize</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga39032c979a9bb46994242a38d0832339</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_resize</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa1332b32680cff38fd71bcc58930bb86</anchor>
      <arglist>(ARRAY *arr, size_t num_elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint8_t *</type>
      <name>ARRAY_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga741b102f833e0ee35d7413d12c57f136</anchor>
      <arglist>(ARRAY *arr, size_t index)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_copy_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaffc77bae46165067e9b7894e7007d9c7</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_insert_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga178c95e99fb2c218963f440c295c8380</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_set_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa527efc4a7222882143f6eaf88474c2e</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_delete_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga476adf0145ce1808b2427ff0bf4cd6c0</anchor>
      <arglist>(ARRAY *arr, size_t index)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_push_back</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga557c0fb20ce197428a69374b47e387e0</anchor>
      <arglist>(ARRAY *arr, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_pop_back</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga5c8e00dc4d38ebdb2da305e91887ffdd</anchor>
      <arglist>(ARRAY *arr, void *ptr, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_stack_top</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gae28547acd38f72d116742b8009ed9e07</anchor>
      <arglist>(ARRAY *arr, void *ptr, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_foreach</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga107782865cee682eba1fc89e764f5e03</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_foreach_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gab0f58e396b81f03d0d96ddbd55672045</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_foreach_reverse</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga348bcb863616489982859044a4907c05</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_foreach_reverse_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga3bb873bda9ffaa686071a4ec35307636</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga8862f23900d93ed5753bde771f556414</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga903c986f29a84e8ea1b01583f03dac0b</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_reverse</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga41162038e45941474c4b9f7e5254e6d9</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_reverse_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa4564f7eefb2ff3f1ca290c216629632</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx, uint32_t *retval)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>base.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00014</filename>
    <member kind="define">
      <type>#define</type>
      <name>_MEMBEROF</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a7b468c9bd3d1f3de0bb47e2ea23ddb1c</anchor>
      <arglist>(ptr, offset)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_INLINE</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>ad8685d2efad5bbbf60ebe6bc79e05350</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_UNUSED</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>a8a28f4ccca88db778f9dad279d3ba818</anchor>
      <arglist>(x)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>bhash.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00015</filename>
    <includes id="a00016" name="bhash.h" local="no" imported="no">cutils/bhash.h</includes>
    <includes id="a00032" name="util.h" local="no" imported="no">cutils/util.h</includes>
    <includes id="a00022" name="hashfunction.h" local="no" imported="no">cutils/hashfunction.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ADJUST_HASH</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>acb4c58835714882f70736b2c45188ff4</anchor>
      <arglist>(hash, buckets)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_init</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga73ce0ca18399c5db0bac0491aeadba8d</anchor>
      <arglist>(HASH *hash, size_t buckets, int ismultimap, HASH_COMPARE_KEY compare_key, HASH_FUNCTION hash_func)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_resize</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gacef4d862b25db1563ce621265299fb2e</anchor>
      <arglist>(HASH *hash, size_t buckets)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find_in_bucket</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>aa0f01a8d9a72c6a69d0891fd81076bee</anchor>
      <arglist>(SRING *abucket, HASH_VALUE hash, HASH_COMPARE_KEY compare_key, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga072fef06fb9a516f47771ddd3946bf20</anchor>
      <arglist>(HASH *phash, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find_next</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga2f20017ef2a49dc235a8bea78b254dd6</anchor>
      <arglist>(HASH *phash, HASH_Entry *prev, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_insert</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gaf9ce37d092fe7b3ae217336ef623a7dc</anchor>
      <arglist>(HASH *phash, HASH_Entry *entry, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_unlink</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga9546382a27adc8b91d70cd6e4c4723c3</anchor>
      <arglist>(HASH *phash, void *key, ssize_t key_size)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>bhash.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00016</filename>
    <includes id="a00028" name="sring.h" local="no" imported="no">cutils/sring.h</includes>
    <includes id="a00022" name="hashfunction.h" local="no" imported="no">cutils/hashfunction.h</includes>
    <class kind="struct">HASH_Entry</class>
    <class kind="struct">HASH</class>
    <member kind="define">
      <type>#define</type>
      <name>HASH_BUCKET_FOREACH</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gad6e4f490068b19fa3fea075d3b0180f7</anchor>
      <arglist>(cur, bucket)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH_KEY</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga7a866beabda7e0bd95e19ae7c708c8c0</anchor>
      <arglist>(cur, hash, key, key_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_KEY</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gad5d12368e1648a383d048e7fed56fa82</anchor>
      <arglist>(cur, hash, key, key_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_KEY_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga6bdd6abcda051166ee1ce8586b80cd8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga635ee4184a9ca1e86138aec673a0a9b1</anchor>
      <arglist>(cur, hash)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga1dd13a6578ab349c2cbac48304a0f5e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga1193ff43ed7c2f743c974d75c39efc93</anchor>
      <arglist>(cur, hash)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gaee7185cd320df1c4a801762c8f5201b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>HASH_VALUE(*</type>
      <name>HASH_FUNCTION</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga11718c21601b9ee597d31cb7e720c260</anchor>
      <arglist>)(void *data, ssize_t length)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HASH_COMPARE_KEY</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gaf9eab147ddd898a2d7fb376e1f8753a3</anchor>
      <arglist>)(HASH_Entry *, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HASH_VISITOR</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga1bcff3f62d46d79b790bed89891cd9a5</anchor>
      <arglist>)(HASH_Entry *, void *context)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_init</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga73ce0ca18399c5db0bac0491aeadba8d</anchor>
      <arglist>(HASH *hash, size_t buckets, int ismultimap, HASH_COMPARE_KEY compare_key, HASH_FUNCTION hash_func)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_resize</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gacef4d862b25db1563ce621265299fb2e</anchor>
      <arglist>(HASH *hash, size_t buckets)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_free</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga889a4afc9437f453ed7db27d7bd39d3d</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HASH_size</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga8924e383f82b78dcb693ffcab49188b0</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga072fef06fb9a516f47771ddd3946bf20</anchor>
      <arglist>(HASH *phash, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find_next</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga2f20017ef2a49dc235a8bea78b254dd6</anchor>
      <arglist>(HASH *phash, HASH_Entry *prev, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_insert</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gaf9ce37d092fe7b3ae217336ef623a7dc</anchor>
      <arglist>(HASH *phash, HASH_Entry *entry, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_unlink</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga9546382a27adc8b91d70cd6e4c4723c3</anchor>
      <arglist>(HASH *hash, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_foreach_key</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga96c141f22c63392dfe758de963ffe8f5</anchor>
      <arglist>(HASH *hash, void *key, ssize_t key_size, HASH_VISITOR eval_func, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_foreach</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga251e1a1fd0e312bae674631001fd4b55</anchor>
      <arglist>(HASH *hash, HASH_VISITOR eval_func, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE HASH_Entry *</type>
      <name>HASH_findif</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga58acba9b6611fe07a44253eeb4307e09</anchor>
      <arglist>(HASH *hash, HASH_VISITOR eval_func, void *context, int *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_deleteall</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga0ed6fe026b7e8aef4611325ab2ffdc47</anchor>
      <arglist>(HASH *hash, int offset_of_link, HASH_VISITOR on_delete, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HASH_check</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga3482872e1f844042afc76de5136bff3f</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>dlist.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00017</filename>
    <includes id="a00018" name="dlist.h" local="no" imported="no">cutils/dlist.h</includes>
  </compound>
  <compound kind="file">
    <name>dlist.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00018</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">tagDLIST_entry</class>
    <class kind="struct">DLIST</class>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga563984e90677ff397213299a05ca8d04</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_REVERSE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gab21653656a5607e487327ed381a42ffc</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_SAVE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaa8491f4f93ae66bcd0006914484ec013</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_REVERSE_SAVE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga46f8bbaa697f05c1f4e444ffd04f72d1</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagDLIST_entry</type>
      <name>DLIST_entry</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga8bcfc120d0cdb50aa5e418d3c7dd1bdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DLIST_VISITOR_V</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac53c87d52fbf30ec94612007d2838089</anchor>
      <arglist>)(DLIST *list, DLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DLIST_VISITOR</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga54b6468514a681ad99c543b7e1f94a32</anchor>
      <arglist>)(DLIST *list, DLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DLIST_COMPARE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gad66eadd6b35614c8b18f1687db7ce300</anchor>
      <arglist>)(DLIST_entry *, DLIST_entry *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_init</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4b32655ea9e332581a0bab18857f8adb</anchor>
      <arglist>(DLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_isempty</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gacbf81693b507a588b1abcd3377471558</anchor>
      <arglist>(DLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_insert_before</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga572203aad7a7d974bcffc50baef3105d</anchor>
      <arglist>(DLIST *list, DLIST_entry *pos, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_insert_after</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac7af205a388bf32b16cb8b7c8ecc32f4</anchor>
      <arglist>(DLIST *list, DLIST_entry *pos, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_unlink</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga9bbc2c5bac97e9ed491c8fd6f6eae1ce</anchor>
      <arglist>(DLIST *list, DLIST_entry *link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_push_back</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gafbf7e8463b8a2615c35175e1b804b8df</anchor>
      <arglist>(DLIST *list, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_push_front</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4da016f9927a26c8d90fe81416684bc7</anchor>
      <arglist>(DLIST *list, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_pop_front</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga9145408d5a80137e5660f32c21029d9b</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_pop_back</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga32a3f6d0de35b46dd011702f5873bb72</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_first</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaefa144f015d6a1283afcb2716b0dc747</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_last</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gae039944967f35a70066609fc5d016942</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_next</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga6af1da3a12a2ca6b60695bbb083d3198</anchor>
      <arglist>(DLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_prev</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac17b78519ee5c430e2eb4d471e2713ee</anchor>
      <arglist>(DLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DLIST_size</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga1884ac42bd98ebbf0b7f71bdd67579b6</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_nth</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaff66f931cabc3fbcf6e420135a135877</anchor>
      <arglist>(DLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_nth_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4c8a5673bcc07dd3f5f4f7736f7a798d</anchor>
      <arglist>(DLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_foreach</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaf6188d6ec22905f7ea11335c30b1627f</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_foreach_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga120a5162547a971d45a2d6446a9c4273</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_findif</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaec008957a6a9b8aca1f29e691f8d929c</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_findif_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga37989ca126a98eeab2a6a4310b1edf70</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_deleteif</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gab24282d74d6ac2ff24b28e213d3cea15</anchor>
      <arglist>(DLIST *list, DLIST_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_deleteall</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaa0e8e5379952c2c61e373f576c047792</anchor>
      <arglist>(DLIST *list, DLIST_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_insert_sorted</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga82c92d75b00d2d927eeb44c9413cc1f6</anchor>
      <arglist>(DLIST *list, DLIST_COMPARE compare, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga078d4f221e97947eeebb89877973a9cf</anchor>
      <arglist>(DLIST *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_check</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga3152f24ad9c9f817968e6eb04d7f8474</anchor>
      <arglist>(DLIST *header)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>dring.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00019</filename>
    <includes id="a00020" name="dring.h" local="no" imported="no">cutils/dring.h</includes>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gacc803a0c5a8bc3ce165f8d8fe5e78ab1</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *data, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga98724ac564245aa77a1a4c55f83fadb9</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *data, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_insert_sorted</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gad8c4485c62148db72ca05f55dcc75709</anchor>
      <arglist>(DRING *list, DRING_COMPARE compare, DRING *newentry)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>dring.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00020</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">tagDRING</class>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac8118521928cd5a00b83ee9c0ba48308</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_REVERSE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga550a7b5adbe2d3325db38c49764bdc8c</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_SAVE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaab96829db58e4eaa3cec725c9c99ff69</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_REVERSE_SAVE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac5d159651c812e6785f81bc2dfc73b7d</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagDRING</type>
      <name>DRING</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga4630800b5e50c32e36110b0a6dfcadc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DRING_VISITOR_V</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga516498a8d915f013d2466856c72a0bd6</anchor>
      <arglist>)(DRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DRING_VISITOR</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gadbbb7e55996e9a7bc35f1f5bd26b75d0</anchor>
      <arglist>)(DRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DRING_COMPARE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga275575e4b5e7edcaaf471ec1e5f02b5a</anchor>
      <arglist>)(DRING *, DRING *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_init</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaea5e890f7d1dab572fe3ef6f067e5d26</anchor>
      <arglist>(DRING *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DRING_isempty</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga7b28ffdc594d835f2c49cf9e74f7c3b3</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_insert_before</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga93f762e668a7283b18025310886635ea</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_insert_after</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gab65d72bd525d11559228b5aa3aa641f0</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_insert_sorted</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gad8c4485c62148db72ca05f55dcc75709</anchor>
      <arglist>(DRING *list, DRING_COMPARE compare, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_unlink</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga81d2db3ee3b83fe28393d45530bed230</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_push_back</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaed724487f6cd26c0b79b875ff3390c61</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_push_front</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga897228c60f1de15be0799dadea9d0337</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_pop_front</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga315bb0f49e0ccbadcc59a5e9258eff19</anchor>
      <arglist>(DRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_pop_back</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga50912d37cc30e961dd53ff291b8db05e</anchor>
      <arglist>(DRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_first</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaa392ee3a23da21ccf4e4ea62bd6c7de8</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_last</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac4fbe3b2aa6b11ce356755c89235b9d4</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_next</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga44a51594b007ec843d30bf5c3b3ddc73</anchor>
      <arglist>(DRING *end, DRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_prev</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gae8038a9a107a06bb0fae04cd8f8f7711</anchor>
      <arglist>(DRING *end, DRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DRING_size</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gafb5caaea75f7196bd6abe1ac8e81a7f3</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_nth</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga7f552929105d5cf0a6f621c6e8463f2d</anchor>
      <arglist>(DRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_nth_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga82d60185895dcf1deecd14d4a809741f</anchor>
      <arglist>(DRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gacc803a0c5a8bc3ce165f8d8fe5e78ab1</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga98724ac564245aa77a1a4c55f83fadb9</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_findif</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga3efd1408b3d08b2253e1bf781306a425</anchor>
      <arglist>(DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_find_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga8d223c41dde7af2a4a154e09bf9c3a03</anchor>
      <arglist>(DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DRING_deleteif</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga44b92d9180c58865b9d4a1c6d5d9c37f</anchor>
      <arglist>(DRING *list, DRING_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_deleteall</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gab49cbee747109aad10c6e6a67c1149d9</anchor>
      <arglist>(DRING *list, DRING_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga02284390005b102707b116d7de2b7ae6</anchor>
      <arglist>(DRING *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DRING_check</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaead03ab9284bb52e1a5204010109b69d</anchor>
      <arglist>(DRING *header)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hashfunction.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00021</filename>
    <includes id="a00022" name="hashfunction.h" local="yes" imported="no">hashfunction.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>hashsize</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a86173ca25b300cf3276b088d3fc6bdaf</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>hashmask</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>aacd6ed104463685849b2550389d05ff3</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>mix</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>afd9fa1747724ec6142812721997394c3</anchor>
      <arglist>(a, b, c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BJ_INIT_VAL</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a99562323a2cf4494995a145c766d3241</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_sample_hash_func</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a8b8ce0c98596d3de6f7d82f8298b93b4</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_PJW</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>acf656e3b07391cb24287f1c04b45c7fd</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_rotating</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a434709d2c4c30391c1b56111cb6cca21</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_shift_and_xor</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a96ac8abe8c87ddc684080fe775fe2144</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Fowler_Noll_Vo</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a2f9d42247ebd6fdadfb4b677f9d8c93e</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Bob_Jenkins_one_at_a_time</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a1498645507ca3a2df89dabf640fde50d</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_ELF</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a162cb7511f51645fa3cc7f9e06ac3b6e</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Bob_Jenkins</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a95f3a27dbdf1499a650ba11211284869</anchor>
      <arglist>(void *keydata, ssize_t length)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hashfunction.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00022</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>HASH_STRING</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a610bced429f9d429f9b54d69135e9253</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>HASH_VALUE</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a0add298fa768754c300d2489164b8677</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_PJW</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>acf656e3b07391cb24287f1c04b45c7fd</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_rotating</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a434709d2c4c30391c1b56111cb6cca21</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_shift_and_xor</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a96ac8abe8c87ddc684080fe775fe2144</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Fowler_Noll_Vo</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a2f9d42247ebd6fdadfb4b677f9d8c93e</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Bob_Jenkins_one_at_a_time</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a1498645507ca3a2df89dabf640fde50d</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_ELF</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a162cb7511f51645fa3cc7f9e06ac3b6e</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_Bob_Jenkins</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a95f3a27dbdf1499a650ba11211284869</anchor>
      <arglist>(void *keydata, ssize_t length)</arglist>
    </member>
    <member kind="function">
      <type>HASH_VALUE</type>
      <name>HASHFUNCTION_sample_hash_func</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>a8b8ce0c98596d3de6f7d82f8298b93b4</anchor>
      <arglist>(void *keydata, ssize_t key_length)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>heap.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00023</filename>
    <includes id="a00024" name="heap.h" local="no" imported="no">heap.h</includes>
    <member kind="function">
      <type>int</type>
      <name>HEAP_push</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gaf33187b5e3ea29b6da1ccf4b2490bc7f</anchor>
      <arglist>(HEAP *heap, void *element, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_pop</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gadb9cc342dba8eb29fe8be37e56a16370</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>check_recursive</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a68f48f61e4ae987ccd74d127d7cf7c76</anchor>
      <arglist>(HEAP *heap, size_t pos)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_check</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga689c175c301ff3025290d37ca5800039</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>visit_sorted</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a0f51edc6a8cf741b8a292e63f59cbd10</anchor>
      <arglist>(HEAP *heap, size_t pos, HEAP_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HEAP_foreach_sorted</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga72dd058aadb08417c5fcb618e9be6833</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>heap.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00024</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">HEAP</class>
    <member kind="typedef">
      <type>int(*</type>
      <name>HEAP_compare</name>
      <anchorfile>a00024.html</anchorfile>
      <anchor>aa0102894a26d562e126b76e7df87a143</anchor>
      <arglist>)(void *lhs, void *rhs, size_t elmsize)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HEAP_VISITOR</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga13498c6ee613dce46c5f505dc66fa6dc</anchor>
      <arglist>)(int index, void *elm, size_t elmsize, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HEAP_init</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac152cfd9171fdc1cee9f7d08ba85f6c4</anchor>
      <arglist>(HEAP *heap, size_t elmcount, size_t elmsize, HEAP_compare compare_func)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HEAP_free</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga74a671a1104e9a2d48a430427a2f729b</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_size</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4c1edfd6bdc79001bb92fcd20efc0058</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_maxsize</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4f0ddfd22ffd30752a88c25c27508ff1</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_elmsize</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga2a7f7199e6201e88740ee037daef1027</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>HEAP_top</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga3cdfaf0ef3e2728601b89c445b938b31</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>HEAP_get_at</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac1bbf31bcce42f7b63433f0ba732a1bf</anchor>
      <arglist>(HEAP *heap, int index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_pop</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gadb9cc342dba8eb29fe8be37e56a16370</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_push</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gaf33187b5e3ea29b6da1ccf4b2490bc7f</anchor>
      <arglist>(HEAP *heap, void *element, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_check</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga689c175c301ff3025290d37ca5800039</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HEAP_foreach_sorted</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga72dd058aadb08417c5fcb618e9be6833</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HEAP_foreach</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4bc6463fb11b07c4fa67125675342066</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HEAP_findif</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac2fb2dbcb7d6f5b2d394046334a696f4</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>slist.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00025</filename>
    <includes id="a00026" name="slist.h" local="no" imported="no">cutils/slist.h</includes>
  </compound>
  <compound kind="file">
    <name>slist.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00026</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">tagSLIST_entry</class>
    <class kind="struct">SLIST</class>
    <member kind="define">
      <type>#define</type>
      <name>SLIST_FOREACH</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga13e3414add243765e1acc420f8a0f478</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SLIST_FOREACH_SAVE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1fd8f8e8cee04217d15bc9e40dc22874</anchor>
      <arglist>(loopvarname, loopvarnamenext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSLIST_entry</type>
      <name>SLIST_entry</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>gaacb00d1a4daaa75eee59c1483cb57b80</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>SLIST_VISITOR_V</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gac7e7623e772b63394d7aa00c570f8e84</anchor>
      <arglist>)(SLIST *list, SLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SLIST_VISITOR</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga7b248c4eed2d3b3cf32fc0329063de57</anchor>
      <arglist>)(SLIST *list, SLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SLIST_COMPARE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga986253a6f00466c9a8fc577922946578</anchor>
      <arglist>)(SLIST_entry *, SLIST_entry *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_init</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gad4951a8a17c4c71dbccf87f25e1d0cc9</anchor>
      <arglist>(SLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SLIST_isempty</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga167ddd6e77f962055cbf23edfae47a55</anchor>
      <arglist>(SLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_insert_after</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga28b526884512439a095d0a1f8defbf7a</anchor>
      <arglist>(SLIST *list, SLIST_entry *pos, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_unlink_after</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0242e7614311873394c09036c3f8db31</anchor>
      <arglist>(SLIST *list, SLIST_entry *link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_push_back</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga162430e86a10d44d37f06f3286336e39</anchor>
      <arglist>(SLIST *list, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_push_front</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9640eb240d27802e9762df5eb890751f</anchor>
      <arglist>(SLIST *list, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_append</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga496c6461f8655694a2163dbf2cc2ce3c</anchor>
      <arglist>(SLIST *dest, SLIST *src)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_pop_front</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1f0ecce5cb624eee52d5d1385b355296</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_first</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga2e184c75f69621e1ca7d77d762cf3ac4</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_last</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gab1da0b7495c594466c35f43b62ba2fa6</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_next</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0377a487001bec48ddfccf6e3f72653a</anchor>
      <arglist>(SLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SLIST_size</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gae1806eb6dd23d1489e2a3a87c16a7c89</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_nth</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga2b16d0912f1762278bd39a3166f2937a</anchor>
      <arglist>(SLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_nth_reverse</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga4ed5d1aa0c00b8e6e7208a136cf9836f</anchor>
      <arglist>(SLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_foreach</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9a2bcb303e10ce22f43138d974cf6f61</anchor>
      <arglist>(SLIST *lst, SLIST_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_findif</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gab7659684a1854619f6514007a4a2a7ac</anchor>
      <arglist>(SLIST *lst, SLIST_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_insert_sorted</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9d6b9df04f353039015f1f96349d6daf</anchor>
      <arglist>(SLIST *list, SLIST_COMPARE compare, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_deleteif</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gaa33b9f8d6cfd2573bd33e49ef0dd9b62</anchor>
      <arglist>(SLIST *list, SLIST_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_deleteall</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1c75f1166c0c6048c115f0de18c2dde2</anchor>
      <arglist>(SLIST *list, SLIST_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_reverse</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0979079d34e30779ee70a356bc46371f</anchor>
      <arglist>(SLIST *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SLIST_check</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga69ab9537af27dcf8890148ccdc8d5a27</anchor>
      <arglist>(SLIST *header)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sring.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00027</filename>
    <includes id="a00028" name="sring.h" local="no" imported="no">cutils/sring.h</includes>
  </compound>
  <compound kind="file">
    <name>sring.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00028</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">tagSRING</class>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga39582582e3bfccf18e39710e75009ad1</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH_SAVE</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga7f6178ad165e9b2f699efd88a3cbca8a</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH_SAVE_END</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga0e4892ffac6b949a95b1d9b1895433b7</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSRING</type>
      <name>SRING</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga478bf88a63498eec67b9720586d6bba9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>SRING_VISITOR_V</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga8fd5f1f287aecfe1a45ad63bd0c38fca</anchor>
      <arglist>)(SRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SRING_VISITOR</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gab481e0fe056d133b74281971cb70a90f</anchor>
      <arglist>)(SRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SRING_COMPARE</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaa197bf51f9b33ec012296a14be9c4838</anchor>
      <arglist>)(SRING *, SRING *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_init</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga53ac9788c8cdc9e03a9e343fd7e8b15a</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SRING_isempty</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga23aab5b8a7efeacf278b444bb785bc1d</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_insert_after</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga6a2a1950260f3fb3d385bde480e5c976</anchor>
      <arglist>(SRING *list, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_unlink_after</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gab573cc304039b055762f9251cc551d36</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_push_front</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga608f3b7787a81834f15eddbc4135d04f</anchor>
      <arglist>(SRING *list, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_pop_front</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga1702358f1edcb1d85df818fdb8a20a23</anchor>
      <arglist>(SRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_first</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaa43e9213ae7d390a22ffd34164a9a8fb</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_last</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga9bfd87c38a546d0bd880e2ef6afb4308</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_next</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga02277d246cf87fa74cf2face4271dc46</anchor>
      <arglist>(SRING *end, SRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_size</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gad00dfbc796c0f39381f8cb207d45a628</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_nth</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga0bf1663e78699f16d176b4e5eab35760</anchor>
      <arglist>(SRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_nth_reverse</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga4eacc9ad78693d75fc82960853bfc77d</anchor>
      <arglist>(SRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_foreach</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga948f45c8af2b99aec05ca538b4a07e9e</anchor>
      <arglist>(SRING *lst, SRING_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_findif</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga742710ac325213e24ddf5d02ef30bc9c</anchor>
      <arglist>(SRING *lst, SRING_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_deleteif</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga182b8c409d5c05637085210b31950431</anchor>
      <arglist>(SRING *list, SRING_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_deleteall</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga96914d218afb3f3dac01bf98702e3ef7</anchor>
      <arglist>(SRING *list, SRING_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_insert_sorted</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga6a5cdf3eb278daa953e2f45dc4cb4518</anchor>
      <arglist>(SRING *list, SRING_COMPARE compare, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_reverse</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaff2e874e90de5dbc9c321146a302cba9</anchor>
      <arglist>(SRING *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_check</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga7d49e006bf25f2d83d15b70caf9f3254</anchor>
      <arglist>(SRING *head)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tree.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00029</filename>
    <includes id="a00030" name="tree.h" local="no" imported="no">cutils/tree.h</includes>
  </compound>
  <compound kind="file">
    <name>tree.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00030</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <class kind="struct">tagTREENODE</class>
    <member kind="define">
      <type>#define</type>
      <name>TREE_NEXTLEVEL_LAST</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>a95b940d1ebbfce437fb43d6cfb4b23e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_NEXTLEVEL_LEFT</name>
      <anchorfile>a00030.html</anchorfile>
      <anchor>a719ee37d46bdbf50728e0b6658f21214</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_CHILD</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaae718f42f4d1d72b715248550610ebe0</anchor>
      <arglist>(current, node)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_CHILD_REVERSE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga6f0686cdd2280d83191f09c671e63ebd</anchor>
      <arglist>(current, node)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_PREORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga78104ec79d0b31f3e06c70b6796a54c8</anchor>
      <arglist>(current, tree)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_PREORDER_END</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gab1367d2f48012bb9b750a0609d1305cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_POSTORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaaeea62e7b7e668075caa6ee635fba60a</anchor>
      <arglist>(current, tree)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_POSTORDER_END</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga01c708ec70f756c8045b16d7fca53f5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagTREENODE</type>
      <name>TREENODE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga105c1d47dd05c5293aff00cecacb8b82</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>TREE_VISITOR</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga10480aa5215e24ed69b820687247e05a</anchor>
      <arglist>)(TREENODE *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>TREE_VISITOR_V</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga69c7ccf722b0f3e72d2f040b62a7020b</anchor>
      <arglist>)(TREENODE *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>TREE_VISITOR_EXT_V</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gad3070030c101f9e06861f273769c333b</anchor>
      <arglist>)(TREENODE *entry, TREE_VISIT_EVENT visit, void *context)</arglist>
    </member>
    <member kind="enumeration">
      <name>TREE_INSERT_MODE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga5700592b3cba85ef8eac8e784d408736</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_INSERT_FIRST</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga5700592b3cba85ef8eac8e784d408736a47aa04f3ffb4563c60becb0546af290c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_INSERT_LAST</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga5700592b3cba85ef8eac8e784d408736ae058f886793e29ef262fee2ef766a6e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>TREE_VISIT_EVENT</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga11e6992d87f7732b07a127c57d5e22f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_PREORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga11e6992d87f7732b07a127c57d5e22f2a809369ffc0a9dc720deb16e631f1a49f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_POSTORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga11e6992d87f7732b07a127c57d5e22f2a04ee042a20e0ad6e886f8870e57b5e4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_init_root</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga0ea2162b901f647f5dac3b15dfecdb12</anchor>
      <arglist>(TREENODE *tree)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_parent</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga27a00a38d3c36cab6ff6adb7a831c4ce</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_left_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga06ef1bfc9046f778cf6c8cc287bec132</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_right_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaa2b3ebe7d515ad3e30c9ad0996109978</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_leftmost_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga1ae823cfa1464a0eca0cd376a107b6d9</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_rightmost_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaff7bf0e8b830dec211d8760b01427d5a</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_first_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga2695e9ae02cd3359771cacdf776918e4</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_last_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga8f8605be102eedc9bb9e49cf343f2a13</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_insert_right_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaa242aa64487dd0db1ee4659a28d9aca9</anchor>
      <arglist>(TREENODE *current, TREENODE *newnode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_insert_left_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga530586b2042c37e278d8666c75978497</anchor>
      <arglist>(TREENODE *current, TREENODE *newnode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_insert_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga604dc40c8dfea4f7beed9b0437dfb496</anchor>
      <arglist>(TREENODE *parent, TREENODE *newnode, TREE_INSERT_MODE mode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_merge_childs</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gab2df5d5218e9b473a61b0cbfdb2ae8d0</anchor>
      <arglist>(TREENODE *parent, TREE_INSERT_MODE mode, TREENODE *newnode)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_unlink_node</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga7beb2cf4338f19f13977cc49b3f2cd43</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>TREE_count_child_nodes</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga53ff6ce0b781ef3165b5239ba466a7a3</anchor>
      <arglist>(TREENODE *current)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_preorder_next</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga52b32e7eea57b597b406c36265a1639a</anchor>
      <arglist>(TREENODE *current)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga0843046582beafa1e2158cf0261fe8a1</anchor>
      <arglist>(TREENODE *lst, TREE_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_child_reverse</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gacb91cd0e28147dc399cba4cc4ddea1c1</anchor>
      <arglist>(TREENODE *lst, TREE_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_findif_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga2a122a49ba6652748765845608a019f3</anchor>
      <arglist>(TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_findif_child_reverse</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga9332aebaeaae4fa46b2668766d07f40c</anchor>
      <arglist>(TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_postorder_next</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga064ab627a56dc9871c3869bf62b56d0e</anchor>
      <arglist>(TREENODE *current, TREENODE *prev)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_preorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gae16d9f6e7f210f3596a00688b0598a05</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR_V visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_find_preorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga5feac0050d51b2ee0886cabe2bcf201b</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_postorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga272a19d0e9bb16460b010d7c38f6038a</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR_V visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_find_postorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga9b8add397903fd2bd602953efa922101</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_check_tree</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga82d94f5f16e87e8dc6047797bda2e080</anchor>
      <arglist>(TREENODE *root)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>util.c</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00031</filename>
    <includes id="a00032" name="util.h" local="no" imported="no">cutils/util.h</includes>
  </compound>
  <compound kind="file">
    <name>util.h</name>
    <path>/home/mmoser/archive/cstuff_bak/cutils/</path>
    <filename>a00032</filename>
    <includes id="a00014" name="base.h" local="no" imported="no">cutils/base.h</includes>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>UTIL_swapbytes</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a0274ebef917fc3eab099d48a7b1d6d40</anchor>
      <arglist>(unsigned char *src, unsigned char *dst, size_t size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>UTIL_is_power_of_two</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a6880ce12ecc30a2745b3f38695084295</anchor>
      <arglist>(uint32_t num)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint32_t</type>
      <name>UTIL_align</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a3eb8ff3c6b6f4d57041c392fadaeb247</anchor>
      <arglist>(uint32_t num, uint32_t align)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>UTIL_ptr_align</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a72e25095046c84ed9c97031ca8a345b2</anchor>
      <arglist>(void *ptr, size_t align)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>UTIL_ptr_page_start</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>ad9e29ba5ed5215ce47a18b3fd195b2c6</anchor>
      <arglist>(void *ptr, size_t page_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>UTIL_ptr_is_aligned</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>aec4a23e0db37c75ecd6ac13dc58eb431</anchor>
      <arglist>(void *ptr, size_t page_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>UTIL_ptr_in_range</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a96499778bf3317f2514da193548a748d</anchor>
      <arglist>(void *ptr, void *from, void *to)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>UTIL_round_to_power_of_n</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a191e663cb1cc920f6b50f17567ea8c5c</anchor>
      <arglist>(size_t num)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>UTIL_log_base_2_of_n</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>ae3076057de2f9b1fcb084284a73037df</anchor>
      <arglist>(size_t num)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>UTIL_skip_spaces</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a0f8d2184dce7443c70508dc31f786849</anchor>
      <arglist>(const char *p)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE char *</type>
      <name>UTIL_skip_nspace</name>
      <anchorfile>a00032.html</anchorfile>
      <anchor>a08de89165d6c4a729e3ff056a9736da6</anchor>
      <arglist>(const char *p)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>ARRAY</name>
    <title>ARRAY</title>
    <filename>a00033.html</filename>
    <class kind="struct">ARRAY</class>
    <member kind="define">
      <type>#define</type>
      <name>ARRAY_init_stack</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga563145759e08f5ec765057251f720ebf</anchor>
      <arglist>(arr, elmsize, numofelem)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>ARRAY_VISITOR</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga4787a36c1aa147cb273c6ea71d28f47b</anchor>
      <arglist>)(int index, void *elm, size_t elmsize, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_init</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga694aa4139823d620f5bb031b49aa4f0f</anchor>
      <arglist>(ARRAY *arr, size_t elmsize, size_t numofelem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_init_fixed</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gafd44eaffad9a4b06e18dfdfb7e6c9652</anchor>
      <arglist>(ARRAY *arr, size_t elmsize, void *ptr, size_t bufsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_free</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gafed6207d4e648f9653cb2bb5a5ceec43</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_reset</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga0a21f05148b610f008ab77af9146971e</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>ARRAY_size</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaac3845deec6b60a387315965cf64d267</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>ARRAY_maxsize</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga39032c979a9bb46994242a38d0832339</anchor>
      <arglist>(ARRAY *arr)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_resize</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa1332b32680cff38fd71bcc58930bb86</anchor>
      <arglist>(ARRAY *arr, size_t num_elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE uint8_t *</type>
      <name>ARRAY_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga741b102f833e0ee35d7413d12c57f136</anchor>
      <arglist>(ARRAY *arr, size_t index)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_copy_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaffc77bae46165067e9b7894e7007d9c7</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_insert_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga178c95e99fb2c218963f440c295c8380</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ARRAY_set_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa527efc4a7222882143f6eaf88474c2e</anchor>
      <arglist>(ARRAY *arr, size_t index, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_delete_at</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga476adf0145ce1808b2427ff0bf4cd6c0</anchor>
      <arglist>(ARRAY *arr, size_t index)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_push_back</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga557c0fb20ce197428a69374b47e387e0</anchor>
      <arglist>(ARRAY *arr, void *elm, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_pop_back</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga5c8e00dc4d38ebdb2da305e91887ffdd</anchor>
      <arglist>(ARRAY *arr, void *ptr, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_stack_top</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gae28547acd38f72d116742b8009ed9e07</anchor>
      <arglist>(ARRAY *arr, void *ptr, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_foreach</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga107782865cee682eba1fc89e764f5e03</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_foreach_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gab0f58e396b81f03d0d96ddbd55672045</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>ARRAY_foreach_reverse</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga348bcb863616489982859044a4907c05</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_foreach_reverse_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga3bb873bda9ffaa686071a4ec35307636</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga8862f23900d93ed5753bde771f556414</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga903c986f29a84e8ea1b01583f03dac0b</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_reverse</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>ga41162038e45941474c4b9f7e5254e6d9</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, uint32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>ARRAY_findif_reverse_range</name>
      <anchorfile>a00033.html</anchorfile>
      <anchor>gaa4564f7eefb2ff3f1ca290c216629632</anchor>
      <arglist>(ARRAY *arr, ARRAY_VISITOR eval, void *context, int from_idx, int to_idx, uint32_t *retval)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HASH_entry</name>
    <title>HASH_entry</title>
    <filename>a00034.html</filename>
    <class kind="struct">HASH_Entry</class>
    <subgroup>HASH</subgroup>
    <member kind="typedef">
      <type>HASH_VALUE(*</type>
      <name>HASH_FUNCTION</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga11718c21601b9ee597d31cb7e720c260</anchor>
      <arglist>)(void *data, ssize_t length)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HASH_COMPARE_KEY</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gaf9eab147ddd898a2d7fb376e1f8753a3</anchor>
      <arglist>)(HASH_Entry *, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>HASH_VISITOR</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga1bcff3f62d46d79b790bed89891cd9a5</anchor>
      <arglist>)(HASH_Entry *, void *context)</arglist>
    </member>
    <member kind="variable">
      <type>HASH_VALUE</type>
      <name>hash</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga0a321e75db9083da90e2557e6cc24de8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ismultimap</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga1e56521c716b07928dc326eced3b50c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>buckets_size</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga13b4c75f393b997dc965cef4fc2508c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gadfd1d7fb9add98a87d2852e45c7cd018</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>resize_threshold</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga00bd0beb9248ca36fff916569eb71526</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>mult_load_factor</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga8713031ba9a40fa717f1d3b8114691c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>div_load_factor</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga6f4e674640c97d88dfd2e557d660f147</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH_FUNCTION</type>
      <name>hash_func</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gaa8d9ed13981a492e221f55d4a0ff8a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH_COMPARE_KEY</type>
      <name>compare_key</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga9f012f5c0abfa8de8f6ff6d98a3d5f83</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HASH</name>
    <title>HASH</title>
    <filename>a00035.html</filename>
    <class kind="struct">HASH</class>
    <member kind="define">
      <type>#define</type>
      <name>HASH_BUCKET_FOREACH</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gad6e4f490068b19fa3fea075d3b0180f7</anchor>
      <arglist>(cur, bucket)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH_KEY</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga7a866beabda7e0bd95e19ae7c708c8c0</anchor>
      <arglist>(cur, hash, key, key_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_KEY</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gad5d12368e1648a383d048e7fed56fa82</anchor>
      <arglist>(cur, hash, key, key_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_KEY_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga6bdd6abcda051166ee1ce8586b80cd8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga635ee4184a9ca1e86138aec673a0a9b1</anchor>
      <arglist>(cur, hash)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_FOREACH_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga1dd13a6578ab349c2cbac48304a0f5e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga1193ff43ed7c2f743c974d75c39efc93</anchor>
      <arglist>(cur, hash)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HASH_DELETEALL_END</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gaee7185cd320df1c4a801762c8f5201b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_init</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga73ce0ca18399c5db0bac0491aeadba8d</anchor>
      <arglist>(HASH *hash, size_t buckets, int ismultimap, HASH_COMPARE_KEY compare_key, HASH_FUNCTION hash_func)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_resize</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gacef4d862b25db1563ce621265299fb2e</anchor>
      <arglist>(HASH *hash, size_t buckets)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_free</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga889a4afc9437f453ed7db27d7bd39d3d</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HASH_size</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga8924e383f82b78dcb693ffcab49188b0</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga072fef06fb9a516f47771ddd3946bf20</anchor>
      <arglist>(HASH *phash, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_find_next</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga2f20017ef2a49dc235a8bea78b254dd6</anchor>
      <arglist>(HASH *phash, HASH_Entry *prev, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HASH_insert</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>gaf9ce37d092fe7b3ae217336ef623a7dc</anchor>
      <arglist>(HASH *phash, HASH_Entry *entry, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>HASH_Entry *</type>
      <name>HASH_unlink</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga9546382a27adc8b91d70cd6e4c4723c3</anchor>
      <arglist>(HASH *hash, void *key, ssize_t key_size)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_foreach_key</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga96c141f22c63392dfe758de963ffe8f5</anchor>
      <arglist>(HASH *hash, void *key, ssize_t key_size, HASH_VISITOR eval_func, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_foreach</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga251e1a1fd0e312bae674631001fd4b55</anchor>
      <arglist>(HASH *hash, HASH_VISITOR eval_func, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE HASH_Entry *</type>
      <name>HASH_findif</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga58acba9b6611fe07a44253eeb4307e09</anchor>
      <arglist>(HASH *hash, HASH_VISITOR eval_func, void *context, int *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HASH_deleteall</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga0ed6fe026b7e8aef4611325ab2ffdc47</anchor>
      <arglist>(HASH *hash, int offset_of_link, HASH_VISITOR on_delete, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HASH_check</name>
      <anchorfile>a00035.html</anchorfile>
      <anchor>ga3482872e1f844042afc76de5136bff3f</anchor>
      <arglist>(HASH *hash)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DLIST_entry</name>
    <title>DLIST_entry</title>
    <filename>a00036.html</filename>
    <class kind="struct">tagDLIST_entry</class>
    <subgroup>DLIST</subgroup>
    <member kind="typedef">
      <type>struct tagDLIST_entry</type>
      <name>DLIST_entry</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga8bcfc120d0cdb50aa5e418d3c7dd1bdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagDLIST_entry *</type>
      <name>prev</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga6353911f6d3fc5c023e5bc6c520adb34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLIST_entry</type>
      <name>root</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga68fa6740c5444af3c0bda5bc2f78bff4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DLIST</name>
    <title>DLIST</title>
    <filename>a00037.html</filename>
    <class kind="struct">DLIST</class>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga563984e90677ff397213299a05ca8d04</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_REVERSE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gab21653656a5607e487327ed381a42ffc</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_SAVE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaa8491f4f93ae66bcd0006914484ec013</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLIST_FOREACH_REVERSE_SAVE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga46f8bbaa697f05c1f4e444ffd04f72d1</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DLIST_VISITOR_V</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac53c87d52fbf30ec94612007d2838089</anchor>
      <arglist>)(DLIST *list, DLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DLIST_VISITOR</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga54b6468514a681ad99c543b7e1f94a32</anchor>
      <arglist>)(DLIST *list, DLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DLIST_COMPARE</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gad66eadd6b35614c8b18f1687db7ce300</anchor>
      <arglist>)(DLIST_entry *, DLIST_entry *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_init</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4b32655ea9e332581a0bab18857f8adb</anchor>
      <arglist>(DLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_isempty</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gacbf81693b507a588b1abcd3377471558</anchor>
      <arglist>(DLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_insert_before</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga572203aad7a7d974bcffc50baef3105d</anchor>
      <arglist>(DLIST *list, DLIST_entry *pos, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_insert_after</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac7af205a388bf32b16cb8b7c8ecc32f4</anchor>
      <arglist>(DLIST *list, DLIST_entry *pos, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_unlink</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga9bbc2c5bac97e9ed491c8fd6f6eae1ce</anchor>
      <arglist>(DLIST *list, DLIST_entry *link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_push_back</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gafbf7e8463b8a2615c35175e1b804b8df</anchor>
      <arglist>(DLIST *list, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_push_front</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4da016f9927a26c8d90fe81416684bc7</anchor>
      <arglist>(DLIST *list, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_pop_front</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga9145408d5a80137e5660f32c21029d9b</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_pop_back</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga32a3f6d0de35b46dd011702f5873bb72</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_first</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaefa144f015d6a1283afcb2716b0dc747</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_last</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gae039944967f35a70066609fc5d016942</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_next</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga6af1da3a12a2ca6b60695bbb083d3198</anchor>
      <arglist>(DLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_prev</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gac17b78519ee5c430e2eb4d471e2713ee</anchor>
      <arglist>(DLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DLIST_size</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga1884ac42bd98ebbf0b7f71bdd67579b6</anchor>
      <arglist>(DLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_nth</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaff66f931cabc3fbcf6e420135a135877</anchor>
      <arglist>(DLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_get_nth_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga4c8a5673bcc07dd3f5f4f7736f7a798d</anchor>
      <arglist>(DLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_foreach</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaf6188d6ec22905f7ea11335c30b1627f</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_foreach_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga120a5162547a971d45a2d6446a9c4273</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR_V eval, void *context, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_findif</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaec008957a6a9b8aca1f29e691f8d929c</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DLIST_entry *</type>
      <name>DLIST_findif_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga37989ca126a98eeab2a6a4310b1edf70</anchor>
      <arglist>(DLIST *lst, DLIST_VISITOR eval, void *context, int32_t *retval, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_deleteif</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gab24282d74d6ac2ff24b28e213d3cea15</anchor>
      <arglist>(DLIST *list, DLIST_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_deleteall</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>gaa0e8e5379952c2c61e373f576c047792</anchor>
      <arglist>(DLIST *list, DLIST_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_insert_sorted</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga82c92d75b00d2d927eeb44c9413cc1f6</anchor>
      <arglist>(DLIST *list, DLIST_COMPARE compare, DLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DLIST_reverse</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga078d4f221e97947eeebb89877973a9cf</anchor>
      <arglist>(DLIST *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DLIST_check</name>
      <anchorfile>a00037.html</anchorfile>
      <anchor>ga3152f24ad9c9f817968e6eb04d7f8474</anchor>
      <arglist>(DLIST *header)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DRING</name>
    <title>DRING</title>
    <filename>a00038.html</filename>
    <class kind="struct">tagDRING</class>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac8118521928cd5a00b83ee9c0ba48308</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_REVERSE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga550a7b5adbe2d3325db38c49764bdc8c</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_SAVE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaab96829db58e4eaa3cec725c9c99ff69</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DRING_FOREACH_REVERSE_SAVE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac5d159651c812e6785f81bc2dfc73b7d</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagDRING</type>
      <name>DRING</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga4630800b5e50c32e36110b0a6dfcadc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DRING_VISITOR_V</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga516498a8d915f013d2466856c72a0bd6</anchor>
      <arglist>)(DRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DRING_VISITOR</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gadbbb7e55996e9a7bc35f1f5bd26b75d0</anchor>
      <arglist>)(DRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>DRING_COMPARE</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga275575e4b5e7edcaaf471ec1e5f02b5a</anchor>
      <arglist>)(DRING *, DRING *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_init</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaea5e890f7d1dab572fe3ef6f067e5d26</anchor>
      <arglist>(DRING *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DRING_isempty</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga7b28ffdc594d835f2c49cf9e74f7c3b3</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_insert_before</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga93f762e668a7283b18025310886635ea</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_insert_after</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gab65d72bd525d11559228b5aa3aa641f0</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_insert_sorted</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gad8c4485c62148db72ca05f55dcc75709</anchor>
      <arglist>(DRING *list, DRING_COMPARE compare, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_unlink</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga81d2db3ee3b83fe28393d45530bed230</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_push_back</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaed724487f6cd26c0b79b875ff3390c61</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_push_front</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga897228c60f1de15be0799dadea9d0337</anchor>
      <arglist>(DRING *list, DRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_pop_front</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga315bb0f49e0ccbadcc59a5e9258eff19</anchor>
      <arglist>(DRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_pop_back</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga50912d37cc30e961dd53ff291b8db05e</anchor>
      <arglist>(DRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_first</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaa392ee3a23da21ccf4e4ea62bd6c7de8</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_last</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gac4fbe3b2aa6b11ce356755c89235b9d4</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_next</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga44a51594b007ec843d30bf5c3b3ddc73</anchor>
      <arglist>(DRING *end, DRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_prev</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gae8038a9a107a06bb0fae04cd8f8f7711</anchor>
      <arglist>(DRING *end, DRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DRING_size</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gafb5caaea75f7196bd6abe1ac8e81a7f3</anchor>
      <arglist>(DRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_nth</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga7f552929105d5cf0a6f621c6e8463f2d</anchor>
      <arglist>(DRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_get_nth_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga82d60185895dcf1deecd14d4a809741f</anchor>
      <arglist>(DRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gacc803a0c5a8bc3ce165f8d8fe5e78ab1</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DRING_foreach_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga98724ac564245aa77a1a4c55f83fadb9</anchor>
      <arglist>(DRING *lst, DRING_VISITOR_V eval, void *context, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_findif</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga3efd1408b3d08b2253e1bf781306a425</anchor>
      <arglist>(DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_del)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE DRING *</type>
      <name>DRING_find_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga8d223c41dde7af2a4a154e09bf9c3a03</anchor>
      <arglist>(DRING *lst, DRING_VISITOR eval, void *context, int32_t *retval, int save_from_delete)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>DRING_deleteif</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga44b92d9180c58865b9d4a1c6d5d9c37f</anchor>
      <arglist>(DRING *list, DRING_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_deleteall</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gab49cbee747109aad10c6e6a67c1149d9</anchor>
      <arglist>(DRING *list, DRING_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>DRING_reverse</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>ga02284390005b102707b116d7de2b7ae6</anchor>
      <arglist>(DRING *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>DRING_check</name>
      <anchorfile>a00038.html</anchorfile>
      <anchor>gaead03ab9284bb52e1a5204010109b69d</anchor>
      <arglist>(DRING *header)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>HEAP</name>
    <title>HEAP</title>
    <filename>a00039.html</filename>
    <class kind="struct">HEAP</class>
    <member kind="typedef">
      <type>int(*</type>
      <name>HEAP_VISITOR</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga13498c6ee613dce46c5f505dc66fa6dc</anchor>
      <arglist>)(int index, void *elm, size_t elmsize, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HEAP_init</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac152cfd9171fdc1cee9f7d08ba85f6c4</anchor>
      <arglist>(HEAP *heap, size_t elmcount, size_t elmsize, HEAP_compare compare_func)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HEAP_free</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga74a671a1104e9a2d48a430427a2f729b</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_size</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4c1edfd6bdc79001bb92fcd20efc0058</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_maxsize</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4f0ddfd22ffd30752a88c25c27508ff1</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>HEAP_elmsize</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga2a7f7199e6201e88740ee037daef1027</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>HEAP_top</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga3cdfaf0ef3e2728601b89c445b938b31</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void *</type>
      <name>HEAP_get_at</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac1bbf31bcce42f7b63433f0ba732a1bf</anchor>
      <arglist>(HEAP *heap, int index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_pop</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gadb9cc342dba8eb29fe8be37e56a16370</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_push</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gaf33187b5e3ea29b6da1ccf4b2490bc7f</anchor>
      <arglist>(HEAP *heap, void *element, size_t elmsize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>HEAP_check</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga689c175c301ff3025290d37ca5800039</anchor>
      <arglist>(HEAP *heap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HEAP_foreach_sorted</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga72dd058aadb08417c5fcb618e9be6833</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>HEAP_foreach</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>ga4bc6463fb11b07c4fa67125675342066</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>HEAP_findif</name>
      <anchorfile>a00039.html</anchorfile>
      <anchor>gac2fb2dbcb7d6f5b2d394046334a696f4</anchor>
      <arglist>(HEAP *heap, HEAP_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SLIST_entry</name>
    <title>SLIST_entry</title>
    <filename>a00040.html</filename>
    <class kind="struct">tagSLIST_entry</class>
    <member kind="typedef">
      <type>struct tagSLIST_entry</type>
      <name>SLIST_entry</name>
      <anchorfile>a00040.html</anchorfile>
      <anchor>gaacb00d1a4daaa75eee59c1483cb57b80</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SLIST</name>
    <title>SLIST</title>
    <filename>a00041.html</filename>
    <class kind="struct">SLIST</class>
    <member kind="define">
      <type>#define</type>
      <name>SLIST_FOREACH</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga13e3414add243765e1acc420f8a0f478</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SLIST_FOREACH_SAVE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1fd8f8e8cee04217d15bc9e40dc22874</anchor>
      <arglist>(loopvarname, loopvarnamenext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>SLIST_VISITOR_V</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gac7e7623e772b63394d7aa00c570f8e84</anchor>
      <arglist>)(SLIST *list, SLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SLIST_VISITOR</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga7b248c4eed2d3b3cf32fc0329063de57</anchor>
      <arglist>)(SLIST *list, SLIST_entry *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SLIST_COMPARE</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga986253a6f00466c9a8fc577922946578</anchor>
      <arglist>)(SLIST_entry *, SLIST_entry *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_init</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gad4951a8a17c4c71dbccf87f25e1d0cc9</anchor>
      <arglist>(SLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SLIST_isempty</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga167ddd6e77f962055cbf23edfae47a55</anchor>
      <arglist>(SLIST *head)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_insert_after</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga28b526884512439a095d0a1f8defbf7a</anchor>
      <arglist>(SLIST *list, SLIST_entry *pos, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_unlink_after</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0242e7614311873394c09036c3f8db31</anchor>
      <arglist>(SLIST *list, SLIST_entry *link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_push_back</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga162430e86a10d44d37f06f3286336e39</anchor>
      <arglist>(SLIST *list, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_push_front</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9640eb240d27802e9762df5eb890751f</anchor>
      <arglist>(SLIST *list, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_append</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga496c6461f8655694a2163dbf2cc2ce3c</anchor>
      <arglist>(SLIST *dest, SLIST *src)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_pop_front</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1f0ecce5cb624eee52d5d1385b355296</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_first</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga2e184c75f69621e1ca7d77d762cf3ac4</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_last</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gab1da0b7495c594466c35f43b62ba2fa6</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_next</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0377a487001bec48ddfccf6e3f72653a</anchor>
      <arglist>(SLIST_entry *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SLIST_size</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gae1806eb6dd23d1489e2a3a87c16a7c89</anchor>
      <arglist>(SLIST *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_nth</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga2b16d0912f1762278bd39a3166f2937a</anchor>
      <arglist>(SLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_get_nth_reverse</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga4ed5d1aa0c00b8e6e7208a136cf9836f</anchor>
      <arglist>(SLIST *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_foreach</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9a2bcb303e10ce22f43138d974cf6f61</anchor>
      <arglist>(SLIST *lst, SLIST_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SLIST_entry *</type>
      <name>SLIST_findif</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gab7659684a1854619f6514007a4a2a7ac</anchor>
      <arglist>(SLIST *lst, SLIST_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_insert_sorted</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga9d6b9df04f353039015f1f96349d6daf</anchor>
      <arglist>(SLIST *list, SLIST_COMPARE compare, SLIST_entry *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_deleteif</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>gaa33b9f8d6cfd2573bd33e49ef0dd9b62</anchor>
      <arglist>(SLIST *list, SLIST_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_deleteall</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga1c75f1166c0c6048c115f0de18c2dde2</anchor>
      <arglist>(SLIST *list, SLIST_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SLIST_reverse</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga0979079d34e30779ee70a356bc46371f</anchor>
      <arglist>(SLIST *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SLIST_check</name>
      <anchorfile>a00041.html</anchorfile>
      <anchor>ga69ab9537af27dcf8890148ccdc8d5a27</anchor>
      <arglist>(SLIST *header)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SRING</name>
    <title>SRING</title>
    <filename>a00042.html</filename>
    <class kind="struct">tagSRING</class>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga39582582e3bfccf18e39710e75009ad1</anchor>
      <arglist>(loopvarname, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH_SAVE</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga7f6178ad165e9b2f699efd88a3cbca8a</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRING_FOREACH_SAVE_END</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga0e4892ffac6b949a95b1d9b1895433b7</anchor>
      <arglist>(loopvarname, loopvarnext, list)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagSRING</type>
      <name>SRING</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga478bf88a63498eec67b9720586d6bba9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>SRING_VISITOR_V</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga8fd5f1f287aecfe1a45ad63bd0c38fca</anchor>
      <arglist>)(SRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SRING_VISITOR</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gab481e0fe056d133b74281971cb70a90f</anchor>
      <arglist>)(SRING *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>SRING_COMPARE</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaa197bf51f9b33ec012296a14be9c4838</anchor>
      <arglist>)(SRING *, SRING *)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_init</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga53ac9788c8cdc9e03a9e343fd7e8b15a</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>SRING_isempty</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga23aab5b8a7efeacf278b444bb785bc1d</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_insert_after</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga6a2a1950260f3fb3d385bde480e5c976</anchor>
      <arglist>(SRING *list, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_unlink_after</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gab573cc304039b055762f9251cc551d36</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_push_front</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga608f3b7787a81834f15eddbc4135d04f</anchor>
      <arglist>(SRING *list, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_pop_front</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga1702358f1edcb1d85df818fdb8a20a23</anchor>
      <arglist>(SRING *elem)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_first</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaa43e9213ae7d390a22ffd34164a9a8fb</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_last</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga9bfd87c38a546d0bd880e2ef6afb4308</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_next</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga02277d246cf87fa74cf2face4271dc46</anchor>
      <arglist>(SRING *end, SRING *cur)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_size</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gad00dfbc796c0f39381f8cb207d45a628</anchor>
      <arglist>(SRING *list)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_nth</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga0bf1663e78699f16d176b4e5eab35760</anchor>
      <arglist>(SRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_get_nth_reverse</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga4eacc9ad78693d75fc82960853bfc77d</anchor>
      <arglist>(SRING *list, size_t nth)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_foreach</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga948f45c8af2b99aec05ca538b4a07e9e</anchor>
      <arglist>(SRING *lst, SRING_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE SRING *</type>
      <name>SRING_findif</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga742710ac325213e24ddf5d02ef30bc9c</anchor>
      <arglist>(SRING *lst, SRING_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_deleteif</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga182b8c409d5c05637085210b31950431</anchor>
      <arglist>(SRING *list, SRING_VISITOR check_if, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_deleteall</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga96914d218afb3f3dac01bf98702e3ef7</anchor>
      <arglist>(SRING *list, SRING_VISITOR_V on_delete, void *context, int offset_of_link)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_insert_sorted</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga6a5cdf3eb278daa953e2f45dc4cb4518</anchor>
      <arglist>(SRING *list, SRING_COMPARE compare, SRING *newentry)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>SRING_reverse</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>gaff2e874e90de5dbc9c321146a302cba9</anchor>
      <arglist>(SRING *lst)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>SRING_check</name>
      <anchorfile>a00042.html</anchorfile>
      <anchor>ga7d49e006bf25f2d83d15b70caf9f3254</anchor>
      <arglist>(SRING *head)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>TREE</name>
    <title>TREE</title>
    <filename>a00043.html</filename>
    <class kind="struct">tagTREENODE</class>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_CHILD</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaae718f42f4d1d72b715248550610ebe0</anchor>
      <arglist>(current, node)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_CHILD_REVERSE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga6f0686cdd2280d83191f09c671e63ebd</anchor>
      <arglist>(current, node)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_PREORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga78104ec79d0b31f3e06c70b6796a54c8</anchor>
      <arglist>(current, tree)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_PREORDER_END</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gab1367d2f48012bb9b750a0609d1305cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_POSTORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaaeea62e7b7e668075caa6ee635fba60a</anchor>
      <arglist>(current, tree)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TREE_FOREACH_POSTORDER_END</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga01c708ec70f756c8045b16d7fca53f5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagTREENODE</type>
      <name>TREENODE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga105c1d47dd05c5293aff00cecacb8b82</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>TREE_VISITOR</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga10480aa5215e24ed69b820687247e05a</anchor>
      <arglist>)(TREENODE *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>TREE_VISITOR_V</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga69c7ccf722b0f3e72d2f040b62a7020b</anchor>
      <arglist>)(TREENODE *entry, void *context)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>TREE_VISITOR_EXT_V</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gad3070030c101f9e06861f273769c333b</anchor>
      <arglist>)(TREENODE *entry, TREE_VISIT_EVENT visit, void *context)</arglist>
    </member>
    <member kind="enumeration">
      <name>TREE_INSERT_MODE</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga5700592b3cba85ef8eac8e784d408736</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_INSERT_FIRST</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga5700592b3cba85ef8eac8e784d408736a47aa04f3ffb4563c60becb0546af290c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_INSERT_LAST</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga5700592b3cba85ef8eac8e784d408736ae058f886793e29ef262fee2ef766a6e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>TREE_VISIT_EVENT</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga11e6992d87f7732b07a127c57d5e22f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_PREORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga11e6992d87f7732b07a127c57d5e22f2a809369ffc0a9dc720deb16e631f1a49f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TREE_POSTORDER</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gga11e6992d87f7732b07a127c57d5e22f2a04ee042a20e0ad6e886f8870e57b5e4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_init_root</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga0ea2162b901f647f5dac3b15dfecdb12</anchor>
      <arglist>(TREENODE *tree)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_parent</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga27a00a38d3c36cab6ff6adb7a831c4ce</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_left_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga06ef1bfc9046f778cf6c8cc287bec132</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_right_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaa2b3ebe7d515ad3e30c9ad0996109978</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_leftmost_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga1ae823cfa1464a0eca0cd376a107b6d9</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_rightmost_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaff7bf0e8b830dec211d8760b01427d5a</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_first_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga2695e9ae02cd3359771cacdf776918e4</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_last_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga8f8605be102eedc9bb9e49cf343f2a13</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_insert_right_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gaa242aa64487dd0db1ee4659a28d9aca9</anchor>
      <arglist>(TREENODE *current, TREENODE *newnode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_insert_left_sibling</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga530586b2042c37e278d8666c75978497</anchor>
      <arglist>(TREENODE *current, TREENODE *newnode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_insert_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga604dc40c8dfea4f7beed9b0437dfb496</anchor>
      <arglist>(TREENODE *parent, TREENODE *newnode, TREE_INSERT_MODE mode, int node_is_leaf)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_merge_childs</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gab2df5d5218e9b473a61b0cbfdb2ae8d0</anchor>
      <arglist>(TREENODE *parent, TREE_INSERT_MODE mode, TREENODE *newnode)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_unlink_node</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga7beb2cf4338f19f13977cc49b3f2cd43</anchor>
      <arglist>(TREENODE *node)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE size_t</type>
      <name>TREE_count_child_nodes</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga53ff6ce0b781ef3165b5239ba466a7a3</anchor>
      <arglist>(TREENODE *current)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_preorder_next</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga52b32e7eea57b597b406c36265a1639a</anchor>
      <arglist>(TREENODE *current)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga0843046582beafa1e2158cf0261fe8a1</anchor>
      <arglist>(TREENODE *lst, TREE_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_child_reverse</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gacb91cd0e28147dc399cba4cc4ddea1c1</anchor>
      <arglist>(TREENODE *lst, TREE_VISITOR_V eval, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_findif_child</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga2a122a49ba6652748765845608a019f3</anchor>
      <arglist>(TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_findif_child_reverse</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga9332aebaeaae4fa46b2668766d07f40c</anchor>
      <arglist>(TREENODE *tree, TREE_VISITOR eval, void *context, int32_t *retval)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_postorder_next</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga064ab627a56dc9871c3869bf62b56d0e</anchor>
      <arglist>(TREENODE *current, TREENODE *prev)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_preorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>gae16d9f6e7f210f3596a00688b0598a05</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR_V visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_find_preorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga5feac0050d51b2ee0886cabe2bcf201b</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE void</type>
      <name>TREE_foreach_postorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga272a19d0e9bb16460b010d7c38f6038a</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR_V visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE TREENODE *</type>
      <name>TREE_find_postorder</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga9b8add397903fd2bd602953efa922101</anchor>
      <arglist>(TREENODE *node, TREE_VISITOR visit, void *context)</arglist>
    </member>
    <member kind="function">
      <type>M_INLINE int</type>
      <name>TREE_check_tree</name>
      <anchorfile>a00043.html</anchorfile>
      <anchor>ga82d94f5f16e87e8dc6047797bda2e080</anchor>
      <arglist>(TREENODE *root)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ARRAY</name>
    <filename>a00001.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>elmmaxcount</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a6be9a6ad3322cf633af6817b094c0c2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmsize</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a0dafbfd308171034ac914536e7410ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>aa48e77e31b8f17e52723e568a9be9ff4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>buffer</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a48eec4b964d0ca087087e2dca6a50907</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DLIST</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a0a8db3c6c9143e389c537a1920c278aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DLIST_entry</type>
      <name>root</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga68fa6740c5444af3c0bda5bc2f78bff4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HASH</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>SRING *</type>
      <name>buckets</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>a7ec749b263c20879444d18b7ab4824e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ismultimap</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga1e56521c716b07928dc326eced3b50c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>buckets_size</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga13b4c75f393b997dc965cef4fc2508c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gadfd1d7fb9add98a87d2852e45c7cd018</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>resize_threshold</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga00bd0beb9248ca36fff916569eb71526</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>mult_load_factor</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga8713031ba9a40fa717f1d3b8114691c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>div_load_factor</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga6f4e674640c97d88dfd2e557d660f147</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH_FUNCTION</type>
      <name>hash_func</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>gaa8d9ed13981a492e221f55d4a0ff8a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH_COMPARE_KEY</type>
      <name>compare_key</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga9f012f5c0abfa8de8f6ff6d98a3d5f83</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HASH_Entry</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>SRING</type>
      <name>entry</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a41027b39fbfa250c8b29c79a4634e7f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH_VALUE</type>
      <name>hash</name>
      <anchorfile>a00034.html</anchorfile>
      <anchor>ga0a321e75db9083da90e2557e6cc24de8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HEAP</name>
    <filename>a00005.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a0dd2675a99a68e7109d4d890fb4ce8f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmmaxcount</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a11522bc8e4a0ea00b3ce3b6893eba413</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>elmsize</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>aae914a0aafd256e96e7742eaed23633a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>buffer</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>ae4ae987dedca7bef9a8eb5628a8af092</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HEAP_compare</type>
      <name>compare_func</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a58d5a3131976055b67894a0ba8467210</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SLIST</name>
    <filename>a00006.html</filename>
    <member kind="variable">
      <type>size_t</type>
      <name>elmcount</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a7361a7792b34b35b4bdd5bdcace39d38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SLIST_entry *</type>
      <name>prev</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a1277c2a660611df18e9e9f22c7ed8717</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SLIST_entry *</type>
      <name>next</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>aec4f4d9f8363c74fdbf19e4e48e5138b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagDLIST_entry</name>
    <filename>a00007.html</filename>
    <member kind="variable">
      <type>struct tagDLIST_entry *</type>
      <name>next</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aa0d0a39a702ab45e678b018be2ad3d38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagDLIST_entry *</type>
      <name>prev</name>
      <anchorfile>a00036.html</anchorfile>
      <anchor>ga6353911f6d3fc5c023e5bc6c520adb34</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagDRING</name>
    <filename>a00008.html</filename>
    <member kind="variable">
      <type>struct tagDRING *</type>
      <name>next</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aec13745a49e3ff2cd72745c0ff3e4e45</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagDRING *</type>
      <name>prev</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a8f2313e1ce7ba8c157ce612faad73aad</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSLIST_entry</name>
    <filename>a00009.html</filename>
    <member kind="variable">
      <type>struct tagSLIST_entry *</type>
      <name>next</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>aa1047cea85e491e7d1ae1485e8fc7263</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagSRING</name>
    <filename>a00010.html</filename>
    <member kind="variable">
      <type>struct tagSRING *</type>
      <name>next</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a9842e33c47a3fff2a6ddec4078ef7190</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagTREENODE</name>
    <filename>a00011.html</filename>
    <member kind="variable">
      <type>struct tagTREENODE *</type>
      <name>parent</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a3be948eaba1ce4c959c4211c487e775e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagTREENODE *</type>
      <name>nextlevel_first</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>af76e9208f6c55ff1069fde7b66c3f232</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagTREENODE *</type>
      <name>nextlevel_last</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>aa851db5a7e782b18f81d32fa31b58440</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagTREENODE *</type>
      <name>right</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a3e635860e1ac41723bfca6cb3417af95</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagTREENODE *</type>
      <name>left</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a5606167dda105adf0db9728c2004f2f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
