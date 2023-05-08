#include "ns3/log.h"
#include "ns3/ppp-header.h"
#include "protocol_number.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ProtocolNumber");


NS_OBJECT_ENSURE_REGISTERED (ProtocolNumber);

TypeId 
ProtocolNumber::GetTypeId(void)
{
    static TypeId tid = TypeId ("ns3::ProtocolNumber")
        .SetParent<Object> ()
        .SetGroupName ("ProtocolNumber");
    return tid;
}


ProtocolNumber::ProtocolNumber()
{
    NS_LOG_FUNCTION(this);
}

ProtocolNumber::~ProtocolNumber()
{
    NS_LOG_FUNCTION(this); 

}

ProtocolNumber::ProtocolNumber(uint32_t pro)
{
    protocol = pro;
}


bool
ProtocolNumber::match(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    Ptr<Packet> copy = packet->Copy();
    PppHeader pppHeader;
    Ipv4Header ipv4Header;
    copy->RemoveHeader(pppHeader);
    copy->PeekHeader(ipv4Header);

    uint8_t protocolNumber = ipv4Header.GetProtocol();

    std::cout << " ProtocolNumber.protocolNumber:" << protocolNumber << std::endl;
    std::cout << " ProtocolNumber.value:" << protocolNumber << std::endl;

    return protocolNumber == protocol;

    // if (protocolNumber == protocol){
    //     std::cout << " Matched protocolNumber:" << protocolNumber << std::endl;
    //     return true;
    // } else {
    //     std::cout << " Not Matched protocolNumber:" << protocolNumber << std::endl;
    //     return false;
    // }

}