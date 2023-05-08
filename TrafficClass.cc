#include "TrafficClass.h"
#include "Filter.h"
#include "destination_ip_address.h"
#include "ns3/log.h"
#include "ns3/packet.h"

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TrafficClass");

TypeId
TrafficClass::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::TrafficClass").SetParent<Object>().SetGroupName("Network QoS");
    return tid;
}

TrafficClass::TrafficClass(uint32_t maxPackets,
                           uint32_t priority_level,
                           bool isDefault,
                           Ipv4Address destIpAddr,
                           Ipv4Mask destMask,
                           uint32_t destPortNum,
                           uint32_t protNum,
                           Ipv4Address sourIpAddr,
                           Ipv4Mask sourMask,
                           uint32_t sourPortNum)
{
    this->maxPackets = maxPackets;
    this->priority_level = priority_level;
    this->isDefault = isDefault;
    // this->filters = filters;
    DestinationIpAddress* dest_ip_addr = new DestinationIpAddress(destIpAddr);
    DestinationMask* dest_mask = new DestinationMask(destMask);
    DestinationPortNumber* dest_port_number = new DestinationPortNumber(destPortNum);
    ProtocolNumber* porto_number = new ProtocolNumber(protNum);
    SourceIpAddress* sour_ip_addr = new SourceIpAddress(sourIpAddr);
    SourceMask* sour_mask = new SourceMask(sourMask);
    SourcePortNumber* sour_port_number = new SourcePortNumber(sourPortNum);
    Filter* filter =
        new Filter(dest_ip_addr, dest_mask, dest_port_number, porto_number, sour_ip_addr, sour_mask, sour_port_number);
    std::vector<Filter*> filters;
    filters.push_back(filter);
    this->filters = filters;

    NS_LOG_FUNCTION(this);
}

TrafficClass::~TrafficClass()
{
    NS_LOG_FUNCTION(this);
}

void
TrafficClass::SetPackets(uint32_t p) // todo
{
    packets = p;
}

uint32_t
TrafficClass::GePackets() // todo
{
    return m_queue.size();
}

void
TrafficClass::SetMaxPackets(uint32_t mp)
{
    maxPackets = mp;
}

uint32_t
TrafficClass::GetMaxPackets()
{
    return maxPackets;
}

void
TrafficClass::SetPriorityLevel(double_t p)
{
    priority_level = p;
}

uint32_t
TrafficClass::GetPriorityLevel()
{
    return priority_level;
}

void
TrafficClass::SetDefault(bool d)
{
    isDefault = d;
}

bool
TrafficClass::GetDefault()
{
    return isDefault;
}

std::queue<Ptr<Packet>>* 
TrafficClass:: getMqueue()
{
    return &m_queue;
}

bool
TrafficClass::Enqueue(Ptr<Packet> p)
{
    if (match(p) && m_queue.size() < maxPackets)
    {
        m_queue.push(p);
        packets++; // todo
        return true;
    }
    return false;
}

Ptr<Packet>
TrafficClass::Dequeue()
{
    NS_LOG_FUNCTION(this);
    if (m_queue.empty())
    {
        NS_LOG_LOGIC("Queue empty");
        return nullptr;
    }
    Ptr<Packet> res = m_queue.front();
    m_queue.pop();
    packets--; // todo
    return res;
}

bool
TrafficClass::match(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    for (Filter* filter : filters)
    {
        if (filter->match(packet))
        {
            return true;
        }
    }
    return false;
}

bool
TrafficClass::IsEmpty()
{
    NS_LOG_FUNCTION(this);
    return m_queue.empty();
}

Ptr<Packet>
TrafficClass::Remove()
{
    NS_LOG_FUNCTION(this);

    if (!m_queue.empty())
    {
        Ptr<Packet> packet = m_queue.front();
        m_queue.pop();
        return packet;
    }
    return nullptr;
}

Ptr<Packet>
TrafficClass::Peek()
{
    NS_LOG_FUNCTION(this);

    if (m_queue.empty())
    {
        NS_LOG_LOGIC("Queue empty");
        return nullptr;
    }

    Ptr<Packet> p = m_queue.front();
    return p;
}

// uint32_t
// TrafficClass::GetHeadPacketSize()
// {
//     Ptr<Packet> packet = m_queue.front();
//     return packet->GetSize();
// }