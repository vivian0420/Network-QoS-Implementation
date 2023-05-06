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

#include <vector>
#include <queue>
#include <math.h>
#include <string>


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
        bool match(Ptr<Packet> p
        ) 
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
        //uint32_t bytes;
        uint32_t packets;
        uint32_t maxPackets;
        //u_int32_t maxBytes;
        //double_t weight;
        uint32_t priority_level;
        bool isDefault;
        std::queue<Ptr<Packet>> m_queue;
        std::vector<Filter*> filters;
    public:
        bool Enqueue(Ptr<Packet> p);
        Ptr<Packet> Dequeue(); 
        bool match(Ptr<Packet> p); 
        uint32_t getPackets();
        uint32_t getMaxPackets();
        void setMaxPackets(uint32_t m);
        void setPriorityLevel(uint32_t p);
        uint32_t getPriorityLevel();
        std::queue<Ptr<Packet>>* getMqueue();
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

uint32_t TrafficClass:: getPackets()
{
    return m_queue.size();
}

void TrafficClass:: setMaxPackets(uint32_t m)
{
    maxPackets = m;
}

uint32_t TrafficClass:: getMaxPackets()
{
    return maxPackets;
}

void TrafficClass:: setPriorityLevel(uint32_t p)
{
    priority_level = p;
}

uint32_t TrafficClass:: getPriorityLevel()
{
    return priority_level;
}

std::queue<Ptr<Packet>>* TrafficClass:: getMqueue()
{
    return &m_queue;
}

template <typename Packet>
class SPQ : public DiffServ<Packet> 
{
    protected:
        std::vector<TrafficClass*> q_class;
        bool DoEnqueue(Ptr<Packet> packet);
        Ptr<Packet> DoDequeue();
        Ptr<Packet> DoRemove();
        Ptr<Packet> DoPeek();
    public:
        SPQ();
        ~SPQ();  
        static TypeId GetTypeId();   
        Ptr<Packet> Dequeue() override;
        bool Enqueue(Ptr<Packet> packet) override;
        Ptr<Packet> Remove() override;
        Ptr<const Packet> Peek() const override;
        Ptr<Packet> Schedule();
        bool Classify(Ptr<Packet> packet);        
};

template <typename Packet>
SPQ<Packet>::~SPQ(){

}

template <typename Packet>
SPQ<Packet>::SPQ()
{
    TrafficClass* tc1 = new TrafficClass();
    tc1->setMaxPackets(1000000);             //todo: read data from config file
    tc1->setPriorityLevel(1);
    q_class.push_back(tc1);

    TrafficClass* tc2 = new TrafficClass();
    tc2->setMaxPackets(1000000);
    tc2->setPriorityLevel(2);
    q_class.push_back(tc2);
}

template <typename Packet>
TypeId
SPQ<Packet>::GetTypeId()
{
    static TypeId tid =
        TypeId("SPQ<Packet>")
            .SetParent<Queue<Packet>>()
            .SetGroupName("SPQ")
            .template AddConstructor<SPQ<Packet>>();       
    return tid;
}

template <typename Packet>
bool
SPQ<Packet>::Enqueue(Ptr<Packet> packet)
{
    NS_LOG_FUNCTION(this << packet);

    return DoEnqueue(packet);
}

template <typename Packet>
Ptr<Packet>
SPQ<Packet>::Dequeue()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> packet = DoDequeue();

    NS_LOG_LOGIC("Popped " << packet);

    return packet;
}

template <typename Packet>
Ptr<Packet>
SPQ<Packet>::Remove()
{
    NS_LOG_FUNCTION(this);

    Ptr<Packet> packet = DoRemove();

    NS_LOG_LOGIC("Removed " << packet);

    return packet;
}

template <typename Packet>
Ptr<const Packet>
SPQ<Packet>::Peek() const
{
    NS_LOG_FUNCTION(this);

    return nullptr;
}

template <typename Packet>
bool 
SPQ<Packet>::DoEnqueue(Ptr<Packet> packet)
{
    if(1==1) {          //todo: replace this condition to 'match'

        Ptr<Packet> copy = packet->Copy();
        PppHeader pppHeader;
        Ipv4Header ipHeader;
        UdpHeader header;
        copy->RemoveHeader(pppHeader);
        copy->RemoveHeader(ipHeader);
        copy->RemoveHeader(header);
        copy->PeekHeader(header);
        
        if(header.GetSourcePort() == uint16_t(9999)) {                     //todo: read data from config file
            if (q_class[0]->getPackets() < q_class[0]->getMaxPackets()) {
                q_class[0]->getMqueue()->push(packet);
                return true;
            } 
        } else if(header.GetSourcePort() == uint16_t(8888)) {
            if (q_class[1]->getPackets() < q_class[1]->getMaxPackets()) {   //todo: read data from config file
                q_class[1]->getMqueue()->push(packet);
                return true;
            } 
        } 
        return false;
    }       
}

template <typename Packet>
Ptr<Packet> 
SPQ<Packet>::DoDequeue()
{
    if (q_class[0]->getPackets() != 0) {
        std::queue<Ptr<Packet>>* queue = q_class[0]->getMqueue();
        Ptr<Packet> p = queue->front();
        q_class[0]->getMqueue()->pop();        
        return p;
    } 
    else if (!q_class[1]->getMqueue()->empty()) {
        std::queue<Ptr<Packet>>* queue = q_class[1]->getMqueue();
        Ptr<Packet> p = queue->front();
        q_class[1]->getMqueue()->pop();        
        return p;
    } else {
        return nullptr;
    }
}

template <typename Packet>
Ptr<Packet> 
SPQ<Packet>::DoRemove()
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
SPQ<Packet>::DoPeek()
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
        UdpApplication ();
        virtual ~UdpApplication ();
        static TypeId GetTypeId();
        void Setup(Ptr<Socket> socket,
                   Address address,
                   uint32_t packetSize,
                   uint32_t nPackets,
                   DataRate dataRate,
                   uint32_t portNumber);

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

    
    //LogComponentEnable("UdpApplication", LOG_LEVEL_INFO);
    //LogComponentEnable("UdpServerApplication", LOG_LEVEL_INFO);

    //NS_LOG_INFO("Create nodes.");
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
    Ptr<SPQ<Packet>> queueA = CreateObject<SPQ<Packet>>();
    devA->SetQueue(queueA);

    Ptr<PointToPointNetDevice> devB = CreateObject<PointToPointNetDevice>();
    devB->SetAttribute("DataRate", StringValue("10Mbps"));
    devB->SetAddress(Mac48Address::Allocate());
    n0r.Get(1)->AddDevice(devB);
    Ptr<SPQ<Packet>> queueB = CreateObject<SPQ<Packet>>();
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
    Ptr<SPQ<Packet>> queueC = CreateObject<SPQ<Packet>>();
    devC->SetQueue(queueC);

    Ptr<PointToPointNetDevice> devD = CreateObject<PointToPointNetDevice>();
    devD->SetAttribute("DataRate", StringValue("10Mbps"));
    devD->SetAddress(Mac48Address::Allocate());
    n1r.Get(1)->AddDevice(devD);
    Ptr<SPQ<Packet>> queueD = CreateObject<SPQ<Packet>>();
    devD->SetQueue(queueD);

    Ptr<PointToPointChannel> channel2 = CreateObject<PointToPointChannel>();
    devC->Attach(channel2);
    devD->Attach(channel2);
    d1r.Add(devC);
    d1r.Add(devD);


    //eNS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0r = ipv4.Assign(d0r);
    ipv4.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i1r = ipv4.Assign(d1r);


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    UdpServerHelper Server(9);

    ApplicationContainer serverApps = Server.Install(nodes.Get(2));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(20.0));
    
    Ptr<Socket> socket1 = Socket::CreateSocket(n0r.Get(0), UdpSocketFactory::GetTypeId());
    socket1->Bind();
    Address address1(InetSocketAddress(i1r.GetAddress(1), 9));

    Ptr<Socket> socket2 = Socket::CreateSocket(n0r.Get(0), UdpSocketFactory::GetTypeId());
    socket2->Bind();
    Address address2(InetSocketAddress(i1r.GetAddress(1), 9));

    Ptr<UdpApplication> clientApps1 = CreateObject<UdpApplication>() ;
    nodes.Get(0)->AddApplication(clientApps1);
    clientApps1->SetStartTime(Seconds(1.0));
    clientApps1->SetStopTime(Seconds(20.0));
    clientApps1->Setup(socket1, address1, 1000, 1000000, DataRate("10Mbps"), 8888);

    Ptr<UdpApplication> clientApps2 = CreateObject<UdpApplication>() ;
    nodes.Get(0)->AddApplication(clientApps2);
    clientApps2->SetStartTime(Seconds(10.0));
    clientApps2->SetStopTime(Seconds(20.0));
    clientApps2->Setup(socket2, address2, 1000, 1500000, DataRate("10Mbps"), 9999);

    PointToPointHelper pointToPoint0;
    pointToPoint0.EnablePcapAll("udp");


    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
