<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title></title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>cuitestrunner.c</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00005</filename>
    <includes id="a00008" name="vtest.h" local="yes" imported="no">vtest.h</includes>
    <includes id="a00010" name="vtestrunner.h" local="yes" imported="no">vtestrunner.h</includes>
    <includes id="a00009" name="vtestcui.h" local="yes" imported="no">vtestcui.h</includes>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>MSG</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a9dfa546021b64697f491654585c90f2b</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VTEST_CUI_set_debug_output_hook</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a35aaeea65a742eab0299b77e423fae2c</anchor>
      <arglist>(PFN_DEBUG_FUNCTION debug_output_hook_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CUI_report_suite_start</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>aa624f16d5c909b150b236004fcdf1355</anchor>
      <arglist>(const char *suite_name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CUI_report_test_start</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a43cae7f0efc99960580f207891498f80</anchor>
      <arglist>(const char *suite_name, const char *test_name, int iteration, int maxiteration)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CUI_report_results</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a352188f0c11f4d7d15b8ceaa647fd4e5</anchor>
      <arglist>(VTEST_STATUS status, const char *suite_name, const char *test_name, struct timeval *duration, const char *fail_cond, const char *fail_file, int fail_line)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CUI_report_wrapup</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a40db9ec73ebb6d79b7abad4ee443d274</anchor>
      <arglist>(int suitesinitfailed, int suitesteardownfailed, int tests_passed, int tests_failed, int testnotrun)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_CUI_test_runner</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>ab57fc055a968dc550bded8e2e0e2e98b</anchor>
      <arglist>(VTEST_TEST_SUITE *suite)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_CUI_test_runner_cmdline</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>afe34205fbd267e1174d1d405983d3179</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, int argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VTEST_CUI_list_suite</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a5b0105cc019c019f7faf51d0d2f6f471</anchor>
      <arglist>(VTEST_TEST_SUITE *suite)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_CUI_list_tests_of_suite</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a994c54aa6d85aa7db61592394e33aca7</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, const char *suite_name)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static PFN_DEBUG_FUNCTION</type>
      <name>debug_output_hook</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>a89d5e1526988bb8bf9d9bdd4fb7e9b87</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>example.c</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00006</filename>
    <includes id="a00008" name="vtest.h" local="yes" imported="no">vtest.h</includes>
    <includes id="a00009" name="vtestcui.h" local="yes" imported="no">vtestcui.h</includes>
    <member kind="function">
      <type>void</type>
      <name>always_fails</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a69980e01737c656b2fc290884242a1e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>always_pass</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a1f6412c924d926b810b771b107f372c4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setUpFails</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a33f3b97a07c7b9e9d34d46d1f958e552</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>VTEST_END_SUITE VTEST_END_SUITE VTEST_END_SUITE int</type>
      <name>main</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a4584b7928a03107561dd3f60c4127af8</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>vtest.c</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00007</filename>
    <includes id="a00008" name="vtest.h" local="yes" imported="no">vtest.h</includes>
    <includes id="a00010" name="vtestrunner.h" local="yes" imported="no">vtestrunner.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>timerclear_timeval</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ae3e2f7b5d5cc123773ac1f3c99fe8d14</anchor>
      <arglist>(tvp)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>timersub_timeval</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aa27119f0dfe5abbd5fcb21a0bdca24d8</anchor>
      <arglist>(a, b, result)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>timeradd_timeval</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a25765cd5bcf3020fd22b203cf6506469</anchor>
      <arglist>(a, b, result)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VFAIL</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ac238eb93c68c155eca5ada030eaeaec9</anchor>
      <arglist>(const char *cond, const char *file, int line)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parse_suite_name</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a1513dea154603f49cb0d46e50659602b</anchor>
      <arglist>(const char *line, char **suite_name, int *len)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>parse_next_test_name</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>adfa4d68daca4820097c72d87bb3c0bc0</anchor>
      <arglist>(const char *line, char **test_name, int *len, char **last_pos)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>VTEST_is_run_suite</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>ab8c1dd9c6da0bf795cc70a1feeef6292</anchor>
      <arglist>(const char *suite_name, char *argv[], int argc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>VTEST_is_run_test</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a2578781a977cfd9eeb86039c8818a42c</anchor>
      <arglist>(const char *suite_name, const char *test_name, char *argv[], int argc)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VTEST_goto_next_suite</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aafd1fce2122fb39ccbc0afb612c59be3</anchor>
      <arglist>(const char *suite_name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static VTEST_TEST_SUITE *</type>
      <name>find_suite_by_name</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a526d7e5ae684f37a5eecacf20fcba6a5</anchor>
      <arglist>(VTEST_TEST_SUITE *first_suite, const char *suite_name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_test_runner_cmdline</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aa917b1c203ce5d52b924f3369956df93</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl, int argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_test_runner</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>aa417fd7f65b276ab08060848510399b4</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl)</arglist>
    </member>
    <member kind="variable">
      <type>VTEST_RUNNER_IMPL *</type>
      <name>vtest_impl</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a53a3558c2850ac9d96eeb795fbfc70c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char *</type>
      <name>g_suite_name</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>a1fab7003576575cdfd86da7a19ae87a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>vtest.h</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00008</filename>
    <class kind="struct">tagVTEST_TEST</class>
    <class kind="struct">tagVTEST_TEST_SUITE</class>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_DEFINE_LAST_SUITE</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a8c22a4adb5e6672ba2ff5b791e83b7cf</anchor>
      <arglist>(name, setUp, tearDown)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_DEFINE_SUITE</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a81a3a26a755a914bb1dc8d21aed7686a</anchor>
      <arglist>(name, setUp, tearDown, nextsuite)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_END_SUITE</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a19f89dad114bee707a093d149b246309</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_TEST_REPEATED</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ae40cf61f03977745b30c70c021a76de2</anchor>
      <arglist>(tname, test_fun, repeat_count)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_TEST</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a40c258d5454c0813cc1507a10f08f7fb</anchor>
      <arglist>(tname, test_fun)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_TESTN</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a9910ddcb6cfb672b9f499d56a3dec825</anchor>
      <arglist>(test_fun)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VCHECK</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>af05c0f4039763345f60f2813ed5d953f</anchor>
      <arglist>(cond)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VASSERT</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a639de122fa28e899c9ebf1f2e2cb0180</anchor>
      <arglist>(cond)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VASSERT_RET</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>aea60d30457d296471e930b952cc4f175</anchor>
      <arglist>(cond, ret)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_SUITE_DECLARE_GET</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a7333efd3082382b7a210af5b60f9463e</anchor>
      <arglist>(name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VTEST_SUITE_GET</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a5978ce16e26bc00963f0fbfb328f950b</anchor>
      <arglist>(name)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>VTEST_ACTION</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ae3080db232fc1a680902449dccc65a2a</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagVTEST_TEST</type>
      <name>VTEST_TEST</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a88b5d9557b0b1df1f6ab3766d477ef66</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tagVTEST_TEST_SUITE</type>
      <name>VTEST_TEST_SUITE</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>a24193db1a9fe627fe83d34121c6af101</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VFAIL</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>ac238eb93c68c155eca5ada030eaeaec9</anchor>
      <arglist>(const char *cond, const char *file, int line)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>vtestcui.h</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00009</filename>
    <member kind="typedef">
      <type>void(*</type>
      <name>PFN_DEBUG_FUNCTION</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>abd7685e0d1a16698e13f51bea60323a9</anchor>
      <arglist>)(const char *msg)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_CUI_test_runner</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>ab57fc055a968dc550bded8e2e0e2e98b</anchor>
      <arglist>(VTEST_TEST_SUITE *suite)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_CUI_test_runner_cmdline</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>afe34205fbd267e1174d1d405983d3179</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, int argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VTEST_CUI_set_debug_output_hook</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>acb688e29e69e58b8ebb173f167bd4667</anchor>
      <arglist>(PFN_DEBUG_FUNCTION debug_output_hook)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>vtestrunner.h</name>
    <path>/home/mmoser/archive/cstuff_bak/vtest/</path>
    <filename>a00010</filename>
    <class kind="struct">tagVTEST_RUNNER_IMPL</class>
    <member kind="typedef">
      <type>void(*</type>
      <name>VTEST_RUNNER_report_suite_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a723f694d8b96fcb8021ac0fb346eef76</anchor>
      <arglist>)(const char *suite_name)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>VTEST_RUNNER_report_test_start</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ad3a2da9e0d9fe0171c6f160ee9e6c2d4</anchor>
      <arglist>)(const char *suite_name, const char *test_name, int iteration, int maxiteration)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>VTEST_RUNNER_report_results</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a191dccb37a176ce012e4366367dc570d</anchor>
      <arglist>)(VTEST_STATUS status, const char *suite_name, const char *test_name, struct timeval *duration, const char *fail_cond, const char *fail_file, int fail_line)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>VTEST_RUNNER_report_wrapup</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>ab438f5d719eec3f97d51a0bc33416e3d</anchor>
      <arglist>)(int suitesinitfailed, int suitesteardownfailed, int tests_passed, int tests_failed, int testnotrun)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tagVTEST_RUNNER_IMPL</type>
      <name>VTEST_RUNNER_IMPL</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a0f3c206df647b82d4aee79bad3a978f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>VTEST_STATUS</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_TEST_FAILED</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1a1c90df150f8ebaf0848650618c66bec7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_TEST_OK</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1a707418bf7b2400b7282f16af32769069</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_SUITE_SETUP_FAILED</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1a41eb9054c64f8662bd8303e861a1c986</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_SUITE_SETUP_OK</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1a5e7abc933e42bb5da70c90612652a8cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_SUITE_TEARDOWN_FAILED</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1aead358466757d63f9ff64e91099729c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>VTEST_SUITE_TEARDOWN_OK</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>a8e1ed97b5fce3b7a10ab01d202fdc8c1ac1a892989da91fd3d362008c3cd12f69</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_test_runner</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>aa417fd7f65b276ab08060848510399b4</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>VTEST_test_runner_cmdline</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>aa917b1c203ce5d52b924f3369956df93</anchor>
      <arglist>(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl, int argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VTEST_goto_next_suite</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>aafd1fce2122fb39ccbc0afb612c59be3</anchor>
      <arglist>(const char *suite_name)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagVTEST_RUNNER_IMPL</name>
    <filename>a00002.html</filename>
    <member kind="variable">
      <type>VTEST_RUNNER_report_suite_start</type>
      <name>suite_start</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a84275152b2efc400e37d05a3e2cae229</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_RUNNER_report_test_start</type>
      <name>test_start</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a2d98ba63d74c6cfab28598a800985d46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_RUNNER_report_results</type>
      <name>results</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a2242678dd75ca03df88bb28f311ea17a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_RUNNER_report_wrapup</type>
      <name>wrapup</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a1363985c7632864c794d5366453becc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>current_test_state</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a85901f9cda6b2b7ce0a101cda31d9974</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_STATUS</type>
      <name>scope_fail</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a90804fc11deef7b08763e47e01a9e6d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_TEST_SUITE *</type>
      <name>suite</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>a82335d40bafedf6ed3df5bb2c2ca63a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_TEST *</type>
      <name>test</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ad9a7903a319ed14b62fb2de303e6f5b9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagVTEST_TEST</name>
    <filename>a00003.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>aedbd326bc39dd559d329cb4610da93d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_ACTION</type>
      <name>function</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>aab90c5c247a2c1db1b525927425466c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>repeat</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>ae67a002d03792b660e181c0d370e6ea3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tagVTEST_TEST_SUITE</name>
    <filename>a00004.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a606e0fc75c44a092aeb4ce79dc66da4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_ACTION</type>
      <name>setUp</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a73c6b953e3b1c5f39b34d02ae9b693ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_ACTION</type>
      <name>tearDown</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a4963995b80ce16dd826c2e9db46be15a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tagVTEST_TEST_SUITE *</type>
      <name>next_suite</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>a56e5e719928172615641f316d5144401</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VTEST_TEST *</type>
      <name>test_cases</name>
      <anchorfile>a00004.html</anchorfile>
      <anchor>ae6efa9c269cc192393aa2dfd3ce83d38</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
