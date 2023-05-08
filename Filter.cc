#include "ns3/log.h"
#include "Filter.h"
#include "FilterElement.h"
#include "destination_ip_address.h"
#include "destination_mask.h"
#include "destination_port_number.h"
#include "protocol_number.h"
#include "source_ip_address.h"
#include "source_mask.h"
#include "source_port_number.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Filter");
NS_OBJECT_ENSURE_REGISTERED(Filter);

Filter::Filter()
{
    NS_LOG_FUNCTION(this);
    
}

Filter::Filter(DestinationIpAddress* destIpAddr, DestinationMask* destMask, DestinationPortNumber* destPortNum, 
         ProtocolNumber* protNum, SourceIpAddress* sourIpAddr, SourceMask* sourMask, SourcePortNumber* sourPortNum)
{
    // DestinationIpAddress* destinationIpAddress = new DestinationIpAddress("10.1.2.0");         // read from config file
    // DestinationMask* destinationMask = new DestinationMask("255.255.255.0");                   // read from config file
    // DestinationPortNumber* destinationPortNumber = new DestinationPortNumber(9);                // read from config file
    // ProtocolNumber* protocolNumber = new ProtocolNumber(17);                                    // read from config file
    // SourceIpAddress* sourceIpAddress = new SourceIpAddress("10.1.1.0");                         // read from config file
    // SourceMask* sourceMask = new SourceMask("255.255.255.0");                                   // read from config file
    // SourcePortNumber* sourcePortNumber = new SourcePortNumber(8888);                            // read from config file
    elements.push_back(destIpAddr);
    elements.push_back(destMask);
    elements.push_back(destPortNum);
    elements.push_back(protNum);
    elements.push_back(sourIpAddr);
    elements.push_back(sourMask);
    elements.push_back(sourPortNum);
}

Filter::~Filter()
{
    NS_LOG_FUNCTION(this);
}

bool
Filter::match(Ptr<ns3::Packet> p)
{
    for (FilterElement* elem : elements) {
        // AND, if one doesn't match, return false
        if (elem->match(p) == false) {
            return false;
        }
    }
    return true;
}