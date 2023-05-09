#include "TrafficClass.h"
#include "DiffServ.h"
#include "UdpApplication.h"
#include "util.h"
#include <vector>

using namespace ns3;

template <typename Packet>
class SPQ : public DiffServ<Packet>
{
    public:
        SPQ<Packet>();
        SPQ(std::vector<MyConfig> configs);
        ~SPQ<Packet>();

        static TypeId GetTypeId(void);
    
        bool Enqueue(Ptr<Packet> item) override;
        Ptr<Packet> Dequeue() override;
        Ptr<Packet> Remove() override;
        Ptr<const Packet> Peek(void) const override;
        uint32_t Classify(Ptr<Packet> p);
        std::queue<Ptr<Packet>>* Schedule();

    protected:
        std::vector<TrafficClass*> q_class;
        bool DoEnqueue(Ptr<Packet> p);
        Ptr<Packet> DoDequeue();
        Ptr<const Packet> DoPeek() const;
        Ptr<Packet> DoRemove();
    
    private:
        uint32_t q_num;
};