#include "source_port_number.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SourcePortNumber");
NS_OBJECT_ENSURE_REGISTERED (SourcePortNumber);

TypeId
SourcePortNumber::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::SourcePortNumber")
            .SetParent<FilterElement> ()
            .SetGroupName ("trafficControl");
    return tid;
}

SourcePortNumber::SourcePortNumber(uint32_t port)
{
    src_port = port;
    std::cout << " Constructor.SourcePortNumber.value:" << src_port << std::endl;

}

SourcePortNumber::~SourcePortNumber()
{
    NS_LOG_FUNCTION (this);
}

bool 
SourcePortNumber::match(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION (this);
    Ptr<Packet> copyPacket = packet->Copy();
    PppHeader pppHeader;
    Ipv4Header ipv4Header;
    UdpHeader udpHeader;

    copyPacket -> RemoveHeader(pppHeader);
    copyPacket -> RemoveHeader(ipv4Header);
    copyPacket -> RemoveHeader(udpHeader);
    copyPacket -> PeekHeader(udpHeader);

    uint32_t sourcePort = udpHeader.GetSourcePort();       
                            
    return sourcePort == src_port;
}