TOPDIR=..


TARGETS=loop loop_udp udpsim captest
 


# ---------------------------------------

loop_TYPE=lib

loop_SRC=loop_lib.c

loop_CFLAGS=-Ilibpcap/inst/inc

# ---------------------------------------

udpsim_TYPE=exe

udpsim_SRC=sourcecheck.c udpsim.c tokparser.c subst.c loop_lib.c udpsim_blurb.c hex.c sleeep.c

udpsim_LIBS=c cutils pthread loop 

udpsim_CFLAGS=-Ilibpcap/inst/inc

udpsim_LIBS=pcap pthread cutils

# ---------------------------------------

loop_udp_TYPE=exe

loop_udp_SRC=loop_udp.c loop_lib.c loop_udp_blurb.c

loop_udp_LIBS=c cutils pthread loop

loop_udp_CFLAGS=-Ilibpcap/inst/inc

loop_udp_LIBS=pcap cutils

# ---------------------------------------

captest_TYPE=exe

captest_SRC=captest.c loop_lib.c captest_blurb.c

captest_LIBS=c cutils pthread loop

captest_CFLAGS=-Ilibpcap/inst/inc

captest_LIBS=pcap cutils

include $(TOPDIR)/rules.make

#---------------------------------------

udpsim_blurb.c : udpsim_blurb.txt
	perl make-blurb.pl $<

loop_udp_blurb.c : loop_udp_blurb.txt
	perl make-blurb.pl $<

captest_blurb.c : captest_blurb.txt
	perl make-blurb.pl $<


