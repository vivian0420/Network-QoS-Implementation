// Network topology
// //             n0   r    n1
// //             |    _    |
// //             ====|_|====
// //                router

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ppp-header.h"

#include <math.h>
#include <queue>
#include <string>
#include <vector>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("project2");

class FilterElement
{
  public:
    virtual bool match(Ptr<Packet> p) = 0;
};

template <typename Packet>
class SourceIpAddress : public FilterElement
{
  private:
    Ipv4Address value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class SourceMask : public FilterElement
{
  private:
    Ipv4Mask value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class SourcePortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class DestinationIpAddress : public FilterElement
{
  private:
    Ipv4Address value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class DestinationMask : public FilterElement
{
  private:
    Ipv4Mask value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class DestinationPortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

template <typename Packet>
class ProtocolNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool match(Ptr<Packet> p)
    {
    }
};

class Filter
{
  public:
    std::vector<FilterElement*> elements;
    bool match(Ptr<Packet> p);
};

class TrafficClass
{
  private:
    uint32_t* packets;
    uint32_t* maxPackets;
    uint32_t* quantum_size;
    uint32_t* deficit_counter;
    //uint32_t right_to_serve;
    // double_t weight;
    uint32_t priority_level;
    bool isDefault;
    std::queue<Ptr<Packet>> m_queue;
    std::vector<Filter*> filters;

  public:
    TrafficClass();
    bool Enqueue(Ptr<Packet> p);
    Ptr<Packet> Dequeue();
    bool match(Ptr<Packet> p);
    uint32_t getPackets();
    uint32_t getMaxPackets();
    void setMaxPackets(uint32_t m);
    void setPriorityLevel(uint32_t p);
    uint32_t getPriorityLevel();
    std::queue<Ptr<Packet>>* getMqueue();
    void setQuantumSize(uint32_t q);
    uint32_t getQuantumSize();
    void setDeficitCounter(uint32_t d);
    uint32_t getDeficitCounter();
};

template <typename Packet>
class DiffServ : public Queue<Packet>
{
  protected:
    std::vector<TrafficClass> q_class;
    bool DoEnqueue(Ptr<Packet> p);
    Ptr<Packet> DoDequeue();
    Ptr<Packet> DoRemove();
    Ptr<const Packet> DoPeek();

  public:
    Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet> p);
};

TrafficClass::TrafficClass()
{
    this->deficit_counter = new uint32_t;
    this->quantum_size = new uint32_t;
    this->packets = new uint32_t;
    this->maxPackets = new uint32_t;
}

uint32_t
TrafficClass::getPackets()
{
    return m_queue.size();
}

void
TrafficClass::setMaxPackets(uint32_t m)
{
    *maxPackets = m;
}

uint32_t
TrafficClass::getMaxPackets()
{
    return *maxPackets;
}

void
TrafficClass::setPriorityLevel(uint32_t p)
{
    priority_level = p;
}

uint32_t
TrafficClass::getPriorityLevel()
{
    return priority_level;
}

std::queue<Ptr<Packet>>*
TrafficClass::getMqueue()
{
    return &m_queue;
}

void
TrafficClass::setQuantumSize(uint32_t q)
{
    *quantum_size = q;
}

uint32_t
TrafficClass::getQuantumSize()
{
    return *quantum_size;
}

void
TrafficClass::setDeficitCounter(uint32_t d)
{
    *deficit_counter = d;
}

uint32_t
TrafficClass::getDeficitCounter()
{
    return *deficit_counter;
}

template <typename Packet>
class DRR : public DiffServ<Packet>
{
  private:
    std::queue<Ptr<Packet>> temp_queue;
  
  protected:
    std::vector<TrafficClass*> q_class;
    bool DoEnqueue(Ptr<Packet> packet);
    Ptr<Packet> DoDequeue();
    Ptr<Packet> DoRemove();
    Ptr<Packet> DoPeek();

  public:
    DRR();
    ~DRR();
    static TypeId GetTypeId();
    Ptr<Packet> Dequeue() override;
    bool Enqueue(Ptr<Packet> packet) override;
    Ptr<Packet> Remove() override;
    Ptr<const Packet> Peek() const override;
    Ptr<Packet> Schedule();
    bool Classify(Ptr<Packet> packet);
    std::queue<Ptr<Packet>>* getTempqueue();

};

template <typename Packet>
DRR<Packet>::~DRR()
{
}

template <typename Packet>
DRR<Packet>::DRR()
{
    TrafficClass* tc1 = new TrafficClass();
    tc1->setMaxPackets(1000000);   // todo: read from config file
    tc1->setQuantumSize(1200);
    tc1->setDeficitCounter(0);
    q_class.push_back(tc1);

    TrafficClass* tc2 = new TrafficClass();
    tc2->setMaxPackets(1000000);
    tc2->setQuantumSize(800);
    tc2->setDeficitCounter(0);
    q_class.push_back(tc2);

    TrafficClass* tc3 = new TrafficClass();
    tc3->setMaxPackets(1000000);
    tc3->setQuantumSize(400); // todo: read from config file
    tc3->setDeficitCounter(0);
    q_class.push_back(tc3);
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
    if (1 == 1) // todo: replace this condition to 'match'
    {
        Ptr<Packet> copy = packet->Copy();
        PppHeader pppHeader;
        Ipv4Header ipHeader;
        UdpHeader header;
        copy->RemoveHeader(pppHeader);
        copy->RemoveHeader(ipHeader);
        copy->RemoveHeader(header);
        copy->PeekHeader(header);

        if (header.GetSourcePort() == uint16_t(9999)) // todo: read data from config file
        {
            if (q_class[2]->getPackets() < q_class[2]->getMaxPackets()) // todo: Maybe the value returned by Classify() can
                                             // replace the index 0
            {
                q_class[2]->getMqueue()->push(packet);
                return true;
            }
        }
        else if (header.GetSourcePort() == uint16_t(8888)) // todo: read data from config file
        {
            if (q_class[1]->getPackets() < q_class[1]->getMaxPackets())
            {
                q_class[1]->getMqueue()->push(packet);
                return true;
            }
        }
        else 
        {
            if (q_class[0]->getPackets() < q_class[0]->getMaxPackets())
            {
                q_class[0]->getMqueue()->push(packet);
                return true;
            }
        }
        return false;
    }
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::DoDequeue()
{
    if (getTempqueue()->empty())
    {
        for (int i = 0; i < 3; i++)
        {      // read '3' from config file
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
    if (!getTempqueue()->empty())
    {
        Ptr<Packet> packet = getTempqueue()->front();
        getTempqueue()->pop();
        return packet;
    }
    else
    {
        return nullptr;
    }
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::DoRemove()
{
    if (q_class[0]->getPackets() == 0 && q_class[1]->getPackets() == 0) {
        NS_LOG_LOGIC("Queue empty");
        return nullptr;
    } 
    else if (q_class[0]->getPackets() != 0) {
        Ptr<Packet> packet = q_class[0]->getMqueue()->front();
        q_class[0]->getMqueue()->pop();
        return packet;
    } 
    else if (q_class[1]->getPackets() != 0) {
        Ptr<Packet> packet = q_class[1]->getMqueue()->front();
        q_class[1]->getMqueue()->pop();
        return packet;
    }  else {
        return nullptr;
    }
}

template <typename Packet>
Ptr<Packet>
DRR<Packet>::DoPeek()
{
    if (q_class[0]->getPackets() == 0 && q_class[1]->getPackets() == 0) {
        NS_LOG_LOGIC("Queue empty");
        return nullptr;
    } 
    else if (q_class[0]->getPackets() != 0) {
        return q_class[0]->m_queue.front();
    } 
    else if (q_class[1]->getPackets() != 0) {
        return q_class[1]->m_queue.front();
    }
}

class UdpApplication : public ns3::Application
{
  public:
    UdpApplication();
    virtual ~UdpApplication();
    static TypeId GetTypeId();
    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               uint32_t nPackets,
               DataRate dataRate,
               uint32_t portNumber);
    // void ReceivePacket (Ptr<Socket> socket);
  private:
    void StartApplication() override;
    void StopApplication() override;
    void ScheduleTx();
    void SendPacket();
    Ptr<Socket> m_socket;   //!< The transmission socket.
    Address m_peer;         //!< The destination address.
    uint32_t m_packetSize;  //!< The packet size.
    uint32_t m_nPackets;    //!< The number of packets to send.
    DataRate m_dataRate;    //!< The data rate to use.
    EventId m_sendEvent;    //!< Send event.
    bool m_running;         //!< True if the application is running.
    uint32_t m_packetsSent; //!< The number of packets sent.
    uint16_t m_portNumber;
};

UdpApplication::UdpApplication()
    : m_socket(nullptr),
      m_peer(),
      m_packetSize(0),
      m_nPackets(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0),
      m_portNumber(0)
{
}

UdpApplication::~UdpApplication()
{
    m_socket = nullptr;
}

TypeId
UdpApplication::GetTypeId()
{
    static TypeId tid = TypeId("UdpApplication")
                            .SetParent<Application>()
                            .SetGroupName("Udp")
                            .AddConstructor<UdpApplication>();
    return tid;
}

void
UdpApplication::Setup(Ptr<Socket> socket,
                      Address address,
                      uint32_t packetSize,
                      uint32_t nPackets,
                      DataRate dataRate,
                      uint32_t portNumber)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
    m_portNumber = portNumber;
}

void
UdpApplication::StartApplication()
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
UdpApplication::StopApplication()
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
UdpApplication::SendPacket()
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    UdpHeader header;
    header.SetSourcePort(m_portNumber);
    packet->AddHeader(header);
    m_socket->Send(packet);

    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx();
    }
}

void
UdpApplication::ScheduleTx()
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &UdpApplication::SendPacket, this);
    }
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
    clientApps1->Setup(socket1, address1, 1162, 5000, DataRate("10Mbps"), 6666);
    nodes.Get(0)->AddApplication(clientApps1);
    clientApps1->SetStartTime(Seconds(2.));
    clientApps1->SetStopTime(Seconds(10.));
    

    Ptr<UdpApplication> clientApps2 = CreateObject<UdpApplication>();
    clientApps2->Setup(socket2, address2, 1162, 5000, DataRate("10Mbps"), 8888);
    nodes.Get(0)->AddApplication(clientApps2);
    clientApps2->SetStartTime(Seconds(2.));
    clientApps2->SetStopTime(Seconds(10.));
    

    Ptr<UdpApplication> clientApps3 = CreateObject<UdpApplication>();
    clientApps3->Setup(socket3, address3, 1162, 5000, DataRate("10Mbps"), 9999);
    nodes.Get(0)->AddApplication(clientApps3);
    clientApps3->SetStartTime(Seconds(2.));
    clientApps3->SetStopTime(Seconds(10.));
    

    PointToPointHelper pointToPoint0;
    pointToPoint0.EnablePcapAll("udp");

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
