#ifndef DIFFSERV_H
#define DIFFSERV_H
#include "TrafficClass.h"

#include "ns3/queue.h"

#include <vector>

using namespace ns3;

template <typename Packet>
class DiffServ : public Queue<Packet>
{
  protected:
    int in;
    int out;
    std::vector<TrafficClass*> q_class;
    bool DoEnqueue(Ptr<Packet> p);
    Ptr<Packet> DoDequeue();
    Ptr<Packet> DoRemove();
    Ptr<const Packet> DoPeek();

  public:
    DiffServ();
    DiffServ(uint32_t maxPackets,
             uint32_t priority_level,
             bool isDefault,
             Ipv4Address destIpAddr,
             Ipv4Mask destMask,
             uint32_t destPortNum,
             uint32_t protNum,
             Ipv4Address sourIpAddr,
             Ipv4Mask sourMask,
             uint32_t sourPortNum,
             Ipv4Address sourIp,
             Ipv4Address destIp);
    ~DiffServ();
    static TypeId GetTypeId(void);
    Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet> p);
};

#endif