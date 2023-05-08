#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ppp-header.h"
#include "Drr.h"

#include <math.h>
#include <queue>
#include <string>
#include <vector>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Drr");

template <typename Packet>
TypeId
DiffServ<Packet>::GetTypeId()
{
    static TypeId tid = TypeId("DiffServ")
                            .SetParent<Queue<Packet>>()
                            .SetGroupName("DiffServ")
                            .template AddConstructor<DiffServ<Packet>>();
    return tid;
}

template <typename Packet>
DiffServ<Packet>::DiffServ()
    // : Queue<Packet>()
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
    // : Queue<Packet>()
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

template <typename Packet>
DRR<Packet>::DRR()
{
    NS_LOG_FUNCTION (this);
    // todo: read parameters from config file
    TrafficClass* tc1 = new TrafficClass(100000,
                                         1200,
                                         0,
                                         false,
                                         "10.1.2.2",
                                         "255.255.255.3",
                                         9,
                                         17,
                                         "10.1.1.1",
                                         "255.255.255.3",
                                         6666);
    q_class.push_back(tc1);

    TrafficClass* tc2 = new TrafficClass(100000,
                                         800,
                                         0,
                                         false,
                                         "10.1.2.2",
                                         "255.255.255.3",
                                         9,
                                         17,
                                         "10.1.1.1",
                                         "255.255.255.3",
                                         8888);
    q_class.push_back(tc2);

    TrafficClass* tc3 = new TrafficClass(100000,
                                         400,
                                         0,
                                         true,
                                         "10.1.2.2",
                                         "255.255.255.3",
                                         9,
                                         17,
                                         "10.1.1.1",
                                         "255.255.255.3",
                                         9999);
    q_class.push_back(tc3);
}

template <typename Packet>
DRR<Packet>::~DRR()
{
    NS_LOG_FUNCTION(this);
}


template <typename Packet>
TypeId
DRR<Packet>::GetTypeId()
{
    static TypeId tid = TypeId("DRR<Packet>")
                            .SetParent<Queue<Packet>>()
                            .SetGroupName("DRR")
                            .template AddConstructor<DRR<Packet>>();
    return tid;
}

template <typename Packet>
std::queue<Ptr<Packet>>*
DRR<Packet>::getTempqueue()
{
    return &temp_queue;
}

template <typename Packet>
bool
DRR<Packet>::Enqueue(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    return DoEnqueue(packet);
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::Dequeue()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> packet = DoDequeue();

    NS_LOG_LOGIC("Popped " << packet);

    return packet;
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::Remove()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> packet = DoRemove();

    NS_LOG_LOGIC("Removed " << packet);

    return packet;
}

template <typename Packet>
Ptr<const Packet>
DRR<Packet>::Peek() const
{
    NS_LOG_FUNCTION(this);

    return nullptr;
}

template <typename Packet>
bool
DRR<Packet>::DoEnqueue(Ptr<Packet> packet)
{
    uint32_t index_of_qclass = Classify(packet);
    if (index_of_qclass >= 0 && index_of_qclass < 3)        //todo: read 3 from config file
    {
        if (q_class[index_of_qclass]->GetPackets() < q_class[index_of_qclass]->GetMaxPackets())
        {
            q_class[index_of_qclass]->getMqueue()->push(packet);
            return true;
        }
    }
    else
    {
        for (TrafficClass* tc : q_class)
        {
            if (tc->GetDefault() == true)
            {
                if (tc->GetPackets() < tc->GetMaxPackets())
                {
                    tc->getMqueue()->push(packet);
                    return true;
                }
            }
        }
    }
    return false;
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::DoDequeue()
{
    std::queue<Ptr<Packet>>* tempQueue = Schedule();
    if(!tempQueue->empty()) 
    {
        Ptr<Packet>packet = tempQueue->front();
        tempQueue->pop();
        return packet;
    }
    return nullptr;
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::DoRemove()
{
    std::queue<Ptr<Packet>>* tempQueue = Schedule();
    if(!tempQueue->empty()) 
    {
        Ptr<Packet>packet = tempQueue->front();
        tempQueue->pop();
        return packet;
    }
    return nullptr;
}

template <typename Packet>
Ptr<const Packet>
DRR<Packet>::DoPeek() const
{
    if (getTempqueue()->empty())
    {
        for (int i = 0; i < 3; i++)                // todo: read '3' from config file
        {                                           
            if (q_class[i]->getPackets() != 0)
            {
                q_class[i]->setDeficitCounter(q_class[i]->getDeficitCounter() +
                                              q_class[i]->getQuantumSize());
                Ptr<Packet> packet = q_class[i]->getMqueue()->front();
                uint32_t sizeOfPacket = packet->GetSize();
                if (q_class[i]->getDeficitCounter() >= sizeOfPacket)
                {
                    q_class[i]->setDeficitCounter(q_class[i]->getDeficitCounter() - sizeOfPacket);
                    q_class[i]->getMqueue()->pop();
                    getTempqueue()->push(packet);
                }
            }
        }
    }
    return getTempqueue()->front();
}

template <typename Packet>
uint32_t
DRR<Packet>::Classify(Ptr<Packet> p)
{
    for (int i = 0; i < 2; i++)
    {                                         // todo: read '2' from config
        if (q_class[i]->match(p))
        {
            return i;
        }
    }
    return -1;
}

template <typename Packet>
std::queue<Ptr<Packet>>*
DRR<Packet>::Schedule()
{
    if (getTempqueue()->empty())
    {
        for (int i = 0; i < 3; i++)                // todo: read '3' from config file
        {                                           
            if (q_class[i]->GetPackets() != 0)
            {
                q_class[i]->setDeficitCounter(q_class[i]->getDeficitCounter() +
                                              q_class[i]->getQuantumSize());
                Ptr<Packet> packet = q_class[i]->getMqueue()->front();
                uint32_t sizeOfPacket = packet->GetSize();
                if (q_class[i]->getDeficitCounter() >= sizeOfPacket)
                {
                    q_class[i]->setDeficitCounter(q_class[i]->getDeficitCounter() - sizeOfPacket);
                    q_class[i]->getMqueue()->pop();
                    getTempqueue()->push(packet);
                }
            }
        }
    }
    return getTempqueue();
}

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    // LogComponentEnable("UdpApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("UdpServerApplication", LOG_LEVEL_INFO);

    // NS_LOG_INFO("Create nodes.");
    NodeContainer nodes;
    nodes.Create(3);

    ns3::NodeContainer n0r = NodeContainer(nodes.Get(0), nodes.Get(1));
    ns3::NodeContainer n1r = NodeContainer(nodes.Get(1), nodes.Get(2));

    InternetStackHelper internet;
    internet.Install(nodes);

    NetDeviceContainer d0r;
    Ptr<PointToPointNetDevice> devA = CreateObject<PointToPointNetDevice>();
    devA->SetAttribute("DataRate", StringValue("10Mbps"));
    devA->SetAddress(Mac48Address::Allocate());
    n0r.Get(0)->AddDevice(devA);
    Ptr<DRR<Packet>> queueA = CreateObject<DRR<Packet>>();
    devA->SetQueue(queueA);

    Ptr<PointToPointNetDevice> devB = CreateObject<PointToPointNetDevice>();
    devB->SetAttribute("DataRate", StringValue("10Mbps"));
    devB->SetAddress(Mac48Address::Allocate());
    n0r.Get(1)->AddDevice(devB);
    Ptr<DRR<Packet>> queueB = CreateObject<DRR<Packet>>();
    devB->SetQueue(queueB);

    Ptr<PointToPointChannel> channel1 = CreateObject<PointToPointChannel>();
    devA->Attach(channel1);
    devB->Attach(channel1);
    d0r.Add(devA);
    d0r.Add(devB);

    NetDeviceContainer d1r;
    Ptr<PointToPointNetDevice> devC = CreateObject<PointToPointNetDevice>();
    devC->SetAttribute("DataRate", StringValue("10Mbps"));
    devC->SetAddress(Mac48Address::Allocate());
    n1r.Get(0)->AddDevice(devC);
    Ptr<DRR<Packet>> queueC = CreateObject<DRR<Packet>>();
    devC->SetQueue(queueC);

    Ptr<PointToPointNetDevice> devD = CreateObject<PointToPointNetDevice>();
    devD->SetAttribute("DataRate", StringValue("10Mbps"));
    devD->SetAddress(Mac48Address::Allocate());
    n1r.Get(1)->AddDevice(devD);
    Ptr<DRR<Packet>> queueD = CreateObject<DRR<Packet>>();
    devD->SetQueue(queueD);

    Ptr<PointToPointChannel> channel2 = CreateObject<PointToPointChannel>();
    devC->Attach(channel2);
    devD->Attach(channel2);
    d1r.Add(devC);
    d1r.Add(devD);

    // eNS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0r = ipv4.Assign(d0r);
    ipv4.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i1r = ipv4.Assign(d1r);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    UdpServerHelper Server(9);

    ApplicationContainer serverApps = Server.Install(nodes.Get(2));
    serverApps.Start(Seconds(1.));
    serverApps.Stop(Seconds(11.));

    Ptr<Socket> socket1 = Socket::CreateSocket(n0r.Get(0), UdpSocketFactory::GetTypeId());
    socket1->Bind();
    Address address1(InetSocketAddress(i1r.GetAddress(1), 9));

    Ptr<Socket> socket2 = Socket::CreateSocket(n0r.Get(0), UdpSocketFactory::GetTypeId());
    socket2->Bind();
    Address address2(InetSocketAddress(i1r.GetAddress(1), 9));

    Ptr<Socket> socket3 = Socket::CreateSocket(n0r.Get(0), UdpSocketFactory::GetTypeId());
    socket3->Bind();
    Address address3(InetSocketAddress(i1r.GetAddress(1), 9));

    Ptr<UdpApplication> clientApps1 = CreateObject<UdpApplication>();
    clientApps1->Setup(socket1, address1, 1162, 6000, DataRate("10Mbps"), 6666);
    nodes.Get(0)->AddApplication(clientApps1);
    clientApps1->SetStartTime(Seconds(2.));
    clientApps1->SetStopTime(Seconds(10.));
    

    Ptr<UdpApplication> clientApps2 = CreateObject<UdpApplication>();
    clientApps2->Setup(socket2, address2, 1162, 6000, DataRate("10Mbps"), 8888);
    nodes.Get(0)->AddApplication(clientApps2);
    clientApps2->SetStartTime(Seconds(2.));
    clientApps2->SetStopTime(Seconds(10.));
    

    Ptr<UdpApplication> clientApps3 = CreateObject<UdpApplication>();
    clientApps3->Setup(socket3, address3, 1162, 6000, DataRate("10Mbps"), 9999);
    nodes.Get(0)->AddApplication(clientApps3);
    clientApps3->SetStartTime(Seconds(2.));
    clientApps3->SetStopTime(Seconds(10.));
    

    PointToPointHelper pointToPoint0;
    pointToPoint0.EnablePcapAll("udp");

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
