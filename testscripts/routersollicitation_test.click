source::RouterSollicitationSource(SRC 192.168.2.1, DST 255.255.255.255);
switch::ListenEtherSwitch

source
	-> EtherEncap(0x0800, 1E:83:42:6E:FE:67, FF:FF:FF:FF:FF:FF)
	-> [0] switch
	
switch[0]
	-> Discard
	
switch[1]
	-> ToDump(sollicitationtest.pcap)
	-> Discard