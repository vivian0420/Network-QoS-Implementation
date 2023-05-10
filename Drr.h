#include "TrafficClass.h"
#include "DiffServ.h"
#include "UdpApplication.h"
#include "util.h"

using namespace ns3;

template <typename Packet>
class DRR : public DiffServ<Packet>
{
  private:
    std::queue<Ptr<Packet>> temp_queue;
    uint32_t q_num;
  
  protected:
    std::vector<TrafficClass*> q_class;
    bool DoEnqueue(Ptr<Packet> packet);
    Ptr<Packet> DoDequeue();
    Ptr<Packet> DoRemove();
    Ptr<const Packet> DoPeek() const;

  public:
    DRR();
    DRR(std::vector<MyConfig> configs);
    ~DRR();
    static TypeId GetTypeId();
    Ptr<Packet> Dequeue() override;
    bool Enqueue(Ptr<Packet> packet) override;
    Ptr<Packet> Remove() override;
    Ptr<const Packet> Peek() const override;
    uint32_t Classify(Ptr<Packet> p);
    std::queue<Ptr<Packet>>* Schedule();
    std::queue<Ptr<Packet>>* getTempqueue();
};
