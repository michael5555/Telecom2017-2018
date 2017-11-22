// Home or Foreign Agent
// The input/output configuration is as follows:
//
// Input:
//	[0]: packets received on the private network
//	[1]: packets received on the public network
//
// Output:
//	[0]: packets sent to the private network
//	[1]: packets sent to the public network
//	[2]: packets destined for the router itself

elementclass Agent {
	$private_address, $public_address, $gateway |

	MAState :: MAInfoBase(PUBLIC $public_address, PRIVATE $private_address);
	replygen :: MAReplyGenerator(MABASE MAState)
	registrationhandler :: MARegistrationHandler(MABASE MAState,REPLYGEN replygen)
	echosorter :: MAICMPEchoSorter(MABASE MAState)

	icmpclass :: IPClassifier((icmp type echo or icmp type echo-reply) and (host $public_address),-);

	// Shared IP input path and routing table
	ip :: Strip(14)
		-> CheckIPHeader
		-> rt :: StaticIPLookup(
					$private_address:ip/32 0,
					$public_address:ip/32 0,
					$private_address:ipnet 1,
					$public_address:ipnet 2,
					0.0.0.0/0 $gateway 2,
					255.0.0.0/4 3);
	
	// ARP responses are copied to each ARPQuerier and the host.
	arpt :: Tee (2);
	
	// Input and output paths for interface 0
	input
		-> HostEtherFilter($private_address)
		-> private_class :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800)
		-> ARPResponder($private_address)
		-> output;

	private_arpq :: ARPQuerier($private_address)
		-> output;

	private_class[1]
		-> arpt
		-> [1]private_arpq;

	private_class[2]
		-> Paint(1)
		-> ip;

	// Input and output paths for interface 1
	input[1]
		-> HostEtherFilter($public_address)
		-> public_class :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800)
		-> ARPResponder($public_address)
		-> [1]output;

	public_arpq :: ARPQuerier($public_address)
		-> [1]output;

	public_class[1]
		-> arpt[1]
		-> [1]public_arpq;

	public_class[2]
		-> Paint(2)
		-> ip;

	// Local delivery
	rt[0]
		-> registrationhandler
		-> [2]output
	
	// Forwarding paths per interface
	rt[1]
		-> echosorter
		-> DropBroadcasts
		-> private_paint :: PaintTee(1)
		-> private_ipgw :: IPGWOptions($private_address)
		-> FixIPSrc($private_address)
		-> private_ttl :: DecIPTTL
		-> private_frag :: IPFragmenter(1500)
		-> private_arpq;
	
	private_paint[1]
		-> ICMPError($private_address, redirect, host)
		-> rt;

	private_ipgw[1]
		-> ICMPError($private_address, parameterproblem)
		-> rt;

	private_ttl[1]
		-> ICMPError($private_address, timeexceeded)
		-> rt;

	private_frag[1]
		-> ICMPError($private_address, unreachable, needfrag)
		-> rt;
	

	rt[2]
		-> icmpclass;

	icmpclass
		-> Print(LABEL "ICMP ECHOES HERE", 0)
		-> StripIPHeader
		-> CheckIPHeader
		-> private_arpq;


	icmpclass[1]
		-> Print(LABEL "OTHER IP THERE", 0)
		-> DropBroadcasts
		-> public_paint :: PaintTee(2)
		-> public_ipgw :: IPGWOptions($public_address)
		-> FixIPSrc($public_address)
		-> public_ttl :: DecIPTTL
		-> public_frag :: IPFragmenter(1500)
		-> public_arpq;
	
	public_paint[1]
		-> ICMPError($public_address, redirect, host)
		-> rt;

	public_ipgw[1]
		-> ICMPError($public_address, parameterproblem)
		-> rt;

	public_ttl[1]
		-> ICMPError($public_address, timeexceeded)
		-> rt;

	public_frag[1]
		-> ICMPError($public_address, unreachable, needfrag)
		-> rt;

	rt[3]
	-> sollicitationhandler :: MASollicitationHandler(MABASE MAState, AAGEN advertisementsource)

	advertisementsource :: AgentAdvertisementGenerator(MABASE MAState)
	-> CheckIPHeader
	-> private_arpq

	registrationhandler[1]
	-> SetIPChecksum
	-> CheckIPHeader
	-> SetUDPChecksum
	-> public_arpq

	replygen
	-> CheckIPHeader
	-> SetUDPChecksum
	-> public_arpq

	replygen[1]
	-> CheckIPHeader
	-> SetUDPChecksum
	-> private_arpq

	registrationhandler[2]
	-> SetIPChecksum
	-> CheckIPHeader
	-> SetUDPChecksum
	-> private_arpq

	echosorter[1]
	-> IPEncap(PROTO ipip, SRC 192.168.0.2, DST 192.168.0.3)
	-> DropBroadcasts
	-> FixIPSrc($public_address)
	-> DecIPTTL
	-> public_arpq

}
