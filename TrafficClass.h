#ifndef TRAFFICCLASS_H
#define TRAFFICCLASS_H
#include "Filter.h"

//#include "ns3/packet.h"

#include <cstdint>
#include <math.h>
#include <queue>
#include <vector>

using namespace ns3;

class TrafficClass
{
  private:
    uint32_t packets;
    uint32_t maxPackets;
    uint32_t priority_level;
    bool isDefault;
    std::queue<Ptr<Packet>> m_queue;
    std::vector<Filter*> filters;

  public:
    static TypeId GetTypeId(void);
    TrafficClass();
    TrafficClass(uint32_t maxPackets,
                 uint32_t priority_level,
                 bool isDefault,
                 Ipv4Address destIpAddr,
                 Ipv4Mask destMask,
                 uint32_t destPortNum,
                 uint32_t protNum,
                 Ipv4Address sourIpAddr,
                 Ipv4Mask sourMask,
                 uint32_t sourPortNum);
    ~TrafficClass();

    void SetPackets(uint32_t p);
    uint32_t GePackets();
    void SetMaxPackets(uint32_t mp);
    uint32_t GetMaxPackets();
    void SetPriorityLevel(double_t p);
    uint32_t GetPriorityLevel();
    void SetDefault(bool d);
    bool GetDefault();
    std::queue<Ptr<Packet>>* getMqueue();

    bool Enqueue(Ptr<Packet> p);
    Ptr<Packet> Dequeue();
    Ptr<Packet> Peek();
    bool IsEmpty();
    Ptr<Packet> Remove();
    bool match(Ptr<Packet> p);
    // uint32_t GetHeadPacketSize();
};

#endif