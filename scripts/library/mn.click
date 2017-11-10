// Mobile Node
// The input/output configuration is as follows:
//
// Packets for the network are put on output 0
// Packets for the host are put on output 1

// Input:
//	[0]: packets received from the network
//
// Output:
//	[0]: packets sent to the network
//	[1]: packets destined for the host itself


elementclass MobileNode {
	$address, $gateway, $home_agent |

	MNState :: MNInfoBase(MYADDRESS $address, HAPUBLIC $home_agent, HAPRIVATE $gateway);
	requestsource :: NodeRequestGenerator(MNBASE MNState);


	adverthandler :: MNAdvertisementHandler(MNBASE MNState,RGEN requestsource);
	adverthandler2 :: MNAdvertisementHandler(MNBASE MNState, RGEN requestsource);

	// Shared IP input path
	ip :: Strip(14)
		-> CheckIPHeader
		-> rt :: LinearIPLookup(
			$address:ip/32 0,
			$address:ipnet 1,
			0.0.0.0/0 $gateway 1,
			255.0.0.0/4 2)
		-> adverthandler
		-> [1]output;

	rt[1]	-> ipgw :: IPGWOptions($address)
		-> FixIPSrc($address)
		-> ttl :: DecIPTTL
		-> frag :: IPFragmenter(1500)
		-> arpq :: ARPQuerier($address)
		-> output;

	ipgw[1]	-> ICMPError($address, parameterproblem)
		-> output;

	ttl[1]	-> ICMPError($address, timeexceeded)
		-> output;

	frag[1]	-> ICMPError($address, unreachable, needfrag)
		-> output;

	// incoming packets
	input
		-> HostEtherFilter($address)
		-> in_cl :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800)
		-> arp_res :: ARPResponder($address)
		-> output;

	in_cl[1]
		-> [1]arpq;

	in_cl[2]
		-> ip;

	rt[2]
	    -> adverthandler2
		-> Discard

	sollicitationsource :: NodeSollicitationGenerator(MNBASE MNState)
	-> CheckIPHeader
	-> arpq

	requestsource
	-> CheckIPHeader
	-> CheckUDPHeader
	-> arpq
}
