source::MyICMPPingSource(SRC 10.0.0.1, DST 10.0.0.2, INCR_TTL false);
switch::ListenEtherSwitch

source
	-> EtherEncap(0x0800, 1A:7C:3E:90:78:41, 1A:7C:3E:90:78:42)
	-> [0] switch
	
switch[0]
	-> Discard
	
switch[1]
	-> ToDump(echotest.pcap)
	-> Discard
