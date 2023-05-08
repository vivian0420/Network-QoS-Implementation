#include "ns3/log.h"
#include "ns3/ppp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/point-to-point-net-device.h"
#include "destination_ip_address.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DestinationIpAddress");

NS_OBJECT_ENSURE_REGISTERED (DestinationIpAddress);

TypeId
DestinationIpAddress::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DestinationIpAddress")
    .SetParent<FilterElement>()
    .SetGroupName("trafficControl");
  return tid;
}

DestinationIpAddress::DestinationIpAddress()
{
  NS_LOG_FUNCTION(this);
}

DestinationIpAddress::DestinationIpAddress(Ipv4Address ipV4Address)
{
  NS_LOG_FUNCTION(this);
  this->addr = ipV4Address;
}

DestinationIpAddress::~DestinationIpAddress()
{
  NS_LOG_FUNCTION(this);
}

bool
DestinationIpAddress::match(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION (this << packet);

    Ptr<Packet> copy = packet->Copy();
    PppHeader pppHeader;
	Ipv4Header ipv4Header;
    
	copy->RemoveHeader(pppHeader);
    copy->PeekHeader(ipv4Header);

    Ipv4Address dest_addr = ipv4Header.GetDestination();

    return dest_addr == addr;

    // if (dest_addr == addr) {
    //     std::cout << " Matched Ipv4Address:" << dest_addr << std::endl;
    //     return true;
    // } else {
    //     std::cout << " Not Matched Ipv4Address:" << dest_addr << std::endl;
    //     return false;
    // }
    // return false;
}