#ifndef SPQ_H
#define SPQ_H

#include "TrafficClass.h"
#include "DiffServ.h"
using namespace ns3;

template <typename Packet>
class SPQ : public DiffServ<Packet>
{
    public:
        SPQ();
        ~SPQ();

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


#endif // SPQ_H