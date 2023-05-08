#include "ns3/log.h"
#include "destination_mask.h"
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"
#include "ns3/point-to-point-net-device.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DestinationMask");

NS_OBJECT_ENSURE_REGISTERED (DestinationMask);

TypeId
DestinationMask::GetTypeId (void)
{
    static TypeId tid =
      TypeId ("ns3::DestinationMask").SetParent<FilterElement> ().SetGroupName ("TrafficControl");
    return tid;
}

DestinationMask::DestinationMask()
{
    NS_LOG_FUNCTION (this);
}

DestinationMask::~DestinationMask()
{
    NS_LOG_FUNCTION (this);
}

DestinationMask::DestinationMask(Ipv4Mask ipv4Mask)
{
    NS_LOG_FUNCTION (this);
    this->dest_mask = ipv4Mask;
}

bool
DestinationMask::match(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION (this << packet);
    Ptr<Packet> copy = packet->Copy();
    PppHeader pppHeader;
    Ipv4Header ipv4Header;

    copy->RemoveHeader(pppHeader);
    copy->PeekHeader(ipv4Header);
    Ipv4Address ipv4Address = ipv4Header.GetDestination();
    
    if (ipv4Address == ipv4Address.CombineMask(this->dest_mask)) { 
        std::cout << "Mask:" << dest_mask << " Matched with Ipv4Address:" << ipv4Address << std::endl;
        return true;
    } else {
        std::cout << "Mask:" << dest_mask << " Not Matched Ipv4Address:" << ipv4Address << std::endl;
        return false;
    }
    return false;
}