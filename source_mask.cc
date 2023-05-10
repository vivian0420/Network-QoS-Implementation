#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"
#include "ns3/point-to-point-net-device.h"
#include "source_mask.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SourceMask");
NS_OBJECT_ENSURE_REGISTERED (SourceMask);

TypeId 
SourceMask::GetTypeId(void)
{
    static TypeId tid =
    TypeId ("ns3::SourceMask").SetParent<FilterElement> ().SetGroupName ("trafficControl");
    return tid;
}


SourceMask::SourceMask()
{
    NS_LOG_FUNCTION(this);
}

SourceMask::~SourceMask()
{
    NS_LOG_FUNCTION(this); 
}

SourceMask::SourceMask(Ipv4Mask ipv4Mask, Ipv4Address source_ip_addr)
{ 
    NS_LOG_FUNCTION (this);  
    this->source_mask=ipv4Mask;
    this->source_ip_addr = source_ip_addr;
}


bool
SourceMask::match(Ptr<Packet> packet) 
{
    NS_LOG_FUNCTION (this << packet);    
    Ptr<Packet> copy = packet->Copy();
    PppHeader PppHeader;
    Ipv4Header ipv4Header;
    copy->RemoveHeader(PppHeader);
    copy->PeekHeader(ipv4Header);
    Ipv4Address ipv4Address = ipv4Header.GetSource();

    if (source_ip_addr == ipv4Address.CombineMask(this->source_mask)) {  
        std::cout<< "Mask:" << source_mask << " Matched with Ipv4Address:" << ipv4Address << std::endl;  
        return true;
     } else {
        std::cout<< "Mask:" << source_mask << " Not Matched Ipv4Address:" << ipv4Address << std::endl;
        return false;
    }
}