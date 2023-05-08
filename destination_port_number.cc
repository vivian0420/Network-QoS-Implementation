#include "destination_port_number.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DestinationPortNumber");
NS_OBJECT_ENSURE_REGISTERED (DestinationPortNumber);

TypeId
DestinationPortNumber::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::DestinationPortNumber")
            .SetParent<FilterElement>()
            .SetGroupName ("trafficControl");
    return tid;
}

DestinationPortNumber::DestinationPortNumber(uint32_t port)
{
    port_number = port;
}


DestinationPortNumber::~DestinationPortNumber()
{
    NS_LOG_FUNCTION (this);
}

bool 
DestinationPortNumber::match(Ptr<Packet> p)
{
    NS_LOG_FUNCTION (this);
    Ptr<Packet> copyPacket = p->Copy();
    PppHeader pppHeader;
    Ipv4Header ipv4Header;
    UdpHeader udpHeader;

    copyPacket->RemoveHeader(pppHeader);
    copyPacket->RemoveHeader(ipv4Header);
    copyPacket->PeekHeader(udpHeader);
    uint32_t destinationPort = (uint32_t)udpHeader.GetDestinationPort();

    return destinationPort == port_number;
}