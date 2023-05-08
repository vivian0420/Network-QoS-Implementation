#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/point-to-point-net-device.h"
#include "source_ip_address.h"
#include "ns3/ppp-header.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SourceIpAddress");

NS_OBJECT_ENSURE_REGISTERED (SourceIpAddress);

TypeId
SourceIpAddress::GetTypeId(void)
{
  static TypeId tid =
      TypeId("ns3::SourceIpAddress").SetParent<FilterElement>().SetGroupName ("SourceIpAddress");
  return tid;
}

SourceIpAddress::SourceIpAddress()
{
  NS_LOG_FUNCTION (this);
}

SourceIpAddress::SourceIpAddress(Ipv4Address ipV4Address)
{
  NS_LOG_FUNCTION (this);
  this->ipv4address = ipV4Address;
}

SourceIpAddress::~SourceIpAddress ()
{
  NS_LOG_FUNCTION (this);
}

bool SourceIpAddress::match (Ptr<Packet> packet){
    NS_LOG_FUNCTION(this << packet);
    Ptr<Packet> copy = packet->Copy();
    PppHeader ppph;
	Ipv4Header iph;
    
	copy->RemoveHeader(ppph);
	copy->PeekHeader(iph);
    
    std::cout<< "ipv4 address: " << ipv4address <<std::endl;

    Ipv4Address Ipv4Address = iph.GetSource();
    return Ipv4Address == this->ipv4address;
    // if(Ipv4Address == this->ipv4address){
    //     std::cout << " Matched Ipv4Address:" << Ipv4Address << std::endl;
    //     return true;
    // }  else{
    //     std::cout << " Not Matched Ipv4Address:" << Ipv4Address << std::endl;
    //     return false;
    // }
}