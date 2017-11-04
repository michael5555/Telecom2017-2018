source::RegistrationRequestSource(SRC 192.168.2.1, DST 192.168.3.254, HA 192.168.0.2,COA 192.168.0.3 );
switch::ListenEtherSwitch

source
	-> EtherEncap(0x0800, C6:06:4F:25:2C:52, 9E:A4:AA:A3:A0:93)
	-> [0] switch
	
switch[0]
	-> Discard
	
switch[1]
	-> ToDump(test.pcap)
	-> Discard