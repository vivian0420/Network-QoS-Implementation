#include "Spq.h"
#include "TrafficClass.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ppp-header.h"
#include "UdpApplication.h"

#include <math.h>
#include <queue>
#include <string>
#include <vector>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Spq");

template <typename Packet>
SPQ<Packet>::SPQ()
{
    // todo: read parameters from config file
    TrafficClass* tc_high = new TrafficClass(100000,
                                         2,
                                         false,
                                         "10.1.2.0",
                                         "255.255.255.0",
                                         9,
                                         17,
                                         "10.1.1.0",
                                         "255.255.255.0",
                                         9999);
    q_class.push_back(tc_high);

    TrafficClass* tc_low = new TrafficClass(100000,
                                         1,
                                         true,
                                         "10.1.2.0",
                                         "255.255.255.0",
                                         9,
                                         17,
                                         "10.1.1.0",
                                         "255.255.255.0",
                                         8888);
    q_class.push_back(tc_low);
}

template <typename Packet>
SPQ<Packet>::~SPQ()
{
    NS_LOG_FUNCTION (this);
}

template <typename Packet>
TypeId
SPQ<Packet>::GetTypeId(void)
{
    static TypeId tid = TypeId("SPQ<Packet>")
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

    return DoPeek();
}

template <typename Packet>
bool
SPQ<Packet>::DoEnqueue(Ptr<Packet> packet)
{
    uint32_t index_of_qclass = Classify(packet);
    if (index_of_qclass >= 0)
    {
        Ptr<Packet> copy = packet->Copy();
        PppHeader pppHeader;
        Ipv4Header ipHeader;
        UdpHeader header;
        copy->RemoveHeader(pppHeader);
        copy->RemoveHeader(ipHeader);
        copy->RemoveHeader(header);
        copy->PeekHeader(header);

        if (q_class[index_of_qclass]->GePackets() < q_class[index_of_qclass]->GetMaxPackets())
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
                if (tc->GePackets() < tc->GetMaxPackets())
                {
                    tc->getMqueue()->push(packet);
                }
            }
        }
    }
    return false;
}

template <typename Packet>
Ptr<Packet>
SPQ<Packet>::DoDequeue()
{
    TrafficClass* tc = Schedule();
    if(tc != nullptr) {
        Ptr<Packet> packet = tc->getMqueue()->front();
        tc->getMqueue()->pop();
        return packet;
    } 
    return nullptr;
}

template <typename Packet>
Ptr<Packet>
SPQ<Packet>::DoRemove()
{
    TrafficClass* tc = Schedule();
    if(tc != nullptr) {
        Ptr<Packet> packet = tc->getMqueue()->front();
        tc->getMqueue()->pop();
        return packet;
    } 
    return nullptr;
}

template <typename Packet>
Ptr<const Packet>
SPQ<Packet>::DoPeek() const
{
    for(int i = 0; i < 2; i++) {          //todo: read '2' from config file
        if(!q_class[i]->IsEmpty()) {
            return q_class[i]->getMqueue()->front();
        }
    }
    return nullptr;
}

template <typename Packet>
uint32_t
SPQ<Packet>::Classify(Ptr<Packet> p)
{
    for (int i = 0; i < 2; i++)
    { // todo: read '2' from config
        if (q_class[i]->match(p))
        {
            return i;
        }
    }
    return -1;
}

template <typename Packet>
TrafficClass*
SPQ<Packet>::Schedule()
{
    for(int i = 0; i < 2; i++) {          //todo: read '2' from config file
        if(!q_class[i]->IsEmpty()) {
            return q_class[i];
        }
    }
    return nullptr;
}

// class UdpApplication : public ns3::Application
// {
//   public:
//     UdpApplication();
//     virtual ~UdpApplication();
//     static TypeId GetTypeId();
//     void Setup(Ptr<Socket> socket,
//                Address address,
//                uint32_t packetSize,
//                uint32_t nPackets,
//                DataRate dataRate,
//                uint32_t portNumber);

//   private:
//     void StartApplication() override;
//     void StopApplication() override;
//     void ScheduleTx();
//     void SendPacket();
//     Ptr<Socket> m_socket;   //!< The transmission socket.
//     Address m_peer;         //!< The destination address.
//     uint32_t m_packetSize;  //!< The packet size.
//     uint32_t m_nPackets;    //!< The number of packets to send.
//     DataRate m_dataRate;    //!< The data rate to use.
//     EventId m_sendEvent;    //!< Send event.
//     bool m_running;         //!< True if the application is running.
//     uint32_t m_packetsSent; //!< The number of packets sent.
//     uint16_t m_portNumber;
// };

// UdpApplication::UdpApplication()
//     : m_socket(nullptr),
//       m_peer(),
//       m_packetSize(0),
//       m_nPackets(0),
//       m_dataRate(0),
//       m_sendEvent(),
//       m_running(false),
//       m_packetsSent(0),
//       m_portNumber(0)
// {
// }

// UdpApplication::~UdpApplication()
// {
//     m_socket = nullptr;
// }

// TypeId
// UdpApplication::GetTypeId()
// {
//     static TypeId tid = TypeId("UdpApplication")
//                             .SetParent<Application>()
//                             .SetGroupName("Udp")
//                             .AddConstructor<UdpApplication>();
//     return tid;
// }

// void
// UdpApplication::Setup(Ptr<Socket> socket,
//                       Address address,
//                       uint32_t packetSize,
//                       uint32_t nPackets,
//                       DataRate dataRate,
//                       uint32_t portNumber)
// {
//     m_socket = socket;
//     m_peer = address;
//     m_packetSize = packetSize;
//     m_nPackets = nPackets;
//     m_dataRate = dataRate;
//     m_portNumber = portNumber;
// }

// void
// UdpApplication::StartApplication()
// {
//     m_running = true;
//     m_packetsSent = 0;
//     m_socket->Bind();
//     m_socket->Connect(m_peer);
//     SendPacket();
// }

// void
// UdpApplication::StopApplication()
// {
//     m_running = false;

//     if (m_sendEvent.IsRunning())
//     {
//         Simulator::Cancel(m_sendEvent);
//     }

//     if (m_socket)
//     {
//         m_socket->Close();
//     }
// }

// void
// UdpApplication::SendPacket()
// {
//     Ptr<Packet> packet = Create<Packet>(m_packetSize);
//     UdpHeader header;
//     header.SetSourcePort(m_portNumber);
//     packet->AddHeader(header);
//     m_socket->Send(packet);

//     if (++m_packetsSent < m_nPackets)
//     {
//         ScheduleTx();
//     }
// }

// void
// UdpApplication::ScheduleTx()
// {
//     if (m_running)
//     {
//         Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
//         m_sendEvent = Simulator::Schedule(tNext, &UdpApplication::SendPacket, this);
//     }
// }

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

    // eNS_LOG_INFO("Assign IP Addresses.");
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

    Ptr<UdpApplication> clientApps1 = CreateObject<UdpApplication>();
    nodes.Get(0)->AddApplication(clientApps1);
    clientApps1->SetStartTime(Seconds(1.0));
    clientApps1->SetStopTime(Seconds(20.0));
    clientApps1->Setup(socket1, address1, 1000, 1000000, DataRate("10Mbps"), 8888);

    Ptr<UdpApplication> clientApps2 = CreateObject<UdpApplication>();
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