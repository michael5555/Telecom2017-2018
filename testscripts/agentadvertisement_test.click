source::AgentAdvertisementSource(SRC 192.168.2.254, DST 255.255.255.255, RA 192.168.0.2);
switch::ListenEtherSwitch

source
	-> EtherEncap(0x0800, 1A:7C:3E:90:78:41, 1A:7C:3E:90:78:42)
	-> [0] switch
	
switch[0]
	-> Discard
	
switch[1]
	-> ToDump(test.pcap)
	-> Discard