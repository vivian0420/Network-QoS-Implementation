#include "DiffServ.h"

#include <vector>

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("DiffServ");

// NS_OBJECT_TEMPLATE_CLASS_DEFINE(DiffServ, Packet);

template <typename Packet>
TypeId
DiffServ<Packet>::GetTypeId(void)
{
    static TypeId tid = TypeId("DiffServ<Packet>")
                            .SetParent<Queue<Packet>>()
                            .SetGroupName("Network QoS")
                            .template AddConstructor<DiffServ<Packet>>();
    return tid;
}

template <typename Packet>
DiffServ<Packet>::DiffServ()
    : Queue<Packet>()
{
    NS_LOG_FUNCTION(this);
    in = 0;
    out = 0;
}

template <typename Packet>
DiffServ<Packet>::DiffServ(uint32_t maxPackets,
                           uint32_t priority_level,
                           bool isDefault,
                           Ipv4Address destIpAddr,
                           Ipv4Mask destMask,
                           uint32_t destPortNum,
                           uint32_t protNum,
                           Ipv4Address sourIpAddr,
                           Ipv4Mask sourMask,
                           uint32_t sourPortNum)
    : Queue<Packet>()
{
    NS_LOG_FUNCTION(this);

    TrafficClass* tc = new TrafficClass(maxPackets,
                                        priority_level,
                                        isDefault,
                                        destIpAddr,
                                        destMask,
                                        destPortNum,
                                        protNum,
                                        sourIpAddr,
                                        sourMask,
                                        sourPortNum);
    q_class.push_back(tc);
    in = 0;
    out = 0;
}

template <typename Packet>
DiffServ<Packet>::~DiffServ()
{
    NS_LOG_FUNCTION(this);
}

template <typename Packet>
bool
DiffServ<Packet>::DoEnqueue(Ptr<Packet> p)
{
    NS_LOG_FUNCTION(this << p);
    bool res = Classify(p);
    in++;
    return res;
}

template <typename Packet>
Ptr<Packet>
DiffServ<Packet>::DoDequeue()
{
    NS_LOG_FUNCTION(this);
    if (in <= out)
    {
        return NULL;
    }
    Ptr<Packet> res = Schedule()->Dequeue();
    out++;
    return res;
}

template <typename Packet>
Ptr<Packet>
DiffServ<Packet>::DoRemove()
{
    NS_LOG_FUNCTION(this);
    if (in <= out)
    {
        return NULL;
    }
    Ptr<Packet> res = Schedule()->Dequeue();
    out++;
    return res;
}

template <typename Packet>
Ptr<const Packet>
DiffServ<Packet>::DoPeek()
{
    NS_LOG_FUNCTION(this);
    return q_class[out % q_class.size()].Peek();
}

template <typename Packet>
Ptr<Packet>
DiffServ<Packet>::Schedule()
{
    return q_class[out % q_class.size()].Dequeue();
}

template <typename Packet>
uint32_t
DiffServ<Packet>::Classify(Ptr<Packet> p)
{
   return q_class[in % q_class.size()].Enqueue(p);
}