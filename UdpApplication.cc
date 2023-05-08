#include "UdpApplication.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UdpApplication");


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