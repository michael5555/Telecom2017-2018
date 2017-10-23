source::AgentAdvertisementSource(SRC 192.168.2.254, DST 255.255.255.255, RA 192.168.0.2);
switch::ListenEtherSwitch

source
	-> EtherEncap(0x0800, D2:2D:C1:C7:45:F1, FF:FF:FF:FF:FF:FF)
	-> [0] switch
	
switch[0]
	-> Discard
	
switch[1]
	-> ToDump(test.pcap)
	-> Discard