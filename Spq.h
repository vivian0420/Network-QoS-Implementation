#include "TrafficClass.h"
#include "diffserv.h"
#include "UdpApplication.h"
using namespace ns3;

template <typename Packet>
class SPQ : public DiffServ<Packet>
{
    public:
        SPQ<Packet>();
        ~SPQ<Packet>();

        static TypeId GetTypeId(void);
    
        bool Enqueue(Ptr<Packet> item) override;
        Ptr<Packet> Dequeue() override;
        Ptr<Packet> Remove() override;
        Ptr<const Packet> Peek(void) const override;
        uint32_t Classify(Ptr<Packet> p);
        TrafficClass* Schedule();

    protected:
        std::vector<TrafficClass*> q_class;
        bool DoEnqueue(Ptr<Packet> p);
        Ptr<Packet> DoDequeue();
        Ptr<const Packet> DoPeek() const;
        Ptr<Packet> DoRemove();
};